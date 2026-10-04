/* PS2 override: recover a stale NitroFS archive owner instead of sleeping forever.
 *
 * NitroSDK serializes every archive command with FS_ARCHIVE_FLAG_RUNNING plus an intrusive
 * command list.  A synchronous command queued behind an active owner sleeps until
 * FSi_NextCommand marks it OPERATING and wakes its file queue.
 *
 * On the PS2 port the low-level ROM callback is synchronous, but the DS command scheduler is
 * shared by several real EE threads.  Hardware debugging caught the main thread asleep inside
 * FS_OpenFile("/db/db.p2") while the ROM archive reported RUNNING even though the newly appended
 * command was the *only* command in its list.  With no preceding list node there is nobody that
 * can ever call FSi_NextCommand, so the stock path sleeps forever.
 *
 * Preserve the NitroSDK scheduler verbatim except for that provably stale state.  If the ROM
 * archive is RUNNING, not suspended, and the just-appended synchronous command is the sole list
 * entry, claim it as OPERATING immediately and execute it on the caller.  FSi_ExecuteSyncCommand
 * then performs the normal release/NextCommand path, which clears RUNNING when the list drains.
 */
#include "nitro/types.h"
#include "nitro/fs.h"

extern FSResult FSi_TranslateCommand(FSFile *p_file, FSCommandType command);
extern void FSi_ReleaseCommand(FSFile *p_file, FSResult ret);
extern void FSi_ExecuteAsyncCommand(FSFile *p_file);
extern FSFile *FSi_NextCommand(FSArchive *p_arc);
extern BOOL FSi_ExecuteSyncCommand(FSFile *p_file);
extern void OS_SleepThread(OSThreadQueue *queue);
extern void OS_WakeupThread(OSThreadQueue *queue);
extern OSIntrMode OS_DisableInterrupts(void);
extern OSIntrMode OS_RestoreInterrupts(OSIntrMode state);

extern FSArchive data_02046334; /* fsi_arc_rom */

static inline BOOL fs_is_archive_running(const FSArchive *arc)
{
    return (arc->flag & FS_ARCHIVE_FLAG_RUNNING) != 0;
}

static inline BOOL fs_is_archive_suspended(const FSArchive *arc)
{
    return (arc->flag & FS_ARCHIVE_FLAG_SUSPEND) != 0;
}

static inline BOOL fs_is_file_sync(const FSFile *file)
{
    return (file->stat & FS_FILE_STATUS_SYNC) != 0;
}

static void fs_cut_from_list_core(FSFileLink *link)
{
    FSFile *prev = link->prev;
    FSFile *next = link->next;

    if (prev)
        prev->link.next = next;
    if (next)
        next->link.prev = prev;
}

static void fs_append_to_list(FSFile *file, FSFile *list)
{
    FSFileLink *link = &file->link;

    fs_cut_from_list_core(link);
    while (list->link.next)
        list = list->link.next;
    list->link.next = file;
    link->prev = list;
    link->next = NULL;
}

BOOL FSi_SendCommand(FSFile *p_file, FSCommandType command)
{
    FSArchive *const p_arc = p_file->arc;
    const int bit = 1 << command;
    OSIntrMode bak_psr;

    p_file->command = command;
    p_file->error = FS_RESULT_BUSY;
    p_file->stat |= FS_FILE_STATUS_BUSY;

    bak_psr = OS_DisableInterrupts();

    if (p_arc->flag & FS_ARCHIVE_FLAG_UNLOADING) {
        FSi_ReleaseCommand(p_file, FS_RESULT_CANCELED);
        (void)OS_RestoreInterrupts(bak_psr);
        return FALSE;
    }

    if ((bit & FS_ARCHIVE_PROC_SYNC) != 0)
        p_file->stat |= FS_FILE_STATUS_SYNC;

    fs_append_to_list(p_file, (FSFile *)&p_arc->list);

    if (!fs_is_archive_suspended(p_arc) && !fs_is_archive_running(p_arc)) {
        p_arc->flag |= FS_ARCHIVE_FLAG_RUNNING;
        (void)OS_RestoreInterrupts(bak_psr);

        if ((p_arc->proc_flag & FS_ARCHIVE_PROC_ACTIVATE) != 0)
            (void)(*p_arc->proc)(p_file, FS_COMMAND_ACTIVATE);

        bak_psr = OS_DisableInterrupts();
        p_file->stat |= FS_FILE_STATUS_OPERATING;

        if (!fs_is_file_sync(p_file)) {
            (void)OS_RestoreInterrupts(bak_psr);
            FSi_ExecuteAsyncCommand(p_file);
            return TRUE;
        }

        (void)OS_RestoreInterrupts(bak_psr);
    } else if (!fs_is_file_sync(p_file)) {
        (void)OS_RestoreInterrupts(bak_psr);
        return TRUE;
    } else {
        /*
         * PS2 hardware recovery:
         *
         * We just appended p_file at the tail.  If it is simultaneously the list head and has
         * no successor, it is the only queued command.  A RUNNING archive in that state has no
         * command owner that could wake this synchronous waiter.  This is exactly the deadlock
         * observed after New Game when opening /db/db.p2.
         *
         * Restrict the recovery to the ROM archive and never do it while suspended.
         */
        if (p_arc == &data_02046334 &&
            !fs_is_archive_suspended(p_arc) &&
            fs_is_archive_running(p_arc) &&
            p_arc->list.next == p_file &&
            p_file->link.next == NULL) {
            p_file->stat |= FS_FILE_STATUS_OPERATING;
            (void)OS_RestoreInterrupts(bak_psr);
        } else {
            do {
                OS_SleepThread(p_file->queue);
            } while (!(p_file->stat & FS_FILE_STATUS_OPERATING));
            (void)OS_RestoreInterrupts(bak_psr);
        }
    }

    return FSi_ExecuteSyncCommand(p_file);
}
