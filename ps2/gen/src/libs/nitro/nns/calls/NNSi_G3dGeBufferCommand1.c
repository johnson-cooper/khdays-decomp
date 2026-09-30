/* PS2: mechanically prepared copy of libs/nitro/nns/calls/NNSi_G3dGeBufferCommand1.c (ps2/tools/prep_sources.py). Do not edit. */


#include "nitro/types.h"
#include "nnsys/g3d.h"

extern void MIi_CpuSend32(const void *src, volatile void *dst, u32 size);

extern NNSG3dGeCommandBuffer *data_027e0074;
extern volatile u32 data_027e0078;

void NNSi_G3dGeBufferCommand1(u32 command, u32 argument)
{
    NNSG3dGeCommandBuffer *buffer = data_027e0074;
    volatile u32 *busyFlag = &data_027e0078;
    u32 count = buffer->count;

    if (*busyFlag != 0) {
        if (count + 2 <= 0xc0) {
            buffer->words[count] = command;
            buffer->words[count + 1] = argument;
            buffer->count += 2;
            return;
        }

        while (*busyFlag != 0) {
        }
    }

    if (count != 0) {
        MIi_CpuSend32(buffer->words, (volatile void *)((unsigned int)kh_ds_io + 0x400),
                      count << 2);
        buffer->count = 0;
    }kh_ge_port_write1(0x400, (unsigned int)(command));kh_ge_port_write1(0x400, (unsigned int)(argument));
}
