#ifndef _MACHINE_SETTINGS_H_
#define _MACHINE_SETTINGS_H_

#include "../../../../../inc/MarlinConfigPre.h"
#if ENABLED(BTT_UI_SPI)

#define FILAMENT_RUNOUT_OFF 0
#define FILAMENT_RUNOUT_ON  1

#define POWER_LOSS_RECOVERY_OFF 0
#define POWER_LOSS_RECOVERY_ON  1

#define HOT_HOMING_OFF 0
#define HOT_HOMING_ON  1

void menuRGBSettings(void);
void menuMachineSettings(void);

#endif
#endif
