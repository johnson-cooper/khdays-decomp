/* PS2: mechanically prepared copy of src/overlays/scenes/ov009_camp_save/data/ov009_msglist_02056108.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov009 .rodata 0x02056108-0x02056114: message-list initialiser. */

/* Message-list set initialiser: the language-templated message archive path
 * ("UI/cm/msl_&.msi.z", '&' = language code), a start index and the list capacity. */
typedef struct MsgListInit {
    const char *pszArchive;  /* 0x00 */
    int nFirst;              /* 0x04 */
    int nCapacity;           /* 0x08 */
} MsgListInit;

extern char gOv009UiCmMslPath;  /* "UI/cm/msl_&.msi.z" */

/* Read by Ov009_UpdateCompletionMilestones. */
const MsgListInit data_ov009_02056108 __attribute__((aligned(__alignof__(MsgListInit)))) = { &gOv009UiCmMslPath, 0, 5 };
