/* PS2: mechanically prepared copy of src/overlays/scenes/ov010/Ov010_TitleBootStep.c (ps2/tools/prep_sources.py). Do not edit. */
extern int NNSi_FndGetCurrentRootHeap(void);
extern int Ov010_SetupRootHeapField58(int a, int b, int c);
extern int Ov010_InitFromRootHeapConfig(void);
extern void CallVirtSlot1(int obj, int b);
extern void Text_DrawDirectional(int obj, int b, int c, int d, int e, int f);
extern void Text_UploadTileBuffer(int obj);

/* Boot state machine for the ov010 root-heap work area (state at +0x78, 0..5):
 * configure the field, wait for the config to finish, wait for the A button,
 * reconfigure, then wait again. Always refreshes the +0x18 sub-object; returns
 * -2 once the final state (5) is reached, else 0. */
int Ov010_TitleBootStep(void) {
    int root = NNSi_FndGetCurrentRootHeap();

    switch (*(int *)(root + 0x78)) {
    case 0:
        Ov010_SetupRootHeapField58(0x10000, 0, 0xc8);
        *(int *)(root + 0x78) = 1;
        break;
    case 1:
        if (Ov010_InitFromRootHeapConfig() != 0) {
            *(int *)(root + 0x78) = 2;
        }
        break;
    case 2: {
        volatile unsigned short *reg_keyinput = (volatile unsigned short *)((unsigned int)kh_ds_io + 0x130);
        volatile unsigned short *reg_extkeyin = (volatile unsigned short *)((unsigned int)kh_ds_hiram + 0x1ffa8); /* ARM7 X/Y+hinge */
        unsigned short pressed = (unsigned short)(((*reg_keyinput | *reg_extkeyin) ^ 0x2fff) & 0x2fff);
        if (pressed & 1) {
            *(int *)(root + 0x78) = 3;
        }
        break;
    }
    case 3:
        Ov010_SetupRootHeapField58(0, -0x10000, 0x3e8);
        *(int *)(root + 0x78) = 4;
        break;
    case 4:
        if (Ov010_InitFromRootHeapConfig() != 0) {
            *(int *)(root + 0x78) = 5;
        }
        break;
    }
    CallVirtSlot1(root + 0x18, 0);
    Text_DrawDirectional(root + 0x18, 0x80, 0x60, 1, 0x412, *(int *)(root + 0x74));
    Text_UploadTileBuffer(root + 0x18);
    return *(int *)(root + 0x78) == 5 ? -2 : 0;
}
