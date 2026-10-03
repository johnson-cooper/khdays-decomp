/* PS2: mechanically prepared copy of src/engine/data/main_pointers_020423ec.c (ps2/tools/prep_sources.py). Do not edit. */
/* main .data pointer tables, 0x020423ec-0x02042418.
 *
 * 6 tables, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

extern void Gfd_DefaultAllocTexVram(void);
extern void Gfd_DefaultFreeTexVram(void);
extern void Gfd_DefaultAllocPlttVram(void);
extern void Gfd_DefaultFreePlttVram(void);

/* The five party roster entries (src/engine/data/main_party_roster_02042418.c). */
typedef struct PartyRosterEntry { unsigned int w[6]; } PartyRosterEntry;
extern PartyRosterEntry data_02042418[5];

void *data_020423ec[1] __attribute__((aligned(__alignof__(void *)))) = {

    (void *)Gfd_DefaultAllocTexVram,

};

void *data_020423f0[1] __attribute__((aligned(__alignof__(void *)))) = {

    (void *)Gfd_DefaultFreeTexVram,

};

void *data_020423f4[1] __attribute__((aligned(__alignof__(void *)))) = {

    (void *)Gfd_DefaultAllocPlttVram,

};

void *data_020423f8[1] __attribute__((aligned(__alignof__(void *)))) = {

    (void *)Gfd_DefaultFreePlttVram,

};

void *data_020423fc[2] __attribute__((aligned(__alignof__(void *)))) = {

    &data_02042418[0],

    &data_02042418[3],

};

void *data_02042404[5] __attribute__((aligned(__alignof__(void *)))) = {

    &data_02042418[4],

    &data_02042418[3],

    &data_02042418[0],

    &data_02042418[2],

    &data_02042418[1],

};
