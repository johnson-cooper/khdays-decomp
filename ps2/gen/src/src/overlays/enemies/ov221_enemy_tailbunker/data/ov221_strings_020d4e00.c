/* PS2: mechanically prepared copy of src/overlays/enemies/ov221_enemy_tailbunker/data/ov221_strings_020d4e00.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov221 .data strings, 0x020d4e00-0x020d4e58.
 *
 * 8 symbols in one contiguous run. Each array is sized as the original is, so
 * the literal supplies the text and the declared length pads the rest with NUL.
 */

char gOv221PackPathFmt[12] __attribute__((aligned(__alignof__(char)))) = "Ms/%02x.p";

char gOv221BoneHeadName[12] __attribute__((aligned(__alignof__(char)))) = "Bone_head";

char gOv221BoneTail03Name[16] __attribute__((aligned(__alignof__(char)))) = "Bone_tail_03";

char gOv221BonePelvisName[12] __attribute__((aligned(__alignof__(char)))) = "Bone_pelvis";

char gOv221BoneLHandName[12] __attribute__((aligned(__alignof__(char)))) = "Bone_L_hand";

char gOv221BoneRHandName[12] __attribute__((aligned(__alignof__(char)))) = "Bone_R_hand";

char gOv221MoveName[8] __attribute__((aligned(__alignof__(char)))) = "move";

/* rolling sub-item slot handed to each new enemy by the constructor (Ov221_Construct):
 * advances by one per construction and wraps back to 3 at 0x1f; kept word-aligned so the
 * section ends on the counter's word */
struct RollingCounter { unsigned char value; } __attribute__((aligned(4)));
struct RollingCounter data_ov221_020d4e54 __attribute__((aligned(__alignof__(struct RollingCounter)))) = { 3 };
