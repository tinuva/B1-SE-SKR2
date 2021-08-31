#ifndef _HEAT_H_
#define _HEAT_H_

#include "../../../../../inc/MarlinConfigPre.h"
#if ENABLED(BTT_UI_SPI)

#include "stdint.h"

#define HEAT_DISPLAY_ID  {"Bed",  "T0",      "T1",      "T2",      "T3",      "T4",      "T5"}
extern const char* const heatDisplayID[];

void heatSetCurHeaterIndex(uint8_t index);
void menuHeat(void);

int16_t getRealTargetTemp(void);
void setTargetTemp(int16_t tmp);
int16_t getTargetTemp(void);
int16_t getCurrentTemp(void);

#endif

#endif
