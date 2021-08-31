#include "../../../../../inc/MarlinConfig.h"
#include "../TSC_Menu.h"
#if ENABLED(BTT_UI_SPI)
/******************************************************************** Parameter ***********************************************************************/

#include "../../../../../module/planner.h"
float MarlinApi_GetParameter(PARAMETER_NAME name, uint8_t index)
{
  //if(index >= parameter_element_count[name]) return 0.0f;
  switch (name)
  {
  case P_STEPS_PER_MM:
    return ExtUI::getAxisSteps_per_mm((ExtUI::axis_t)index);

  case P_CURRENT:
    return (index <= Z_STEPPER) ? ExtUI::getAxisCurrent_mA((ExtUI::axis_t)index) : ExtUI::getAxisCurrent_mA((ExtUI::extruder_t)(index - E_STEPPER));

  case P_MAX_FEED_RATE:
    return ExtUI::getAxisMaxFeedrate_mm_s((ExtUI::axis_t)index);

  case P_MAX_ACCELERATION:
    return ExtUI::getAxisMaxAcceleration_mm_s2((ExtUI::axis_t)index);

  case P_ACCELERATION:
    switch (index)
    {
    case 0: // Printing
      return ExtUI::getPrintingAcceleration_mm_s2();
    case 1: // Retract
      return ExtUI::getRetractAcceleration_mm_s2();
    case 2: // Travel
      return ExtUI::getTravelAcceleration_mm_s2();

    default:
      return 0;
    }

  case P_JERK:
 
      return 0;
   

  case P_PROBE_OFFSET:
    return ExtUI::getProbeOffset_mm((ExtUI::axis_t)index);

  case P_BUMPSENSITIVITY:
    return ExtUI::getTMCBumpSensitivity((ExtUI::axis_t)index);

  case P_FWRETRACT:
  
      return 0;
  

  case P_FWRECOVER:
 
      return 0;


  case P_AUTO_RETRACT:

      return 0;
  

  case P_LIN_ADV:

      return 0;
 

  case P_ABL_STATE:
    switch (index)
    {
    case 0: // M420 S
      return ExtUI::getLevelingActive();;
    case 1: // M420 Z
      return planner.z_fade_height;
    default:
      return 0;
    }

  case P_OFFSET_TOOL:

      return 0;
 

  case P_HYBRID_THRESHOLD:

      return 0;

  default:
    return 0.0f;
  }
}

void MarlinApi_SetParameter(PARAMETER_NAME name, uint8_t index, float val)
{
  switch (name)
  {

  case P_PROBE_OFFSET:
    ExtUI::setProbeOffset_mm(val, (ExtUI::axis_t)index);
    break;

  default:
    break;
  }
}
/******************************************************************** Gcode **********************************************************************/

bool MarlinAPI_HasInjectCommands(void)
{
  return (queue.injected_commands[0] != 0);
}

void MarlinAPI_InjectCommand(char * const gcode)
{
  queue.inject(gcode);
}

/******************************************************************** Marlin mode ***********************************************************************/

#include "../../../../marlinui.h"
#ifdef BTT_DUAL_MODE
void MarlinAPI_LCDInit(void)
{
  ui.marlin_init();
}

void MarlinAPI_LCDUpdate(void)
{
  ui.marlin_update();
}

void MarlinAPI_LCDKill(PGM_P lcd_error, PGM_P lcd_component)
{
  ui.marlin_kill_screen(lcd_error, lcd_component);
}
#endif

#include "../../../../../MarlinCore.h"
#include "../../../../../module/endstops.h"
#include "../../../../../sd/cardreader.h"
void abortSDPrinting();
void finishSDPrinting();
void MarlinAPI_MainLoop(void)
{
  idle();

  #if ENABLED(SDSUPPORT)
    card.autofile_check();
    if (card.flag.abort_sd_printing) abortSDPrinting();
    if (marlin_state == MF_SD_COMPLETE) finishSDPrinting();
  #endif

  queue.advance();

  endstops.event_handler();

  TERN_(HAS_TFT_LVGL_UI, printer_state_polling());
}

#endif
