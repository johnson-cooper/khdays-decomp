/* PS2: mechanically prepared copy of src/engine/Boot_InitScene.c (ps2/tools/prep_sources.py). Do not edit. */
/* BootTask_Construct: constructor of g_BootTaskClassDesc (run by RunClassConstructor).
 * Inits subsystems, then selects the first scene from the word OS_ResetSystem leaves at
 * 0x027ffc20 (HW_RESET_PARAMETER_BUF): 0 (a cold boot) and -2 give scene 1 (logos, title);
 * anything else -- Game_RunSceneLoop resets with the wireless state and bit 31 set -- gives
 * scene 12, the connection-error screen. The scene request
 * is latched by Scene_RequestPending (Scene_RequestPending). Returns the boot task's initial
 * per-frame state fn (Scene_StepAndContinue), stored by the caller into obj+0x14.
 *
 * The 4 bytes that kept this parked were the second branch pair: the ROM emits
 * `beq c ; b d` and every if/goto spelling of the chain gave mwcc's `bne d ; b c`.
 * `beq X ; beq Y ; b Z` is the SWITCH compare-chain shape, not an if-chain shape -- so this
 * wants a switch. The old note rejected the switch for the right reason and the wrong
 * conclusion: mwcc orders a switch's compare chain NUMERICALLY, so with `case 0` and
 * `case -2` on a SIGNED selector it tests -2 first, which is backwards. Read the selector
 * as UNSIGNED and the same two cases become 0 and 0xfffffffe, so 0 is tested first and the
 * chain comes out in the ROM's order. The numeric ordering is not an obstacle -- it is the
 * knob. */

#include "game/engine.h"

#include "game/scene.h"
void *Boot_InitScene(void) {
    unsigned int state;
    FileLoader_Init();
    SoundSys_Start(0);
    FSi_BindCardTransfer(0);
    Scene_ClearPending();
    state = *(unsigned int *)((unsigned int)kh_ds_hiram + 0x1fc20);
    switch (state) {
    case 0:
        Scene_RequestPending(SCENE_TITLE, 0);
        break;
    case 0xfffffffeu:
        Scene_RequestPending(SCENE_TITLE, state);
        break;
    default:
        Scene_RequestPending(SCENE_CONNECTION_ERROR, state);
        break;
    }
    return (void *)Scene_StepAndContinue;
}
