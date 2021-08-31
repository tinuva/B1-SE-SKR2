#ifndef _EXTRUDE_H_
#define _EXTRUDE_H_

#include "../../../../../inc/MarlinConfigPre.h"
#if ENABLED(BTT_UI_SPI)
#define EXTRUDER_ID      {"E0",   "E1",      "E2",      "E3",      "E4",      "E5"}
// Extrude speed mm/min
#define EXTRUDE_SLOW_SPEED      60
#define EXTRUDE_NORMAL_SPEED    600
#define EXTRUDE_FAST_SPEED      1200

void menuExtrude(void);
void  menuColdExtrude(void);
#endif

#endif
