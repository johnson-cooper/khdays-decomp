/* PS2: mechanically prepared copy of libs/nitro/os/calls/Game_ReadLocalProfile.c (ps2/tools/prep_sources.py). Do not edit. */
extern void MIi_CpuCopy16(const void *src, void *dst, unsigned int size);

typedef struct {
    unsigned char pad0[0x64];
    unsigned short bits : 3;
    unsigned short : 13;
} SrcA;

typedef struct {
    unsigned char pad0[2];
    unsigned char bits4 : 4;
    unsigned char : 4;
} SrcB;

void Game_ReadLocalProfile(unsigned char *r0) {
    unsigned char *p = (unsigned char *)((unsigned int)kh_ds_hiram + 0x1fc80);
    r0[0] = (unsigned char)((SrcA *)p)->bits;
    r0[1] = ((SrcB *)p)->bits4;
    r0[2] = p[3];
    r0[3] = p[4];
    *(unsigned short *)(r0 + 0x1a) = (unsigned short)p[0x1a];
    *(unsigned short *)(r0 + 0x52) = (unsigned short)p[0x50];
    MIi_CpuCopy16(p + 6, r0 + 4, 0x14);
    MIi_CpuCopy16(p + 0x1c, r0 + 0x1c, 0x34);
    *(unsigned short *)(r0 + 0x18) = 0;
    *(unsigned short *)(r0 + 0x50) = 0;
}
