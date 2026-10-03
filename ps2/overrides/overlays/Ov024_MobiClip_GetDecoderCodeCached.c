/* PS2 override: the MobiClip frame decoder is the portable C++ one.
 *
 * On the DS this copies the position-independent ARM payload (data_ov024_0208c8c4) into the ITCM
 * arena and returns the copy; Ov024_MobiClip_OpenContainer stores it at decoder +0x38 and
 * Ov024_MobiClip_DecodeFrame calls it with the 0x454-byte decoder state.  The EE cannot run ARM,
 * so the entry returned here is MobiClip_DecodeFrameCore (libs/mobiclip/video/portable), the same
 * routine on the same state and planes (verified against the payload and FFmpeg, see
 * docs/MOBICLIP_DECODER.md).
 */
#include "platform/kh_platform.h"

extern int MobiClip_DecodeFrameCore(void *pState);

void *Ov024_MobiClip_GetDecoderCodeCached(void)
{
    KH_INFO("movie", "MobiClip stream opened: portable frame decoder bound");
    return (void *)MobiClip_DecodeFrameCore;
}
