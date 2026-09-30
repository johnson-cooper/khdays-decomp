/* PS2 entry point of the game ELF.
 *
 * Brings the platform up (IOP, memory, VFS, pads, GS, audio), then the PS2 implementation of
 * the NitroSDK layer, then enters the decomp's own main loop (kh_game_main, the PS2 adaptation
 * of src/engine/main.c in ps2/overrides/engine/main.c).
 */
#include "platform/kh_platform.h"

extern void kh_nitro_init(void);
extern void kh_game_main(void);

int main(int argc, char **argv)
{
    kh_platform_init(argc, argv);
    kh_nitro_init();
    KH_INFO("boot", "entering game main loop");
    kh_game_main();
    kh_panic("game main loop returned");
}
