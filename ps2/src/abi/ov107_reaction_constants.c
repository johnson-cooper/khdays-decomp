/* data_ov107_020cb628 - ov107 .rodata 0x020cb628-0x020cb638, as one 16-byte object.
 *
 * On the DS this label starts Ov107_AiState_ApplyHit's two 4-byte local initializers
 * (masks {7, 3, 11, 2}, values {1, 2, 4, 8}) and runs straight into the next table,
 * data_ov107_020cb630 = {0,0,8,9, 0,0,8,4}.  Other ov107 code reads it as an array of 4-byte
 * notification templates {u16 id; u8 flag; u8 arg}:
 *   Ov107_AiState_OnDefeat:  data_ov107_020cb628[2]  -> {0, 8, 9}
 *   func_ov107_020c73a0:     data_ov107_020cb628[3]  -> {0, 8, 4}
 *
 * No C file defined the label, so gen_link.py gave it zero-filled storage.  A template with
 * flag 0 sends the AI's 4-byte stack notification down the full pose-message path
 * (Ov107_AiState_SendPose / Ov107_SendPoseMessage write ~0x24 bytes into it), smashing
 * Ov107_AiState_OnDefeat's stack frame when an enemy dies: the Heartless-battle crashes in the
 * first Marluxia mission (Ov107_AiState_PostTick reading through a garbage pointer, and a jump to
 * address 4 with a heap value in RA).  Defining the real bytes here makes gen_link.py stop
 * stubbing it.
 */

const unsigned char data_ov107_020cb628[16] __attribute__((aligned(4))) = {
    7, 3, 11, 2,        /* ApplyHit masks  */
    1, 2, 4, 8,         /* ApplyHit values */
    0, 0, 8, 9,         /* = data_ov107_020cb630[0..3]: OnDefeat notification */
    0, 0, 8, 4,         /* = data_ov107_020cb630[4..7]: func_ov107_020c73a0 notification */
};
