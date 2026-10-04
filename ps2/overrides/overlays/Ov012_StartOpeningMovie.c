/* PS2 override: opening-scene MobiClip start.
 *
 * The DS code treats a failed stream open as a flag and lets the script wait forever.  That is
 * unhelpful on real PS2 hardware, where there is no debugger attached.  Keep the original setup
 * and behaviour, but make a debug build stop on a clear diagnostic screen if the movie stream
 * cannot be opened.  Release builds preserve the original flag-only path.
 */
#include "nitro/types.h"
#include "platform/kh_platform.h"

typedef struct MobiClipHeader {
    u16 destinationX;
    u16 destinationY;
    u16 macroblockWidth;
    u16 macroblockHeight;
    u16 tileBase;
    u16 palette;
    u16 flags;
    u16 cropLeft;
    u16 cropRight;
    u16 cropTop;
    u16 cropBottom;
} MobiClipHeader;

typedef struct MobiClipOpenRequest {
    char *stream0;
    char *stream1;
    char *stream2;
    void (*frameCallback)(void);
} MobiClipOpenRequest;

extern char *data_ov012_0205cb20;
extern void Ov012_UpdateOpeningGlobals(void);
extern void Ov012_InitOpeningRendererFromMobiClipHeader(void *renderer, int layer, void *font,
                                                        MobiClipHeader *header);
extern u16 *GetBGScreenBaseForLayer(int layer);
extern void Tilemap_FillRect(u16 *tilemap, int width, int height, int x, int y, int mapWidth,
                             int tile, int palette);
extern void Ov012_TileTextRenderer_SetReady(void *renderer, int ready);
extern void MsgQueue_SetMoviePlaying(int value);
extern void Session_SetMoviePlaying(int value);
extern int Ov024_MobiClip_OpenStreams(MobiClipOpenRequest *request);
#if KH_PS2_DEBUG
extern volatile const char *kh_watchdog_mark;
#endif

void Ov012_StartOpeningMovie(char *streamName)
{
    char *context;
    MobiClipOpenRequest request;
    MobiClipHeader header;

    context = data_ov012_0205cb20;
    request.stream0 = 0;
    request.stream1 = streamName;
    request.stream2 = 0;
    request.frameCallback = Ov012_UpdateOpeningGlobals;

    *(int *)(context + 0x8bdc) = 1;

    header.destinationX = 0;
    header.destinationY = 0x14;
    header.macroblockWidth = 0x20;
    header.macroblockHeight = 2;
    header.tileBase = 1;
    header.palette = 0xd;
    header.flags = 0;
    header.cropLeft = 0;
    header.cropRight = 0;
    header.cropTop = 3;
    header.cropBottom = 0;

    Ov012_InitOpeningRendererFromMobiClipHeader(context + 0x8b4c, 4, context + 0x8b40, &header);

    Tilemap_FillRect(GetBGScreenBaseForLayer(5), header.macroblockWidth, header.macroblockHeight,
                     header.destinationX, header.destinationY, 0x20, header.tileBase, 0xe);
    Tilemap_FillRect(GetBGScreenBaseForLayer(6), header.macroblockWidth, header.macroblockHeight,
                     header.destinationX, header.destinationY, 0x20, header.tileBase, 0xe);

    Ov012_TileTextRenderer_SetReady(context + 0x8b4c, 1);
    MsgQueue_SetMoviePlaying(1);
    Session_SetMoviePlaying(1);

#if KH_PS2_DEBUG
    kh_watchdog_mark = "opening movie: Ov024_MobiClip_OpenStreams";
#endif
    if (Ov024_MobiClip_OpenStreams(&request) == 0) {
        *(u16 *)(context + 2) |= 2;
#if KH_PS2_DEBUG
        kh_panic("Opening MobiClip stream failed to open:\n%s",
                 streamName ? streamName : "(null)");
#endif
    }
#if KH_PS2_DEBUG
    kh_watchdog_mark = "opening movie: stream opened";
#endif
}
