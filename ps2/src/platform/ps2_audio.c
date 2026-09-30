/* Audio device bring-up.  The SND/NNS SND replacement (ps2/src/audio) builds on this.
 * At this stage it only loads libsd + audsrv on the IOP and initialises audsrv. */
#include "platform/kh_platform.h"
#include "ps2_internal.h"

#include <audsrv.h>

static int g_audio_ok;

int kh_audio_init(void)
{
    if (ps2_iop_load_audio() < 0)
        return -1;
    if (audsrv_init() != 0) {
        KH_ERR("audio", "audsrv_init failed: %s", audsrv_get_error_string());
        return -1;
    }
    g_audio_ok = 1;
    KH_INFO("audio", "audsrv ready");
    return 0;
}

void kh_audio_update(void)
{
    (void)g_audio_ok;
}
