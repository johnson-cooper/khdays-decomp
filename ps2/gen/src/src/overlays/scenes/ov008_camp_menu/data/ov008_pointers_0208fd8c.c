/* PS2: mechanically prepared copy of src/overlays/scenes/ov008_camp_menu/data/ov008_pointers_0208fd8c.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov008 .rodata pointer tables, 0x0208fd8c-0x0208fdc8.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

typedef void (*Ov_Fn)(void);

extern void Ov008_MissionEnterPage1(void);
extern void Ov008_MissionStep1NoOp(void);
extern void Ov008_MissionEnterPage2(void);
extern void Ov008_MissionStep3NoOp(void);
extern void Ov008_MissionRebuildLayers(void);
extern void Ov008_MissionInitCells(void);
extern void Ov008_MissionStep6NoOp(void);
extern void Ov008_MissionStep7NoOp(void);
extern void Ov008_MissionStep8SetModeIfFlagged(void);
extern void Ov008_MissionStep9NoOp(void);
extern void Ov008_MissionInitDisplayResources(void);
extern void Ov008_ConfigDispcntBothEngines(void);
extern void Ov008_MissionStep12NoOp(void);
extern void Ov008_MissionShutdownDisplayResources(void);
extern void Ov008_ResetTweensAndBlank(void);

const Ov_Fn data_ov008_0208fd8c[15] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    Ov008_MissionEnterPage1,

    Ov008_MissionStep1NoOp,

    Ov008_MissionEnterPage2,

    Ov008_MissionStep3NoOp,

    Ov008_MissionRebuildLayers,

    Ov008_MissionInitCells,

    Ov008_MissionStep6NoOp,

    Ov008_MissionStep7NoOp,

    Ov008_MissionStep8SetModeIfFlagged,

    Ov008_MissionStep9NoOp,

    Ov008_MissionInitDisplayResources,

    Ov008_ConfigDispcntBothEngines,

    Ov008_MissionStep12NoOp,

    Ov008_MissionShutdownDisplayResources,

    Ov008_ResetTweensAndBlank,

};
