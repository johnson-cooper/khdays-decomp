/* PS2 override: keep the original opening subtitle drain, but make a non-progressing
 * stream diagnosable on real hardware instead of spinning forever before MobiClip opens.
 */
#include "platform/kh_platform.h"

extern void kh_debug_stage(const char *stage, int a, int b);
extern void Ov012_MobiClip_UpdateStreamLead(int stream);
extern int  Ov012_IsModeEntryActive(int stream);
extern int  Ov012_MobiClip_PresentRecord(int stream);

void Ov012_MobiClip_DrainStream(int stream, int mode)
{
    int stagnant = 0;

    *(int *)(stream + 0x74) = mode;
    kh_debug_stage("ov012 drain: UpdateStreamLead", *(int *)(stream + 0x6c), *(int *)(stream + 0x64));
    Ov012_MobiClip_UpdateStreamLead(stream);

    while (Ov012_IsModeEntryActive(stream) != 0) {
        int s0 = *(int *)(stream + 0x6c);
        int c0 = *(int *)(stream + 0x64);
        int c1 = *(int *)(stream + 0x68);
        int p0 = *(int *)(stream + 0x58);

        kh_debug_stage("ov012 drain: PresentRecord", s0, c0);
        Ov012_MobiClip_PresentRecord(stream);

        if (s0 == *(int *)(stream + 0x6c) &&
            c0 == *(int *)(stream + 0x64) &&
            c1 == *(int *)(stream + 0x68) &&
            p0 == *(int *)(stream + 0x58)) {
            stagnant++;
            if (stagnant >= 32) {
                kh_panic("Opening pre-movie stream stopped advancing:\n"
                         "state=%d cursor=%d alt=%d pos=%d mode=%p",
                         *(int *)(stream + 0x6c), *(int *)(stream + 0x64),
                         *(int *)(stream + 0x68), *(int *)(stream + 0x58),
                         *(void **)(stream + 0x74));
            }
        } else {
            stagnant = 0;
        }
    }

    *(int *)(stream + 0x74) = 0;
    *(int *)(stream + 0x64) = 0;
    *(int *)(stream + 0x6c) = 0;
    *(int *)(stream + 0x50) = 1;
}
