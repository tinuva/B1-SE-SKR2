#include "../TSC_Menu.h"
#include "../../../../../inc/MarlinConfig.h"

#if ENABLED(BTT_UI_SPI)

#ifndef _MACHINEPARAMETERS_H_
#define _MACHINEPARAMETERS_H_

typedef enum
{
P_STEPS_PER_MM = 0,
P_CURRENT,
P_MAX_FEED_RATE,
P_MAX_ACCELERATION,
P_ACCELERATION,
P_JERK,
P_PROBE_OFFSET,
P_BUMPSENSITIVITY,
P_FWRETRACT,
P_FWRECOVER,
P_AUTO_RETRACT,
P_LIN_ADV,
P_ABL_STATE,
P_OFFSET_TOOL,
P_HYBRID_THRESHOLD,
// Keep below items always at the end
P_SAVE_SETTINGS,
P_RESTORE_SETTINGS,
P_RESET_SETTINGS,
P_ITEMSCOUNT
}PARAMETER_NAME;

#define PARAMETERS_COUNT P_RESET_SETTINGS

typedef enum
{
X_STEPPER = 0,
Y_STEPPER,
Z_STEPPER,
E_STEPPER,
E2_STEPPER,
STEPPER_COUNT
}STEPPERS;

typedef struct
{
float StepsPerMM[STEPPER_COUNT];
float Current[STEPPER_COUNT];
float MaxFeedRate[STEPPER_COUNT];
float MaxAcceleration[STEPPER_COUNT];
float Acceleration[3];
float Jerk[4];
float ProbeOffset[3];
float BumpSensitivity[3];
float FwRetract[4];
float FwRecover[4];
float AutoRetract[1];
float LinAdvance[2];
float ABLState[2];
float OffsetTool[3];
float HybridThreshold[STEPPER_COUNT];
}PARAMETERS;

void setParameter(PARAMETER_NAME name, uint8_t index, float val);

#endif
#endif
