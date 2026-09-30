/* Force-included (-include) in front of every decomp source compiled for the PS2.
 *
 * It adapts CodeWarrior-for-DS spellings to the EE gcc without touching the
 * matching sources.  It must stay small and must not hide real portability
 * problems: anything hardware-specific is replaced in ps2/ (see docs/PS2_PORT.md),
 * never papered over here.
 */
#ifndef KH_PS2_DECOMP_PREFIX_H
#define KH_PS2_DECOMP_PREFIX_H

#ifndef PLATFORM_PS2
#define PLATFORM_PS2 1
#endif

#endif
