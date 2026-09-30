/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/Ov002_PollLinkRequest.c (ps2/tools/prep_sources.py). Do not edit. */
/*
 * Ov002_PollLinkRequest - read this frame's request and hand it to the record.
 *
 * The queued blit goes out first, then the request block is copied in. While a
 * request is still waiting, an empty one clears the wait and nothing else is
 * sent. Otherwise the request is published only when the session answers, the
 * shutdown hook is idle, the transition key is not held and the panel has
 * nothing pending; if any of those refuses, an empty request is published.
 *
 * ARM.
 */

#include "nitro/types.h"

typedef struct {
    void *pSrc;
    u16 wStride;
    unsigned char bWidth;
    unsigned char bHeight;
    void *pParams;
} Ov002BlitDesc;

typedef struct {
    char pad000[4];
    int bWaiting;
} Ov002LinkCtx;

extern Ov002LinkCtx *data_ov002_0207f9f0;

extern Ov002BlitDesc *func_ov002_0206373c(void);
extern void Ov002_QueueBlit(Ov002BlitDesc *pDesc);
extern u16 *Ov002_CopySourceBlock(u16 *pReq);
extern void Ov002_LinkSyncPostLocal(u16 *pReq);
extern int Ov002_World_GetElementListFlag(void);
extern int Ov002_RunShutdownHook(void);
extern int Ov002_GetPanelField01b0(void);

int Ov002_PollLinkRequest(void)
{
    u16 aReq[4];
    Ov002LinkCtx *ctx;

    ctx = data_ov002_0207f9f0;
    Ov002_QueueBlit(func_ov002_0206373c());
    Ov002_CopySourceBlock(aReq);
    if (ctx->bWaiting != 0) {
        if (aReq[2] == 0) {
            ctx->bWaiting = 0;
        }
    } else if (Ov002_World_GetElementListFlag() != 0 && Ov002_RunShutdownHook() == 0 &&
               ((*(volatile u16 *)((unsigned int)kh_ds_hiram + 0x1ffa8) & 0x8000) >> 15) == 0 &&
               Ov002_GetPanelField01b0() == 0) {
        Ov002_LinkSyncPostLocal(Ov002_CopySourceBlock(aReq));
    } else {
        aReq[2] = 0;
        Ov002_LinkSyncPostLocal(aReq);
    }
    return 0;
}
