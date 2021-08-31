#include "../../../../../inc/MarlinConfig.h"
#include "../TSC_Menu.h"
#if ENABLED(BTT_UI_SPI)

PARAMETERS infoParameters;
const uint8_t parameter_element_count[PARAMETERS_COUNT] = {5, 5, 5, 5, 3, 4, 3, 3, 4, 4, 1, 2, 2, 3, 5};

void setParameter(PARAMETER_NAME name, uint8_t index, float val)
{
  if(index >= parameter_element_count[name]) return;
    switch (name)
    {
    case P_STEPS_PER_MM:
      infoParameters.StepsPerMM[index] = val;
      break;
    case P_CURRENT:
      infoParameters.Current[index] = val;
      break;
    case P_MAX_FEED_RATE:
      infoParameters.MaxFeedRate[index]  = val;
      break;
    case P_MAX_ACCELERATION:
      infoParameters.MaxAcceleration[index] = val;
      break;
    case P_ACCELERATION:
      infoParameters.Acceleration[index] = val;
      break;
    case P_JERK:
      infoParameters.Jerk[index] = val;
      break;
    case P_PROBE_OFFSET:
      infoParameters.ProbeOffset[index] = val;
      break;
    case P_BUMPSENSITIVITY:
      infoParameters.BumpSensitivity[index] = val;
      break;
    case P_FWRETRACT:
      infoParameters.FwRetract[index] = val;
      break;
    case P_FWRECOVER:
      infoParameters.FwRecover[index] = val;
      break;
    case P_AUTO_RETRACT:
      infoParameters.AutoRetract[index] = val;
      break;
    case P_LIN_ADV:
      infoParameters.LinAdvance[index] = val;
      break;
    case P_ABL_STATE:
      infoParameters.ABLState[index] = val;
      break;
    case P_OFFSET_TOOL:
      infoParameters.OffsetTool[index] = val;
      break;
    case P_HYBRID_THRESHOLD:
      infoParameters.HybridThreshold[index] = val;
      break;
    default:
      break;
    }
}

#endif

