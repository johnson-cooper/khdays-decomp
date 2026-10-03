/* PS2: mechanically prepared copy of src/overlays/enemies/ov222_enemy_tailbunker_2/data/ov222_strings_020d6c20.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov222 .data strings, 0x020d6c20-0x020d6c78.
 *
 * 8 symbols in one contiguous run. Each array is sized as the original is, so
 * the literal supplies the text and the declared length pads the rest with NUL.
 */

char gOv222PackPathFmt[12] __attribute__((aligned(__alignof__(char)))) = "Ms/%02x.p";

char gOv222BoneHeadName[12] __attribute__((aligned(__alignof__(char)))) = "Bone_head";

char gOv222BoneTail03Name[16] __attribute__((aligned(__alignof__(char)))) = "Bone_tail_03";

char gOv222BonePelvisName[12] __attribute__((aligned(__alignof__(char)))) = "Bone_pelvis";

char gOv222BoneLHandName[12] __attribute__((aligned(__alignof__(char)))) = "Bone_L_hand";

char gOv222BoneRHandName[12] __attribute__((aligned(__alignof__(char)))) = "Bone_R_hand";

char gOv222MoveName[8] __attribute__((aligned(__alignof__(char)))) = "move";

/* rolling sub-item slot handed to each new enemy by the constructor (Ov222_Construct):
 * advances by one per construction and wraps back to 3 at 0x1f; kept word-aligned so the
 * section ends on the counter's word */
struct RollingCounter { unsigned char value; } __attribute__((aligned(4)));
struct RollingCounter data_ov222_020d6c74 __attribute__((aligned(__alignof__(struct RollingCounter)))) = { 3 };
