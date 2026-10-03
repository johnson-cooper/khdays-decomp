/* PS2: mechanically prepared copy of src/overlays/scenes/ov006_mission_mode_select/data/ov006_mission_tables.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov006 .rodata tail: the Mission Mode member-select tables.
 *
 * Reconstructed source for 0x0205628c-0x020563a4, the last 280 bytes of the overlay's
 * .rodata, verified byte and relocation exact with tools/verify_data.py against the
 * delinked ROM image. The range is contiguous and ends exactly at the section end, so
 * it can own a single .rodata range in delinks.txt.
 *
 *   0205628c  setup params        the six roster ids the unlock bits gate, plus the Q12
 *                                 origin the cursor cells are stacked from
 *   0205629c  scene setup         a 20-byte record the scene constructor copies onto the
 *                                 stack and passes by value to the scene-init call at
 *                                 0x0204d098 together with the context
 *   020562b0  buffer targets      one destination per cell buffer; the flush pass walks
 *                                 the dirty mask and enqueues a 0x600-byte transfer from
 *                                 cellBuffers[i] to targets[i]
 *   020562d0  step handlers       15 entries, one per member-select sub-state
 *   0205630c  screen offsets      19 slots: a row of nine at y=0x40 and a row of ten at
 *                                 y=0x80, both 0x18 apart
 */

#include "nitro/types.h"

typedef struct {
    int x;
    int y;
} MissionOffset;

typedef struct {
    MissionOffset entries[19];
} MissionOffsetTable;

typedef struct {
    s8 ids[6];
    u8 pad_0006[2];
    MissionOffset origin;
} MissionSetupParams;

typedef void (*MissionStepFn)(void);
typedef void (*MissionSceneFn)(void);

typedef struct {
    int limit0;
    int limit1;
    int limit2;
    MissionSceneFn callback0;
    MissionSceneFn callback1;
} MissionSceneSetup;

typedef struct {
    int targets[8];
} MissionBufferTargets;

extern void Ov006_CopyCellBufferRegion(void);
extern void Ov006_ClearCellBufferRegion(void);

extern void Ov006_MissionEnterPage1(void);
extern void Ov006_MissionStep1NoOp(void);
extern void Ov006_MissionEnterPage2(void);
extern void Ov006_MissionStep3NoOp(void);
extern void Ov006_MissionRebuildLayers(void);
extern void Ov006_MissionInitCells(void);
extern void Ov006_MissionStep6NoOp(void);
extern void Ov006_MissionStep7NoOp(void);
extern void Ov006_MissionStep8SetModeIfFlagged(void);
extern void Ov006_MissionStep9NoOp(void);
extern void Ov006_MissionInitDisplayResources(void);
extern void Ov006_ConfigDispcntBothEngines(void);
extern void Ov006_MissionStep12NoOp(void);
extern void Ov006_MissionShutdownDisplayResources(void);
extern void Ov006_ResetTweensAndBlank(void);

const MissionSetupParams data_ov006_0205628c __attribute__((aligned(__alignof__(MissionSetupParams)))) = {
    { 0x11, 0x0a, 0x12, 0x09, 0x08, 0x00 },
    { 0, 0 },
    { 0, 0x20000 },
};

const MissionSceneSetup data_ov006_0205629c __attribute__((aligned(__alignof__(MissionSceneSetup)))) = {
    50,
    50,
    20,
    Ov006_CopyCellBufferRegion,
    Ov006_ClearCellBufferRegion,
};

const MissionBufferTargets data_ov006_020562b0 __attribute__((aligned(__alignof__(MissionBufferTargets)))) = {{
    8, 9, 10, 11,
    24, 25, 26, 27,
}};

const MissionStepFn data_ov006_020562d0[15] __attribute__((aligned(__alignof__(MissionStepFn)))) = {
    Ov006_MissionEnterPage1,
    Ov006_MissionStep1NoOp,
    Ov006_MissionEnterPage2,
    Ov006_MissionStep3NoOp,
    Ov006_MissionRebuildLayers,
    Ov006_MissionInitCells,
    Ov006_MissionStep6NoOp,
    Ov006_MissionStep7NoOp,
    Ov006_MissionStep8SetModeIfFlagged,
    Ov006_MissionStep9NoOp,
    Ov006_MissionInitDisplayResources,
    Ov006_ConfigDispcntBothEngines,
    Ov006_MissionStep12NoOp,
    Ov006_MissionShutdownDisplayResources,
    Ov006_ResetTweensAndBlank,
};

const MissionOffsetTable data_ov006_0205630c __attribute__((aligned(__alignof__(MissionOffsetTable)))) = {{
    { 0x20, 0x40 }, { 0x38, 0x40 }, { 0x50, 0x40 }, { 0x68, 0x40 }, { 0x80, 0x40 },
    { 0x98, 0x40 }, { 0xb0, 0x40 }, { 0xc8, 0x40 }, { 0xe0, 0x40 },
    { 0x14, 0x80 }, { 0x2c, 0x80 }, { 0x44, 0x80 }, { 0x5c, 0x80 }, { 0x74, 0x80 },
    { 0x8c, 0x80 }, { 0xa4, 0x80 }, { 0xbc, 0x80 }, { 0xd4, 0x80 }, { 0xec, 0x80 },
}};
