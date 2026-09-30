/* PS2: mechanically prepared copy of src/overlays/scenes/ov005_mission_result/Ov005_UpdateDialogVisibility.c (ps2/tools/prep_sources.py). Do not edit. */
/* Shows the exit dialog's or the waiting message's sprites for the menu state, and the list cursor
 * when neither is up. */

#include "nitro/types.h"

typedef struct Ov005Context {
    char header[0x54];
    char embeddedManager[0x4a80];
    char opaque4ad4[0x11c];
    int menuState;
    char opaque4bf4[0x4c];
    int scrollbarVisible[12];
    char opaque4c70[0x20];
    int rowItemCounts[2];
} Ov005Context;
extern Ov005Context *data_ov005_0205b80c;
extern const short data_ov005_0205b34c[6],data_ov005_0205b33a[3];
extern int Ov005_FindEntryById(void *,int);
extern void Ov005_SetEntrySlotsVisible(void *,int,int);
extern void Ov005_ReleaseTwoSlots_2(void *,int);
extern void G2x_SetBlendBrightness_(void *,unsigned short,int);
static inline void SetVisible(int id,int visible) {
    int entry=Ov005_FindEntryById(data_ov005_0205b80c->embeddedManager,id);
    Ov005_SetEntrySlotsVisible(data_ov005_0205b80c->embeddedManager,entry,visible);
}
void Ov005_UpdateDialogVisibility(void) {
    u8 i;
    int mode,id;
    u8 j;
    int entry,visibleBase,hiddenBase,visible;
    Ov005Context *context;
    if(data_ov005_0205b80c->menuState==4)mode=1;
    else if(data_ov005_0205b80c->menuState==5)mode=2;
    else mode=0;
    if(mode==1) {
        for(i=0;i<6;i++)SetVisible(data_ov005_0205b34c[i],1);
        for(i=0;i<3;i++)SetVisible(data_ov005_0205b33a[i],0);
    } else if(mode==2) {
        for(i=0;i<6;i++)SetVisible(data_ov005_0205b34c[i],0);
        for(j=0;j<3;j++) {
            id=data_ov005_0205b33a[j];
            SetVisible(id,1);
            entry=Ov005_FindEntryById(data_ov005_0205b80c->embeddedManager,id);
            Ov005_ReleaseTwoSlots_2(data_ov005_0205b80c->embeddedManager,entry);
        }
    } else {
        for(i=0;i<6;i++)SetVisible(data_ov005_0205b34c[i],0);
        for(i=0;i<3;i++)SetVisible(data_ov005_0205b33a[i],0);
    }
    visible=mode==0?1:0;
    entry=Ov005_FindEntryById(data_ov005_0205b80c->embeddedManager,1);
    Ov005_SetEntrySlotsVisible(data_ov005_0205b80c->embeddedManager,entry,visible);
    context=data_ov005_0205b80c;
    if(context->rowItemCounts[0]==0 && context->rowItemCounts[1]==0) {
        entry=Ov005_FindEntryById(context->embeddedManager,29);
        Ov005_SetEntrySlotsVisible(data_ov005_0205b80c->embeddedManager,entry,0);
        SetVisible(1,0);
    }
    if(mode!=0) { hiddenBase=4;visibleBase=30; }
    else { hiddenBase=30;visibleBase=4; }
    for(j=0;j<12;j++) {
        context=data_ov005_0205b80c;
        entry=Ov005_FindEntryById(context->embeddedManager,visibleBase+j);
        Ov005_SetEntrySlotsVisible(data_ov005_0205b80c->embeddedManager,entry,context->scrollbarVisible[j]);
        SetVisible(hiddenBase+j,0);
    }
    if(mode!=0)G2x_SetBlendBrightness_((void *)((unsigned int)kh_ds_io + 0x50),15,-8);
    else G2x_SetBlendBrightness_((void *)((unsigned int)kh_ds_io + 0x50),15,0);
}
