/* Platform bring-up test: the first native PS2 milestone.
 *
 * Boots the platform layer, then shows live diagnostics: boot device/dir, pad state, memory,
 * GS VRAM, FPS and whether the game data pack is present.  Built as its own ELF
 * (khdays-platform-test.elf) so the platform can be verified on hardware independently of the
 * game build.
 */
#include "platform/kh_platform.h"

#include <stdio.h>

int main(int argc, char **argv)
{
    uint32_t frame = 0;
    int have_pak;

    kh_platform_init(argc, argv);
    have_pak = kh_file_exists("ps2data/khdays.pak");
    KH_INFO("test", "ps2data/khdays.pak %s", have_pak ? "found" : "NOT found");

    for (;;) {
        const KhPadState *pad;
        const KhProfStats *ps = kh_prof_stats();
        KhMemStats ms;
        int y = 16;

        kh_input_poll();
        pad = kh_input_pad(0);
        kh_mem_get_stats(&ms);

        kh_video_begin_frame(0x102040 + ((frame >> 2) & 0x3f));
        kh_video_debug_text(16, y, 0xffffff, "KH 358/2 Days PS2 - platform test"); y += 12;
        kh_video_debug_text(16, y, 0xc0c0c0, "boot: %s  (%s)", kh_vfs_boot_dir(), kh_vfs_boot_device()); y += 10;
        kh_video_debug_text(16, y, have_pak ? 0x80ff80 : 0xff8080, "ps2data/khdays.pak: %s", have_pak ? "found" : "missing"); y += 10;
        kh_video_debug_text(16, y, 0xc0c0c0, "fps %.1f  vblank %u  %dHz", ps->fps, (unsigned)kh_vblank_count(), kh_video_refresh_hz()); y += 10;
        kh_video_debug_text(16, y, 0xc0c0c0, "EE elf %uK heap used %uK free %uK", ms.ee_elf / 1024, ms.heap_used / 1024, ms.heap_free / 1024); y += 10;
        kh_video_debug_text(16, y, 0xc0c0c0, "GS VRAM %uK / %uK", ms.gs_vram_used / 1024, ms.gs_vram_total / 1024); y += 10;
        kh_video_debug_text(16, y, pad->connected ? 0xffff80 : 0xff8080, "pad0 %s held %04x L(%d,%d) R(%d,%d)",
                            pad->connected ? "ok" : "--", (unsigned)pad->held, pad->lx, pad->ly, pad->rx, pad->ry); y += 10;
        kh_video_end_frame();
        kh_prof_frame();
        if (pad->pressed & KH_BTN_START)
            kh_mem_report();
        frame++;
    }
    return 0;
}
