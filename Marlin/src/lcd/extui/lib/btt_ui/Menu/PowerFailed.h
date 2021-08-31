#ifndef _POWERFAILED_H_
#define _POWERFAILED_H_

#include "../../../../../inc/MarlinConfigPre.h"
#if ENABLED(BTT_UI_SPI)

#ifdef POWER_LOSS_RECOVERY
void menuPowerFailed(void);

#endif
#endif

#endif
