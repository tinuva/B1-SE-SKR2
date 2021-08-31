#ifndef _SELECTMODE_H_
#define _SELECTMODE_H_

#include "../../../../../inc/MarlinConfigPre.h"
#if ENABLED(BTT_UI_SPI)
#ifdef BTT_DUAL_MODE
typedef enum
{
  MKEY_0 = 0,
  MKEY_1,
  MKEY_IDLE = IDLE_TOUCH,
}MKEY_VALUES;

MKEY_VALUES MKeyGetValue(void);
void menuMode(void);
void loopCheckMode(void);
#endif
#endif
#endif