#include "../../../../../inc/MarlinConfig.h"
#include "../TSC_Menu.h"
#if ENABLED(BTT_UI_SPI)

GCODE_QUEUE infoCmd; 

//remove last line from cmd queue
void purgeLastCmd(void)
{
  infoCmd.count--;
  infoCmd.index_r = (infoCmd.index_r + 1) % CMD_MAX_LIST;
}

void sendQueueCmd(void)
{
  if (infoCmd.count == 0)       return;
  if (MarlinAPI_HasInjectCommands()) return;

  MarlinAPI_InjectCommand(infoCmd.queue[infoCmd.index_r].gcode);

  sendGcodeTerminalCache(infoCmd.queue[infoCmd.index_r].gcode, TERMINAL_GCODE);
  purgeLastCmd();

} //sendQueueCmd
#endif
