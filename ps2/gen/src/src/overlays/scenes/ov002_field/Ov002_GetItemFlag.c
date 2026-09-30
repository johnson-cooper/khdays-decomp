/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/Ov002_GetItemFlag.c (ps2/tools/prep_sources.py). Do not edit. */
/*
 * Ov002_GetItemFlag - the flag that goes with a UI item id.
 *
 * Most ids map to a slot, and the answer is that slot's enable word, reached
 * through the item resource table at +0x1c past its head. The seven ids that do
 * not are the ones standing for a background layer - three on the main engine,
 * all four on the sub engine - and for those the answer is that layer's colour
 * mode bit, read straight out of its control register.
 *
 * Not to be confused with the resource lookup at 02053bb8, which answers with
 * the buffer to draw into; that one owns the GetItemResource name.
 *
 * ARM. Two things are faithful to the ROM rather than to good sense: the answer
 * is returned uninitialised when the id is neither, which is what the original
 * does; and each layer is read into its own local copy of the control register,
 * which is why the frame holds seven halfwords that are never read back.
 *
 * The seven copies are declared in the order that puts them in the ROM's
 * stack slots, which is not the order the switch uses them in: mwcc lays
 * locals out from the top of the frame downwards, so the first declared sits
 * highest. The table entry is reached as base + slot * 4 + 0x1c rather than
 * as an index past the start, which is what keeps the offset in the load.
 */

typedef struct {
    unsigned short nPriority : 2;
    unsigned short nCharBase : 4;
    unsigned short bMosaic : 1;
    unsigned short bColor256 : 1;
    unsigned short nScreenBase : 5;
    unsigned short bExtPalette : 1;
    unsigned short nScreenSize : 2;
} BgControl;

extern int Ov002_ItemIdToSlot(int nItemId);

extern int data_ov002_0207f60c;

unsigned int Ov002_GetItemFlag(int nItemId)
{
    BgControl sMainBg1;
    BgControl sMainBg2;
    BgControl sSubBg1;
    BgControl sSubBg0;
    BgControl sSubBg3;
    BgControl sSubBg2;
    BgControl sMainBg3;
    unsigned int nResult;
    int nSlot;

    nSlot = Ov002_ItemIdToSlot(nItemId);
    if (nSlot == -1) {
        switch (nItemId) {
        case 9:
            sMainBg1 = *(volatile BgControl *)((unsigned int)kh_ds_io + 0xa);
            nResult = sMainBg1.bColor256;
            break;
        case 10:
            sMainBg2 = *(volatile BgControl *)((unsigned int)kh_ds_io + 0xc);
            nResult = sMainBg2.bColor256;
            break;
        case 11:
            sMainBg3 = *(volatile BgControl *)((unsigned int)kh_ds_io + 0xe);
            nResult = sMainBg3.bColor256;
            break;
        case 0x18:
            sSubBg0 = *(volatile BgControl *)((unsigned int)kh_ds_io + 0x1008);
            nResult = sSubBg0.bColor256;
            break;
        case 0x19:
            sSubBg1 = *(volatile BgControl *)((unsigned int)kh_ds_io + 0x100a);
            nResult = sSubBg1.bColor256;
            break;
        case 0x1a:
            sSubBg2 = *(volatile BgControl *)((unsigned int)kh_ds_io + 0x100c);
            nResult = sSubBg2.bColor256;
            break;
        case 0x1b:
            sSubBg3 = *(volatile BgControl *)((unsigned int)kh_ds_io + 0x100e);
            nResult = sSubBg3.bColor256;
            break;
        }
    } else {
        nResult = *(unsigned int *)((char *)*(int *)(*(int **)&data_ov002_0207f60c
                                    + 0x25) + nSlot * 4 + 0x1c);
    }
    return nResult;
}
