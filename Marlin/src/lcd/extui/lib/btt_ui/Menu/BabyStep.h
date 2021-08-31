#ifndef _BABYSTEP_H_
#define _BABYSTEP_H_
#include "../../../../../inc/MarlinConfigPre.h"
#if ENABLED(BTT_UI_SPI)

#define BABY_MAX_VALUE  1.00
#define BABY_MIN_VALUE  -1.00

void menuBabyStep(void);
#endif

#endif
