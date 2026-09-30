/* PS2: mechanically prepared copy of libs/nitro/os/auto/OS_SetArenaLo.c (ps2/tools/prep_sources.py). Do not edit. */
void OS_SetArenaLo(int i, int v){ *(int *)(((unsigned int)kh_ds_hiram + 0x1f000) + (i << 2) + 3488) = v; }
