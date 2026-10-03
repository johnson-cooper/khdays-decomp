/* PS2: mechanically prepared copy of src/overlays/screens/ov069/data/ov069_msglist_020ba7ec.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov069 .rodata 0x020ba7ec-0x020ba7f8: message-list initialiser. */

/* Message-list set initialiser: the language-templated message archive path
 * ("UI/cm/msl_&.msi.z", '&' = language code), a start index and the list capacity. */
typedef struct MsgListInit {
    const char *pszArchive;  /* 0x00 */
    int nFirst;              /* 0x04 */
    int nCapacity;           /* 0x08 */
} MsgListInit;

extern char gOv069UiCmMslPath_2;  /* "UI/cm/msl_&.msi.z" */

/* Read by Ov069_TallyMissionRecords. */
const MsgListInit data_ov069_020ba7ec __attribute__((aligned(__alignof__(MsgListInit)))) = { &gOv069UiCmMslPath_2, 0, 4 };
