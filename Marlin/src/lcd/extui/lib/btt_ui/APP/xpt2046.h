#ifndef _XPT2046_H_
#define _XPT2046_H_

#include "../../../../../inc/MarlinConfigPre.h"
#if ENABLED(BTT_UI_SPI)
#include "sw_spi.h"
#include "stdbool.h"

void XPT2046_Init(void);
uint8_t   XPT2046_Read_Pen(void);
uint16_t  XPT2046_Repeated_Compare_AD(uint8_t CMD);

uint8_t LCD_ReadTouch(void);
void Touch_Sw(uint8_t num);
bool LCD_BtnTouch(uint8_t intervals);

#endif
#endif
