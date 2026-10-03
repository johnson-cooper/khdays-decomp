/* PS2: mechanically prepared copy of libs/nns/g2d/auto/g2d_bg_cnt_table.c (ps2/tools/prep_sources.py). Do not edit. */
/* NitroSystem g2di_BGManipulator.c: NNSiG2dBGCNTTable, the BGnCNT register of each of the eight
 * BGs (main 0-3, sub 0-3) by NNSG2dBGSelect. */

#include "nitro/types.h"
#include "nitro/hw.h"

REGType16v *const data_02041ac0[8] __attribute__((aligned(__alignof__(REGType16v *)))) = {
    (REGType16v *)REG_BG0CNT_ADDR, (REGType16v *)REG_BG1CNT_ADDR, (REGType16v *)REG_BG2CNT_ADDR, (REGType16v *)REG_BG3CNT_ADDR,
    (REGType16v *)REG_DB_BG0CNT_ADDR, (REGType16v *)REG_DB_BG1CNT_ADDR, (REGType16v *)REG_DB_BG2CNT_ADDR, (REGType16v *)REG_DB_BG3CNT_ADDR
};
