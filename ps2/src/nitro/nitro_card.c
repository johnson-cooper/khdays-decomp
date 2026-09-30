/* NitroSDK CARD backup (save memory) on the PS2.
 *
 * The game saves to a 64 KiB EEPROM on the DS cartridge (CARD_IdentifyBackup(0x1001)) through
 * CARDi_RequestStreamCommand: request 6 reads backup -> memory, 8 programs memory -> backup.
 * Here the backup is a 64 KiB image in EE RAM, loaded at identification and written back after
 * every program request.
 *
 * Bring-up storage: khdays.sav next to the ELF, written atomically (khdays.sav.tmp, then
 * rename) so an interrupted write never destroys the previous save.  The memory-card save
 * (icon.sys, save directory) replaces this file in a later step; docs/PS2_PORT.md 3.16.
 */
#include "platform/kh_platform.h"
#include "nitro_internal.h"

#include <string.h>

#define BACKUP_SIZE 0x10000
enum { CARD_RESULT_SUCCESS = 0, CARD_RESULT_FAILURE = 1, CARD_RESULT_INVALID_PARAM = 2,
       CARD_RESULT_UNSUPPORTED = 3, CARD_RESULT_NO_RESPONSE = 5 };
enum { REQ_READ_BACKUP = 6, REQ_WRITE_BACKUP = 7, REQ_PROGRAM_BACKUP = 8, REQ_VERIFY_BACKUP = 9,
       REQ_ERASE_PAGE = 10, REQ_ERASE_SECTOR = 11, REQ_ERASE_CHIP = 12 };

typedef void (*CARDCallback)(void *);

static u8 g_backup[BACKUP_SIZE] __attribute__((aligned(16)));
static int g_loaded;
static int g_result;
static const char k_save[] = "khdays.sav";
static const char k_save_tmp[] = "khdays.sav.tmp";

static void load_backup(void)
{
    KhFile *f;
    if (g_loaded)
        return;
    g_loaded = 1;
    memset(g_backup, 0xff, sizeof g_backup);        /* blank EEPROM reads 0xff */
    f = kh_file_open(k_save, 0);
    if (!f) {
        KH_INFO("card", "no save file yet (%s%s)", kh_vfs_boot_dir(), k_save);
        return;
    }
    if (kh_file_read(f, g_backup, BACKUP_SIZE) != BACKUP_SIZE)
        KH_WARN("card", "save file shorter than 64 KiB; rest reads as blank");
    kh_file_close(f);
    KH_INFO("card", "save loaded");
}

static int flush_backup(void)
{
    KhFile *f = kh_file_open(k_save_tmp, 1);
    int ok;
    if (!f) {
        KH_ERR("card", "cannot create %s", k_save_tmp);
        return 0;
    }
    ok = kh_file_write(f, g_backup, BACKUP_SIZE) == BACKUP_SIZE;
    kh_file_close(f);
    if (!ok) {
        kh_file_remove(k_save_tmp);
        KH_ERR("card", "save write failed; previous save kept");
        return 0;
    }
    kh_file_remove(k_save);
    if (kh_file_rename(k_save_tmp, k_save) != 0) {
        KH_ERR("card", "save rename failed; data is in %s", k_save_tmp);
        return 0;
    }
    KH_INFO("card", "saved");
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
    (void)async; (void)retry; (void)mode;
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
        flush_backup();
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
