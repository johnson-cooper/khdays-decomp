/* PS2: mechanically prepared copy of libs/nitro/mi/auto/MIi_DmaSetChannelRegs.c (ps2/tools/prep_sources.py). Do not edit. */
struct DmaChannel {
    unsigned int src;
    unsigned int dst;
    unsigned int ctrl;
};

void MIi_DmaSetChannelRegs(int ch, unsigned int src, unsigned int dst, unsigned int ctrl)
{
    struct DmaChannel *p = (struct DmaChannel *)((unsigned int)kh_ds_io + 0xb0);
    volatile unsigned int *q = &p[ch].dst;
    p[ch].src = src;
    q[0] = dst;
    q[1] = ctrl;
}
