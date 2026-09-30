/* PS2: mechanically prepared copy of src/overlays/screens/ov027_game_over/Ov027_PollConfirmAndAdvance.c (ps2/tools/prep_sources.py). Do not edit. */
/* Game over input step: updates the models and screens, draws the sign-in panel in multiplayer, and
 * on A plays the confirm sound and moves to the idle step. */

typedef int (*Ov027Handler)(void);

typedef struct {
    char pad00[0x24];
    int f24;
} Ov027Sub;

typedef struct {
    char pad00[4];
    Ov027Sub *sub;   /* +0x04 */
} Ov027Res;

extern char *NNSi_FndGetCurrentRootHeap(void);
extern void Ov027_UpdateModels(void);
extern void Ov027_StepScreenSwap(void);
extern int *Session_GetSetup(void);
extern void Ov027_DrawSignInPanel(Ov027Sub *p);
extern void Camera_CommitMatrices(void *p);
extern void PlaySound(int bank, int id);
extern int Ov027_GameOverIdle(void);

extern Ov027Res data_ov027_02084360;
extern unsigned short gPadPressed;

Ov027Handler Ov027_PollConfirmAndAdvance(void) {
    char *root = NNSi_FndGetCurrentRootHeap();

    Ov027_UpdateModels();
    Ov027_StepScreenSwap();
    if (*Session_GetSetup() != 1) {
        Ov027_DrawSignInPanel(data_ov027_02084360.sub);
    }
    Camera_CommitMatrices(root + 0x4d8);
    if ((gPadPressed & 1) != 0) {
        PlaySound(0, 1);
        {
            volatile unsigned int *reg_dispcnt = (volatile unsigned int *)((unsigned int)kh_ds_io + 0x0);
            *reg_dispcnt = (*reg_dispcnt & 0xffffe0ff) | 0x100;
        }
        data_ov027_02084360.sub->f24 = 8;
        return Ov027_GameOverIdle;
    }
    return 0;
}
