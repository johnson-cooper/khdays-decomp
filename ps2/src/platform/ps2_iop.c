/* IOP bring-up: reset, patch the loader, load the IRX modules the port needs.
 *
 * All modules are embedded in the ELF by PS2BUILD (`embed_irx` in ps2.yaml), so the port does
 * not depend on rom0 module versions or on files next to the ELF.  Storage drivers are loaded
 * for the device the ELF was started from (and USB is always attempted, so data on USB still
 * works when booting from elsewhere); loading one device's drivers never unloads another's.
 */
#include "platform/kh_platform.h"
#include "ps2_internal.h"

#include <string.h>
#include <kernel.h>
#include <sifrpc.h>
#include <loadfile.h>
#include <iopcontrol.h>
#include <iopheap.h>
#include <sbv_patches.h>

#define IRX(name) extern unsigned char name##_irx[]; extern unsigned int size_##name##_irx;
IRX(sio2man) IRX(padman) IRX(mcman) IRX(mcserv)
IRX(iomanx) IRX(filexio)
IRX(usbd) IRX(bdm) IRX(bdmfs_fatfs) IRX(usbmass_bd)
IRX(mmceman)
IRX(dev9) IRX(atad) IRX(hdd) IRX(fs)
IRX(libsd) IRX(audsrv)
#undef IRX

static int g_have_filexio;
static int g_loaded_usb, g_loaded_mmce, g_loaded_hdd, g_loaded_audio;

int ps2_iop_have_filexio(void) { return g_have_filexio; }

static int load(const char *name, void *buf, unsigned size, const char *args, int arglen)
{
    int res = 0;
    int id = SifExecModuleBuffer(buf, size, arglen, args, &res);
    if (id < 0 || res == 1) {
        KH_ERR("iop", "module %s failed to load (id=%d res=%d)", name, id, res);
        return -1;
    }
    KH_DBG("iop", "loaded %s (id=%d)", name, id);
    return 0;
}
#define LOAD(n) load(#n, n##_irx, size_##n##_irx, NULL, 0)

void ps2_iop_reset_and_load_base(void)
{
    SifInitRpc(0);
#ifndef KH_NO_IOP_RESET
    /* A clean IOP: whatever launched us (OSDSYS, uLaunchELF, OPL, ps2link) left its own modules. */
    while (!SifIopReset("", 0))
        ;
    while (!SifIopSync())
        ;
    SifInitRpc(0);
#endif
    SifInitIopHeap();
    SifLoadFileInit();
    sbv_patch_enable_lmb();
    sbv_patch_disable_prefix_check();

    if (LOAD(iomanx) == 0 && LOAD(filexio) == 0)
        g_have_filexio = 1;
    LOAD(sio2man);
    LOAD(padman);
    LOAD(mcman);
    LOAD(mcserv);
}

static int load_usb(void)
{
    if (g_loaded_usb)
        return 0;
    g_loaded_usb = 1;
    if (LOAD(bdm) || LOAD(bdmfs_fatfs) || LOAD(usbd) || LOAD(usbmass_bd))
        return -1;
    return 0;
}

static int load_mmce(void)
{
    if (g_loaded_mmce)
        return 0;
    g_loaded_mmce = 1;
    return LOAD(mmceman);
}

static int load_hdd(void)
{
    static const char hddarg[] = "-o" "\0" "4" "\0" "-n" "\0" "20";
    static const char pfsarg[] = "-m" "\0" "4" "\0" "-o" "\0" "10" "\0" "-n" "\0" "40";
    if (g_loaded_hdd)
        return 0;
    g_loaded_hdd = 1;
    if (LOAD(dev9) || LOAD(atad))
        return -1;
    if (load("hdd", hdd_irx, size_hdd_irx, hddarg, sizeof hddarg))
        return -1;
    return load("fs", fs_irx, size_fs_irx, pfsarg, sizeof pfsarg);
}

int ps2_iop_load_device_drivers(const char *device)
{
    int rc = 0;
    if (!strcmp(device, "mmce"))
        rc = load_mmce();
    else if (!strcmp(device, "hdd") || !strcmp(device, "pfs"))
        rc = load_hdd();
    /* USB mass storage is always brought up: it is the common case and costs little IOP RAM. */
    if (load_usb() && !strcmp(device, "mass"))
        rc = -1;
    return rc;
}

int ps2_iop_load_audio(void)
{
    if (g_loaded_audio)
        return 0;
    g_loaded_audio = 1;
    if (LOAD(libsd) || LOAD(audsrv))
        return -1;
    return 0;
}
