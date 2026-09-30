/* PS2: mechanically prepared copy of libs/nitro/nns/calls/GX_SendFifoWords.c (ps2/tools/prep_sources.py). Do not edit. */


#include "nitro/types.h"
#include "nnsys/g3d.h"

extern NNSG3dGeBuffer *data_027e0074;
extern volatile int data_027e0078;

extern void MIi_CpuCopyFast(const void *src, void *dst, u32 size);
extern void MIi_CpuSend32(const void *src, volatile void *dst, u32 size);

void GX_SendFifoWords(u32 op, const u32 *args, u32 numWords)
{
    NNSG3dGeBuffer *buffer = data_027e0074;
    int busyValue = data_027e0078;
    u32 idx = buffer->idx;

    if (busyValue != 0) {
        if (idx + 1 + numWords <= 192) {
            buffer->data[idx++] = op;
            if (numWords > 0) {
                MIi_CpuCopyFast(args, &data_027e0074->data[idx], numWords << 2);
                idx += numWords;
            }
            data_027e0074->idx = idx;
            return;
        }
        while (data_027e0078 != 0) {
        }
    }

    if (idx != 0) {
        MIi_CpuSend32(&buffer->data[0], (volatile void *)((unsigned int)kh_ds_io + 0x400), idx << 2);
        data_027e0074->idx = 0;
    }kh_ge_port_write1(0x400, (unsigned int)(op));
    MIi_CpuSend32(args, (volatile void *)((unsigned int)kh_ds_io + 0x400), numWords << 2);
}
