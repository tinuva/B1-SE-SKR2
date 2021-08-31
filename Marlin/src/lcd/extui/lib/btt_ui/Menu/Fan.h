#ifndef _FAN_H_
#define _FAN_H_

#include "../../../../../inc/MarlinConfigPre.h"
#if ENABLED(BTT_UI_SPI)
#include "stdint.h"
#include "stdbool.h"

#define FAN_ID           {"Fan",    "Fan1",    "Fan2",    "Fan3",    "Fan4",    "Fan5"}
extern const char* fanID[];

void menuFan(void);

#endif

#endif

