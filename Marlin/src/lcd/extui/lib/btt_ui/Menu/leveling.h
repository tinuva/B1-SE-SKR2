#ifndef _LEVELING_H_
#define _LEVELING_H_

#include "../../../../../inc/MarlinConfigPre.h"
#if ENABLED(BTT_UI_SPI)


void menuManualLeveling(void);
void menuAutoLeveling(void);
void level_setMsg(const uint8_t *title, const uint8_t *msg);

#endif
#endif

