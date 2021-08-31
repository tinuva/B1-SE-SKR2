#ifndef _MARLIN_API_H_
#define _MARLIN_API_H_

/******************************************************************** Parameter ***********************************************************************/

float MarlinApi_GetParameter(PARAMETER_NAME name, uint8_t index);
void MarlinApi_SetParameter(PARAMETER_NAME name, uint8_t index, float val);
/******************************************************************** Gcode **********************************************************************/
bool MarlinAPI_HasInjectCommands(void);
void MarlinAPI_InjectCommand(char * const gcode);

#ifdef BTT_DUAL_MODE
void MarlinAPI_LCDInit(void);
void MarlinAPI_LCDUpdate(void);
void MarlinAPI_LCDKill(const char *lcd_error, const char *lcd_component);
#endif

void MarlinAPI_MainLoop(void);
#endif
