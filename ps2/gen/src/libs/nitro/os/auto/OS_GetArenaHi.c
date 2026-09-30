/* PS2: mechanically prepared copy of libs/nitro/os/auto/OS_GetArenaHi.c (ps2/tools/prep_sources.py). Do not edit. */
int OS_GetArenaHi(int i){ return *(int *)(((unsigned int)kh_ds_hiram + 0x1f000) + (i << 2) + 3524); }
