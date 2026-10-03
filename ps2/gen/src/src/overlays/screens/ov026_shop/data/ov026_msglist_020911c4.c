/* PS2: mechanically prepared copy of src/overlays/screens/ov026_shop/data/ov026_msglist_020911c4.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov026 .rodata 0x020911c4-0x020911d0: message-list initialiser. */

/* Message-list set initialiser: the language-templated message archive path
 * ("UI/cm/msl_&.msi.z", '&' = language code), a start index and the list capacity. */
typedef struct MsgListInit {
    const char *pszArchive;  /* 0x00 */
    int nFirst;              /* 0x04 */
    int nCapacity;           /* 0x08 */
} MsgListInit;

extern char gOv026UiCmMslPath;  /* "UI/cm/msl_&.msi.z" */

/* Read by Ov026_RefreshShopUnlockParams. */
const MsgListInit data_ov026_020911c4 __attribute__((aligned(__alignof__(MsgListInit)))) = { &gOv026UiCmMslPath, 0, 5 };
