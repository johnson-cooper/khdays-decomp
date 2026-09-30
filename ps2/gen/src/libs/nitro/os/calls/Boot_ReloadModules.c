/* PS2: mechanically prepared copy of libs/nitro/os/calls/Boot_ReloadModules.c (ps2/tools/prep_sources.py). Do not edit. */

#include "nitro/types.h"

extern void func_01ff84b8(u32 rom_offset, u32 dest, u32 size);
extern void DC_InvalidateAll(void);
extern void DC_StoreAll(void);
extern void DC_WaitWriteBufferEmpty(void);
extern void IC_InvalidateAll(void);
extern u32 OS_DisableInterrupts(void);
extern void OS_RestoreInterrupts(u32);

typedef struct {
    u32 arm9_rom_offset;
    u32 arm9_entry;
    u32 arm9_ram_address;
    u32 arm9_size;
    u32 arm7_rom_offset;
    u32 arm7_entry;
    u32 arm7_ram_address;
    u32 arm7_size;
} RomHeaderPart;

void Boot_ReloadModules(void) {
    u32 *base_ptr = (u32 *)((unsigned int)kh_ds_hiram + 0x1fc2c);
    u32 base = base_ptr[0];
    RomHeaderPart *h;
    u32 a9_rom, a9_ram, a9_size;
    u32 a7_rom, a7_ram, a7_size;
    u32 mask;

    if (base >= 0x8000) {
        func_01ff84b8(base, (u32)base_ptr + 0x1d4, 0x160);
    }

    h = (RomHeaderPart *)((unsigned int)kh_ds_hiram + 0x1fe20);
    a9_rom = h->arm9_rom_offset;
    a9_ram = h->arm9_ram_address;
    a9_size = h->arm9_size;
    a7_rom = h->arm7_rom_offset;
    a7_ram = h->arm7_ram_address;
    a7_size = h->arm7_size;

    mask = OS_DisableInterrupts();
    DC_StoreAll();
    DC_InvalidateAll();
    OS_RestoreInterrupts(mask);
    IC_InvalidateAll();
    DC_WaitWriteBufferEmpty();

    a9_rom += base;
    if (a9_rom < 0x8000) {
        u32 chunk = 0x8000 - a9_rom;
        a9_ram += chunk;
        a9_size -= chunk;
        a9_rom = 0x8000;
    }
    func_01ff84b8(a9_rom, a9_ram, a9_size);
    func_01ff84b8(a7_rom + base, a7_ram, a7_size);
}
