/* NitroSDK CARD backup (save memory) on the PS2.
 *
 * The game saves to a 64 KiB EEPROM on the DS cartridge (CARD_IdentifyBackup(0x1001)) through
 * CARDi_RequestStreamCommand: request 6 reads backup -> memory, 8 programs memory -> backup.
 * Here the backup is a 64 KiB image in EE RAM, loaded at identification and written back after
 * every program request.
 *
 * Bring-up storage: two slot files next to the ELF, khdays_a.sav and khdays_b.sav, each the
 * 64 KiB image followed by a trailer {magic, version, sequence, CRC-32}.  A save overwrites the
 * slot that does NOT hold the newest valid image, so an interrupted write (power loss, card
 * pulled) leaves the previous save intact: at load time the torn slot fails its CRC and the
 * other one is used.  This needs no rename, which several PS2 devices cannot do (PCSX2's host:
 * is a legacy ioman device without a rename op; the removal-then-rename used before was not
 * atomic anyway).  A raw 64 KiB khdays.sav from older builds is still read if no slot is valid.
 * The memory-card save (icon.sys, save directory) replaces these files in a later step;
 * docs/PS2_PORT.md 3.16.
 */
#include "platform/kh_platform.h"
#include "nitro_internal.h"

#include <string.h>
#include <stdio.h>

extern volatile const char *kh_watchdog_mark;   /* ps2_watchdog.c breadcrumb */

#define BACKUP_SIZE 0x10000
enum { CARD_RESULT_SUCCESS = 0, CARD_RESULT_FAILURE = 1, CARD_RESULT_INVALID_PARAM = 2,
       CARD_RESULT_UNSUPPORTED = 3, CARD_RESULT_NO_RESPONSE = 5 };
enum { REQ_READ_BACKUP = 6, REQ_WRITE_BACKUP = 7, REQ_PROGRAM_BACKUP = 8, REQ_VERIFY_BACKUP = 9,
       REQ_ERASE_PAGE = 10, REQ_ERASE_SECTOR = 11, REQ_ERASE_CHIP = 12 };

typedef void (*CARDCallback)(void *);

#define SLOT_MAGIC   0x5653484bu    /* "KHSV" */
#define SLOT_VERSION 1u

typedef struct {
    u32 magic, version, seq, crc;   /* crc: CRC-32 of the image, then of seq (little endian) */
} SlotTrailer;

static u8 g_backup[BACKUP_SIZE] __attribute__((aligned(16)));
static u8 g_scratch[BACKUP_SIZE] __attribute__((aligned(16)));
static int g_loaded;
static int g_result;
static int g_slot = -1;         /* slot holding the newest valid image, -1: none yet */
static u32 g_seq;
static const char *const k_slot[2] = { "khdays_a.sav", "khdays_b.sav" };
static const char k_legacy[] = "khdays.sav";

static u32 crc32_update(u32 crc, const u8 *p, u32 n)
{
    static u32 table[256];
    u32 i;
    if (!table[1]) {
        for (i = 0; i < 256; i++) {
            u32 c = i;
            int k;
            for (k = 0; k < 8; k++)
                c = (c & 1) ? 0xedb88320u ^ (c >> 1) : c >> 1;
            table[i] = c;
        }
    }
    crc = ~crc;
    while (n--)
        crc = table[(crc ^ *p++) & 0xff] ^ (crc >> 8);
    return ~crc;
}

static u32 slot_crc(const u8 *image, u32 seq)
{
    u8 s[4] = { (u8)seq, (u8)(seq >> 8), (u8)(seq >> 16), (u8)(seq >> 24) };
    return crc32_update(crc32_update(0, image, BACKUP_SIZE), s, 4);
}

/* Reads slot i into dst; 1 with *seq set if it holds a complete, intact image. */
static int read_slot(int i, u8 *dst, u32 *seq)
{
    SlotTrailer t;
    KhFile *f = kh_file_open(k_slot[i], 0);
    int ok;
    if (!f)
        return 0;
    ok = kh_file_read(f, dst, BACKUP_SIZE) == BACKUP_SIZE
         && kh_file_read(f, &t, sizeof t) == (int32_t)sizeof t;
    kh_file_close(f);
    if (!ok || t.magic != SLOT_MAGIC || t.version != SLOT_VERSION) {
        KH_WARN("card", "%s is incomplete or not a save slot; ignored", k_slot[i]);
        return 0;
    }
    if (t.crc != slot_crc(dst, t.seq)) {
        KH_WARN("card", "%s fails its CRC (interrupted write?); ignored", k_slot[i]);
        return 0;
    }
    *seq = t.seq;
    return 1;
}

static void load_backup(void)
{
    u32 seq[2];
    int valid[2], i;
    KhFile *f;

    if (g_loaded)
        return;
    g_loaded = 1;
    memset(g_backup, 0xff, sizeof g_backup);        /* blank EEPROM reads 0xff */

    valid[0] = read_slot(0, g_backup, &seq[0]);
    valid[1] = read_slot(1, g_scratch, &seq[1]);
    if (valid[0] || valid[1]) {
        /* newest by serial-number arithmetic, so the sequence may wrap */
        i = !valid[0] || (valid[1] && (s32)(seq[1] - seq[0]) > 0);
        if (i == 1)
            memcpy(g_backup, g_scratch, BACKUP_SIZE);
        g_slot = i;
        g_seq = seq[i];
        KH_INFO("card", "save loaded from %s (sequence %u)", k_slot[i], (unsigned)g_seq);
        return;
    }
    memset(g_backup, 0xff, sizeof g_backup);        /* slot 0 may have been read partially */
    f = kh_file_open(k_legacy, 0);
    if (!f) {
        KH_INFO("card", "no save yet (%s%s)", kh_vfs_boot_dir(), k_slot[0]);
        return;
    }
    if (kh_file_read(f, g_backup, BACKUP_SIZE) != BACKUP_SIZE)
        KH_WARN("card", "%s shorter than 64 KiB; rest reads as blank", k_legacy);
    kh_file_close(f);
    KH_INFO("card", "save loaded from legacy %s; next save goes to %s", k_legacy, k_slot[0]);
}

/* Writes the image to the slot not holding the newest save.  Returns 1 only once the whole
 * slot, trailer included, was written and closed without error. */
static int flush_backup(void)
{
    int target = g_slot == 0 ? 1 : 0;
    u32 seq = g_seq + 1;
    SlotTrailer t = { SLOT_MAGIC, SLOT_VERSION, seq, 0 };
    KhFile *f;
    int ok;

    t.crc = slot_crc(g_backup, seq);
    if (kh_vblank_count() < KH_BOOT_TRACE_VBLANKS)
        KH_INFO("card", "write: create %s", k_slot[target]);
    f = kh_file_open(k_slot[target], 1);
    if (!f) {
        KH_ERR("card", "cannot create %s%s", kh_vfs_boot_dir(), k_slot[target]);
        return 0;
    }
    kh_watchdog_mark = "card: writing the save slot";
    if (kh_vblank_count() < KH_BOOT_TRACE_VBLANKS)
        KH_INFO("card", "write: 64 KiB + trailer");
    ok = kh_file_write(f, g_backup, BACKUP_SIZE) == BACKUP_SIZE;
    ok = ok && kh_file_write(f, &t, sizeof t) == (int32_t)sizeof t;
    kh_watchdog_mark = "card: closing the save slot";
    if (kh_vblank_count() < KH_BOOT_TRACE_VBLANKS)
        KH_INFO("card", "write: close");
    ok = kh_file_close(f) == 0 && ok;
    kh_watchdog_mark = "card: save slot written";
    if (!ok) {
        KH_ERR("card", "writing %s failed; the previous save is kept", k_slot[target]);
        return 0;
    }
    g_slot = target;
    g_seq = seq;
    KH_INFO("card", "saved to %s (sequence %u)", k_slot[target], (unsigned)seq);
    return 1;
}

int CARD_IdentifyBackup(u32 type)
{
    KH_INFO("card", "backup type 0x%x", (unsigned)type);
    load_backup();
    g_result = CARD_RESULT_SUCCESS;
    return 1;
}

int CARDi_RequestStreamCommand(u32 src, u32 dst, u32 len, CARDCallback cb, void *arg, int async,
                               s32 req, s32 retry, s32 mode)
{
    static char mark[64];
    (void)async; (void)retry; (void)mode;
    snprintf(mark, sizeof mark, "card request %d (0x%x bytes)", (int)req, (unsigned)len);
    kh_watchdog_mark = mark;
    if (kh_vblank_count() < KH_BOOT_TRACE_VBLANKS)
        KH_INFO("card", "request %d: src 0x%x dst 0x%x len 0x%x", (int)req, (unsigned)src, (unsigned)dst, (unsigned)len);
    load_backup();
    g_result = CARD_RESULT_SUCCESS;
    switch (req) {
    case REQ_READ_BACKUP:
        if (src + len > BACKUP_SIZE) { g_result = CARD_RESULT_INVALID_PARAM; break; }
        memcpy((void *)(uintptr_t)dst, g_backup + src, len);
        break;
    case REQ_WRITE_BACKUP:
    case REQ_PROGRAM_BACKUP:
        if (dst + len > BACKUP_SIZE) { g_result = CARD_RESULT_INVALID_PARAM; break; }
        memcpy(g_backup + dst, (const void *)(uintptr_t)src, len);
        if (!flush_backup())
            g_result = CARD_RESULT_FAILURE;
        break;
    case REQ_VERIFY_BACKUP:
        if (dst + len > BACKUP_SIZE || memcmp(g_backup + dst, (const void *)(uintptr_t)src, len))
            g_result = CARD_RESULT_FAILURE;
        break;
    case REQ_ERASE_CHIP:
        memset(g_backup, 0xff, sizeof g_backup);
        if (!flush_backup())
            g_result = CARD_RESULT_FAILURE;
        break;
    default:
        KH_WARN("card", "backup request %d not supported", (int)req);
        g_result = CARD_RESULT_UNSUPPORTED;
        break;
    }
    if (cb)
        cb(arg);
    return g_result == CARD_RESULT_SUCCESS;
}

int CARD_GetResultCode(void) { return g_result; }
int func_0200f274(void) { return g_result; }          /* CARD_GetResultCode (FS copy) */
void CARD_UnlockBackup(int id) { (void)id; }
void CARD_LockBackup(int id) { (void)id; }
void CardUnlockAfterKeyShare(int id) { (void)id; }
void *CARD_TryWaitRomAsync(void) { return (void *)1; }   /* nothing asynchronous in flight */
int CARD_WaitBackupAsync(void) { return 1; }
int CARD_TryWaitBackupAsync(void) { return 1; }
void StoreGlobalPairAt118(int a, int b) { (void)a; (void)b; }  /* CARD_SetCacheFlushThreshold */
