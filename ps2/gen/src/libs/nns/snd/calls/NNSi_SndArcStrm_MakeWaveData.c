/* PS2: mechanically prepared copy of libs/nns/snd/calls/NNSi_SndArcStrm_MakeWaveData.c (ps2/tools/prep_sources.py). Do not edit. */


#include "nitro/types.h"
#include "nitro/os_types.h"
#include "nitro/pxi.h"
#include "nitro/spi.h"
#include "nitro/snd.h"
#include "nnsys/fnd.h"

#define NNS_SND_HEAP_INVALID_HANDLE NNS_FND_HEAP_INVALID_HANDLE
#define NNS_SND_STRM_PLAYER_NUM 4
#define BLOCK_SIZE 512
#define BLOCK_NUM 4

enum {
    FS_ARCHIVE_NAME_LEN_MAX = 3
};
typedef enum {
    FS_COMMAND_ASYNC_BEGIN = 0,
    FS_COMMAND_READFILE = FS_COMMAND_ASYNC_BEGIN,
    FS_COMMAND_WRITEFILE,
    FS_COMMAND_ASYNC_END,
    FS_COMMAND_SYNC_BEGIN = FS_COMMAND_ASYNC_END,
    FS_COMMAND_SEEKDIR = FS_COMMAND_SYNC_BEGIN,
    FS_COMMAND_READDIR,
    FS_COMMAND_FINDPATH,
    FS_COMMAND_GETPATH,
    FS_COMMAND_OPENFILEFAST,
    FS_COMMAND_OPENFILEDIRECT,
    FS_COMMAND_CLOSEFILE,
    FS_COMMAND_SYNC_END,
    FS_COMMAND_STATUS_BEGIN = FS_COMMAND_SYNC_END,
    FS_COMMAND_ACTIVATE = FS_COMMAND_STATUS_BEGIN,
    FS_COMMAND_IDLE,
    FS_COMMAND_SUSPEND,
    FS_COMMAND_RESUME,
    FS_COMMAND_STATUS_END,
    FS_COMMAND_INVALID
} FSCommandType;
typedef enum {
    FS_RESULT_SUCCESS = 0,
    FS_RESULT_FAILURE,
    FS_RESULT_BUSY,
    FS_RESULT_CANCELED,
    FS_RESULT_CANCELLED = FS_RESULT_CANCELED,
    FS_RESULT_UNSUPPORTED,
    FS_RESULT_ERROR,
    FS_RESULT_PROC_ASYNC,
    FS_RESULT_PROC_DEFAULT,
    FS_RESULT_PROC_UNKNOWN
} FSResult;
struct FSFile;
struct FSFileLink;
struct FSArchive;
typedef FSResult (*FS_ARCHIVE_PROC_FUNC) (struct FSFile *, FSCommandType);
typedef FSResult (*FS_ARCHIVE_READ_FUNC) (struct FSArchive * p, void * dst, u32 pos, u32 size);
typedef FSResult (*FS_ARCHIVE_WRITE_FUNC) (struct FSArchive * p, const void * src, u32 pos, u32 size);
typedef struct FSFileLink {
    struct FSFile * prev;
    struct FSFile * next;
} FSFileLink;
typedef struct FSArchive {
    union {
        char ptr[FS_ARCHIVE_NAME_LEN_MAX + 1];
        u32 pack;
    } name;
    struct FSArchive * next;
    struct FSArchive * prev;
    OSThreadQueue sync_q;
    OSThreadQueue stat_q;
    u32 flag;
    FSFileLink list;
    u32 base;
    u32 fat;
    u32 fat_size;
    u32 fnt;
    u32 fnt_size;
    u32 fat_bak;
    u32 fnt_bak;
    void * load_mem;
    FS_ARCHIVE_READ_FUNC read_func;
    FS_ARCHIVE_WRITE_FUNC write_func;
    FS_ARCHIVE_READ_FUNC table_func;
    FS_ARCHIVE_PROC_FUNC proc;
    u32 proc_flag;
} FSArchive;
typedef int (*MIDeviceReadFunction)(void * userdata, void * buffer, u32 offset, u32 length);
typedef int (*MIDeviceWriteFunction)(void * userdata, const void * buffer, u32 offset, u32 length);
struct FSFile;
typedef struct {
    struct FSArchive * arc;
    u16 own_id;
    u16 index;
    u32 pos;
} FSDirPos;
typedef struct {
    struct FSArchive * arc;
    u32 file_id;
} FSFileID;
typedef struct {
    union {
        FSFileID file_id;
        FSDirPos dir_id;
    };
    u32 is_directory;
    u32 name_len;
    char name[127 + 1];
} FSDirEntry;
typedef struct {
    FSDirPos pos;
} FSSeekDirInfo;
typedef struct {
    FSDirEntry * p_entry;
    BOOL skip_string;
} FSReadDirInfo;
typedef struct {
    FSDirPos pos;
    const char * path;
    BOOL find_directory;
    union {
        FSFileID * file;
        FSDirPos * dir;
    }
    result;
} FSFindPathInfo;
typedef struct {
    u8 * buf;
    u32 buf_len;
    u16 total_len;
    u16 dir_id;
} FSGetPathInfo;
typedef struct {
    FSFileID id;
} FSOpenFileFastInfo;
typedef struct {
    u32 top;
    u32 bottom;
    u32 index;
} FSOpenFileDirectInfo;
typedef struct {
    u32 reserved;
} FSCloseFileInfo;
typedef struct {
    void * dst;
    u32 len_org;
    u32 len;
} FSReadFileInfo;
typedef struct {
    const void * src;
    u32 len_org;
    u32 len;
} FSWriteFileInfo;
typedef struct FSFile {
    FSFileLink link;
    struct FSArchive * arc;
    u32 stat;
    FSCommandType command;
    FSResult error;
    OSThreadQueue queue[1];
    union {
        struct {
            u32 own_id;
            u32 top;
            u32 bottom;
            u32 pos;
        } file;
        struct {
            FSDirPos pos;
            u32 parent;
        } dir;
    }
    prop;
    union {
        FSReadFileInfo readfile;
        FSWriteFileInfo writefile;
        FSSeekDirInfo seekdir;
        FSReadDirInfo readdir;
        FSFindPathInfo findpath;
        FSGetPathInfo getpath;
        FSOpenFileFastInfo openfilefast;
        FSOpenFileDirectInfo openfiledirect;
        FSCloseFileInfo closefile;
    } arg;
} FSFile;
struct NNSSndHeap;
typedef void (*NNSSndHeapDisposeCallback)(void * mem, u32 size, u32 data1, u32 data2);
typedef struct NNSSndHeap * NNSSndHeapHandle;
void * NNS_SndHeapAlloc(NNSSndHeapHandle heap, u32 size, NNSSndHeapDisposeCallback callback, u32 data1, u32 data2);
typedef enum NNSSndStrmFormat {
    NNS_SND_STRM_FORMAT_PCM8,
    NNS_SND_STRM_FORMAT_PCM16
} NNSSndStrmFormat;
typedef enum NNSSndStrmCallbackStatus {
    NNS_SND_STRM_CALLBACK_SETUP,
    NNS_SND_STRM_CALLBACK_INTERVAL
} NNSSndStrmCallbackStatus;
typedef void (*NNSSndStrmCallback)(NNSSndStrmCallbackStatus status, int numChannles, void * buffer[], u32 len, NNSSndStrmFormat format, void * arg);
typedef struct NNSSndStrm {
    NNSFndLink link;
    PMSleepCallbackInfo preSleepInfo;
    PMSleepCallbackInfo postSleepInfo;
    NNSSndStrmFormat format;
    BOOL activeFlag :1;
    BOOL startFlag :1;
    u32 chBufLen;
    int interval;
    NNSSndStrmCallback callback;
    void * callbackArg;
    int curBuffer;
    int volume;
    int alarmNo;
    u32 chBitMask;
    int numChannels;
    u8 channelNo[16 ];
} NNSSndStrm;
struct NNSSndStrmPlayer;
typedef enum NNSSndArcStrmCallbackStatus {
    NNS_SND_ARC_STRM_CALLBACK_DATA_END
} NNSSndArcStrmCallbackStatus;
typedef struct NNSSndArcStrmCallbackInfo {
    int playerNo;
    int strmNo;
} NNSSndArcStrmCallbackInfo;
typedef struct NNSSndArcStrmCallbackParam {
    int strmNo;
    u32 offset;
} NNSSndArcStrmCallbackParam;
typedef BOOL (*NNSSndArcStrmCallback)(NNSSndArcStrmCallbackStatus status, const NNSSndArcStrmCallbackInfo * info, NNSSndArcStrmCallbackParam * param, void * arg);
typedef struct NNSSndStrmHandle {
    struct NNSSndStrmPlayer * player;
} NNSSndStrmHandle;
typedef struct NNSSndStrmThread {
    OSThread thread;
    u64 stack[ 1024 / sizeof(u64) ];
    OSThreadQueue threadQ;
    OSMutex mutex;
    NNSFndList commandList;
} NNSSndStrmThread;
typedef struct NNSSndArcStrmPlayerInfo {
    u8 numChannels;
    u8 chNoList[2];
} NNSSndArcStrmPlayerInfo;
const NNSSndArcStrmPlayerInfo * NNS_SndArcGetStrmPlayerInfo(int playerNo);
typedef struct NNSSndFader {
    int origin;
    int target;
    int counter;
    int frame;
} NNSSndFader;
struct NNSSndStrmPlayer;
typedef BOOL (*OpenStreamFunc)(struct NNSSndStrmPlayer * player, u32 fileId);
typedef void (*CloseStreamFunc)(struct NNSSndStrmPlayer * player);
typedef s32 (*ReadStreamFunc)(struct NNSSndStrmPlayer * player, void * dest, u32 size, u32 offset);
typedef void (*CancelStreamFunc)(struct NNSSndStrmPlayer * player);
typedef struct NNSSndStrmData {
    struct SNDBinaryFileHeader fileHeader;
    struct SNDBinaryBlockHeader blockHeader;
    u8 format;
    u8 loopFlag;
    u8 numChannels;
    u8 pad_;
    u16 sampleRate;
    u16 timer;
    u32 loopStart;
    u32 loopEnd;
    u32 dataOffset;
    u32 numBlocks;
    u32 blockSize;
    u32 blockSamples;
    u32 lastBlockSize;
    u32 lastBlockSamples;
} NNSSndStrmData;
typedef struct AdpcmState {
    s16 prevSample;
    u8 prevIndex;
    u8 padding;
} AdpcmState;
typedef struct NNSSndStrmPlayer {
    NNSSndStrm stream;
    FSFile file;
    u32 fileOffset;
    NNSSndStrmData info;
    NNSSndFader fader;
    AdpcmState adpcmState[6 ];
    BOOL activeFlag  : 1;
    BOOL playFlag    : 1;
    BOOL startFlag   : 1;
    BOOL fadeOutFlag : 1;
    BOOL dirtyFlag   : 1;
    BOOL finishFlag  : 1;
    BOOL monoFlag    : 1;
    volatile int finishCounter;
    volatile BOOL prepareFlag;
    volatile int commandCount;
    int allocChannelCount;
    u8 numChannels;
    u8 padding;
    u8 chNoList[6 ];
    void * buffer;
    u32 bufSize;
    NNSSndStrmCallback strmCallback;
    void * strmCallbackArg;
    NNSSndArcStrmCallback sndArcStrmCallback;
    void * sndArcStrmCallbackArg;
    int strmNo;
    int playerNo;
    NNSSndStrmHandle * handle;
    int prio;
    int initVolume;
    int extVolume;
    int volume;
    u32 curSample;
    OpenStreamFunc openStreamFunc;
    CloseStreamFunc closeStreamFunc;
    ReadStreamFunc readStreamFunc;
    CancelStreamFunc cancelStreamFunc;
} NNSSndStrmPlayer;
#define ADPCM_INDEX_NUM 89
#define STRM_CHANNEL_MAX 6

typedef enum StrmFormat {
    STRM_FORMAT_PCM8,
    STRM_FORMAT_PCM16,
    STRM_FORMAT_ADPCM
} StrmFormat;

typedef struct LoadCommand {
    NNSFndLink link;
    NNSSndStrmPlayer * player;
    NNSSndStrmCallbackStatus status;
    int numChannels;
    void * buffer[STRM_CHANNEL_MAX];
    u32 bufLen;
} LoadCommand;

extern const s8 data_02041b04[16];                 /* cAdpcmIndexTable */
extern const s16 data_02041b14[ADPCM_INDEX_NUM];   /* cAdpcmStepSizeTable */
#define cAdpcmIndexTable data_02041b04
#define cAdpcmStepSizeTable data_02041b14
extern OSMutex data_0204ada4;                      /* sDecodeBufferMutex */
#define sDecodeBufferMutex data_0204ada4
void OS_LockMutex(OSMutex * mutex);
void OS_UnlockMutex(OSMutex * mutex);
void MI_CpuFill8(void * dest, u8 data, u32 size);
void MI_CpuCopy8(const void * src, void * dest, u32 size);
void DC_FlushRange(const void * startAddr, u32 nBytes);
void OnDataEnd(NNSSndStrmPlayer * player);     /* OnDataEnd */
#define MI_CpuClear8(dest, size) MI_CpuFill8((dest), 0, (size))

/* khdays: shared-bss */
NNSSndStrmThread * sPrepareThread;   /* sPrepareThread */
BOOL data_0204ad8c;   /* initialized$3434 */
u8 * sDecodeBuffer;   /* sDecodeBuffer */

static inline s16 DecodeAdpcm (int code, AdpcmState * state)
{
    int step;
    int sample;
    int index;
    int d;

    sample = state->prevSample;
    index = state->prevIndex;

    step = cAdpcmStepSizeTable[ index ];

    d = step >> 3;
    if (code & 4) d += step;
    if (code & 2) d += step >> 1;
    if (code & 1) d += step >> 2;

    if (code & 8) {
        sample -= d;
        if (sample < -32768) sample = -32768;
    } else {
        sample += d;
        if (sample > 32767) sample = 32767;
    }

    index += cAdpcmIndexTable[ code ];

    if (index < 0) index = 0;
    else if (index > ADPCM_INDEX_NUM - 1) index = ADPCM_INDEX_NUM - 1;

    state->prevSample = (s16)sample;
    state->prevIndex = (u8)index;

    return (s16)sample;
}

/* NNSi_SndArcStrm_MakeWaveData -- NitroSystem sndarc_stream.c: MakeWaveData. Codegen: `destOffset` is a `long`; as the
 * source's u32 mwcc spills it last instead of in its declaration-order stack slot. */
void NNSi_SndArcStrm_MakeWaveData (LoadCommand * command)
{
    NNSSndStrmPlayer * player = command->player;
    BOOL loopFlag;
    long destOffset;
    u32 restSize;
    u32 blockNo;
    u32 blockSize;
    u32 blockSamples;
    u32 blockOffsetSample;
    u32 blockOffset;
    u32 offset;
    u32 samples;
    u32 size;
    u32 readSize;
    int ch;

    if (player->finishFlag && player->finishCounter > 0) {
        player->finishCounter--;
    }

    destOffset = 0;

    restSize = command->bufLen;
    while (restSize > 0) {
        if (player->finishFlag) {
            for (ch = 0; ch < command->numChannels; ch++) {
                MI_CpuClear8((u8 *)(command->buffer[ ch ]) + destOffset, restSize);
            }
            break;
        }

        blockNo = player->curSample / player->info.blockSamples;

        if (blockNo < player->info.numBlocks - 1) {
            blockSize = player->info.blockSize;
            blockSamples = player->info.blockSamples;
        } else {
            blockSize = player->info.lastBlockSize;
            blockSamples = player->info.lastBlockSamples;
        }

        blockOffsetSample = player->curSample;
        blockOffsetSample -= blockNo * player->info.blockSamples;

        samples = restSize;
        if (player->info.format != STRM_FORMAT_PCM8) {
            samples >>= 1;
        }

        if (player->dirtyFlag) {
            if (blockOffsetSample == 0) {
                player->dirtyFlag = FALSE;
            } else {
                samples = blockOffsetSample;
                blockOffsetSample = 0;
            }
        }

        loopFlag = FALSE;
        if (blockOffsetSample + samples >= blockSamples) {
            samples = blockSamples - blockOffsetSample;

            if (blockNo >= player->info.numBlocks - 1) {
                if (player->info.loopFlag) {
                    loopFlag = TRUE;
                } else {
                    player->finishFlag = TRUE;
                }
            }
        }

        blockOffset = blockOffsetSample;
        size = samples;
        switch (player->info.format) {
        case STRM_FORMAT_PCM8:
            readSize = size;
            break;
        case STRM_FORMAT_PCM16:
            blockOffset <<= 1;
            size <<= 1;
            readSize = size;
            break;
        case STRM_FORMAT_ADPCM: {
            u32 endSample = blockOffsetSample + samples;
            blockOffset >>= 1;
            endSample++; endSample >>= 1;
            readSize = endSample - blockOffset;
            if (blockOffsetSample == 0) {
                readSize += sizeof(AdpcmState);
            } else {
                blockOffset += sizeof(AdpcmState);
            }
            size <<= 1;

            break;
        }
        }

        offset = blockOffset;
        offset += blockNo * player->info.blockSize * player->info.numChannels;
        offset += player->info.dataOffset;

        for (ch = 0; ch < command->numChannels; ch++) {
            void * dest;
            void * read_dest;

            dest = read_dest = (u8 *)(command->buffer[ ch ]) + destOffset;

            if (ch < player->info.numChannels) {
                s32 resultSize;

                if (player->info.format == STRM_FORMAT_ADPCM) {
                    OS_LockMutex(&sDecodeBufferMutex);
                    read_dest = sDecodeBuffer;
                }

                resultSize = player->readStreamFunc(
                    player,
                    read_dest,
                    readSize,
                    offset + ch * blockSize
                    );

                if (resultSize != readSize) {
                    size = 0;
                    samples = 0;
                    loopFlag = FALSE;
                    player->finishFlag = TRUE;
                    if (player->info.format == STRM_FORMAT_ADPCM) {
                        OS_UnlockMutex(&sDecodeBufferMutex);
                    }
                    break;
                }

                if (player->info.format == STRM_FORMAT_ADPCM) {
                    AdpcmState * state = &player->adpcmState[ ch ];
                    u8 * srcp = sDecodeBuffer;
                    s16 * destp = dest;
                    u32 i;
                    u32 end;

                    if (blockOffsetSample == 0) {
                        *state = *((*(AdpcmState * *)&(srcp))++);
                    }

                    end = blockOffsetSample + samples;

                    i = blockOffsetSample;
                    if (i & 0x01) {
                        *destp++ = DecodeAdpcm((*srcp >> 4) & 0x0f, state); i++;
                        srcp++;
                    }
                    while (i < (end & ~0x01)) {
                        *destp++ = DecodeAdpcm(*srcp & 0x0f, state); i++;
                        *destp++ = DecodeAdpcm((*srcp >> 4) & 0x0f, state); i++;
                        srcp++;
                    }
                    if (i < end) {
                        *destp++ = DecodeAdpcm(*srcp & 0x0f, state); i++;
                    }
                    OS_UnlockMutex(&sDecodeBufferMutex);
                }
            } else {
                if (player->monoFlag) {
                    MI_CpuClear8(dest, size);
                } else {
                    MI_CpuCopy8((u8 *)(command->buffer[ 0 ]) + destOffset, dest, size);
                }
            }
        }

        if (player->dirtyFlag) {
            player->dirtyFlag = FALSE;
            continue;
        }

        if (loopFlag) {
            player->curSample = player->info.loopStart;
        } else {
            player->curSample += samples;
        }

        destOffset += size;

        restSize -= size;

        if (player->finishFlag && player->sndArcStrmCallback) {
            OnDataEnd(player);
        }
    }

    if (player->strmCallback != NULL) {
        player->strmCallback(
            command->status,
            command->numChannels,
            command->buffer,
            command->bufLen,
            player->info.format == STRM_FORMAT_PCM8 ? NNS_SND_STRM_FORMAT_PCM8 : NNS_SND_STRM_FORMAT_PCM16,
            player->strmCallbackArg
            );
    }

    for (ch = 0; ch < command->numChannels; ch++) {
        DC_FlushRange(command->buffer[ ch ], command->bufLen);
    }

    if (command->status == NNS_SND_STRM_CALLBACK_SETUP) {
        player->prepareFlag = TRUE;
    }
}
