#pragma once

#include "../../../../inc/MarlinConfigPre.h"
#if ENABLED(BTT_UI_SPI)

#include "../../ui_api.h"

#include "../../../../MarlinCore.h"
#include "../../../../module/temperature.h"
#include "../../../../module/motion.h"
#include "../../../../gcode/queue.h"
#include "../../../../module/planner.h"
#include "../../../../sd/cardreader.h"
#include "../../../../libs/duration_t.h"
#include "../../../../module/printcounter.h"
// Hardware

#include "SPIHal/share.h"
#include "APP/sw_spi.h"
#include "APP/w25qxx.h"
#include "APP/xpt2046.h"
#include "APP/touch_process.h"
#include "APP/Flash.h"
#include "APP/LCD_Encoder.h"
#include "APP/parseACK.h"
#include "APP/MachineParameters.h"
#include "APP/MarlinAPI.h"
#include "APP/interfaceCmd.h"
#include "APP/Datatypechange.h"
#include "APP/Buzzer_play.h"
// API
#include "Language/utf8_decode.h"
#include "Language/Language.h"
#include "LCD_Init.h"
#include "GUI.h"
#include "boot.h"
#include "flashStore.h"

// Menu
#include "Menu/Popup.h"
#include "Menu/PowerFailed.h"
#include "Menu/UsrConfirm.h"
#include "Menu/menu.h"
#include "Menu/StatusScreen.h"
#include "Menu/MainPage.h"
#include "Menu/PreheatMenu.h"
#include "Menu/Heat.h"
#include "Menu/Move.h"
#include "Menu/Home.h"
#include "Menu/Print.h"
#include "Menu/Printing.h"
#include "Menu/Speed.h"
#include "Menu/BabyStep.h"
#include "Menu/More.h"
#include "Menu/Extrude.h"
#include "Menu/Fan.h"
#include "Menu/Settings.h"
#include "Menu/ScreenSettings.h"
#include "Menu/TSCAdjust.h"
#include "Menu/MachineSettings.h"
#include "Menu/FeatureSettings.h"
#include "Menu/leveling.h"
#include "Menu/ProbeOffset.h"
#include "Menu/movement.h"
#include "Menu/SendGcode.h"
#include "Menu/Selectmode.h"
#include "Menu/MarlinMode.h"
#include "Menu/Mode.h"
#include "Menu/MeshEditor.h"
#include "Menu/MeshValid.h"
#include "Menu/LanguageSelect.h"
#include "Menu/PrintSelect.h"

#define MAX_MENU_DEPTH 10       // max sub menu depth
typedef void (*FP_MENU)(void); 

typedef struct
{
  FP_MENU menu[MAX_MENU_DEPTH];  // Menu function buffer
  uint8_t cur;                   // Current menu index in buffer
}MENU;

extern MENU  infoMenu;  // Menu structure

//#define ABS(n) ((n) > 0 ? (n) : -(n))
#define MAXT(a, b) ((a) > (b) ? (a) : (b))
#define MINT(a, b) ((a) > (b) ? (b) : (a))
#define NOBEYOND(min, v, max) MAXT(min, MINT(v, max))



void menuUpdate(void);
void menuSetFrontCallBack(FP_MENU cb);
void LCD_Setup();
bool storeCmd(const char *cmd);
void mustStoreCmd(const char *cmd);
void BuzzerTone(const uint16_t fre,const uint16_t ms);

#endif

