/* Private glue between the PS2 platform modules. */
#ifndef KH_PS2_INTERNAL_H
#define KH_PS2_INTERNAL_H

#include <stdint.h>

void ps2_log_open_file(void);
void ps2_crash_install(void);

/* IOP bring-up (ps2_iop.c) */
void ps2_iop_reset_and_load_base(void);
int  ps2_iop_load_device_drivers(const char *device);
int  ps2_iop_load_audio(void);
int  ps2_iop_have_filexio(void);

/* VBlank (ps2_time.c) */
void ps2_time_install_vblank(void);

/* GS VRAM accounting (ps2_gs.c) */
uint32_t ps2_gs_vram_used(void);
uint32_t ps2_gs_vram_total(void);
/* Draw an exception screen through the port's 16-bit FIELD renderer.  Returns 0 if video
 * is not initialized yet, so the caller can fall back to libdebug. */
int ps2_gs_crash_screen(const char *title, const char *const *lines, int count);

#endif
