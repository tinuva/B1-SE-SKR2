#include "../../../../../inc/MarlinConfigPre.h"
#if ENABLED(BTT_UI_SPI)
#include "Settings.h"
#include "../TSC_Menu.h"



SETTINGS infoSettings, // Settings para
         lastSettings; // Last Settings para
extern uint32_t TSC_Para[7]; 
uint32_t lastTSC_Para[7];

// Reset settings data
void infoSettingsReset(void)
{
  infoSettings.baudrate = 115200;
  infoSettings.language = TFT_ENGLISH;
  infoSettings.mode = SERIAL_TSC;
  infoSettings.runout = FIL_RUNOUT_ENABLED_DEFAULT;
  infoSettings.rotate_ui = 0;
  infoSettings.silent = 0;
  infoSettings.auto_off = 0;
  infoSettings.power_loss=PLR_ENABLED_DEFAULT;//Default on
  infoSettings.file_mode=LIST_MODE;
  infoSettings.babystep=0;
  #ifdef Z_HOT_HOMING
  infoSettings.hot_homing=HOT_HOMING_DEFAULT_STATE;
  #else
  infoSettings.hot_homing=0;   
  #endif
}

void loopCheckSettings(void)
{
  static uint32_t lastTime = 0;
  if (millis() - lastTime > 1000)
  {
    lastTime = millis();
    if (memcmp(&infoSettings, &lastSettings, sizeof(SETTINGS))
     || memcmp(lastTSC_Para, TSC_Para, sizeof(TSC_Para)))
    {
      memcpy(&lastSettings, &infoSettings, sizeof(SETTINGS));
      memcpy(lastTSC_Para, TSC_Para, sizeof(TSC_Para));
      storePara();
      //MYSERIAL1.println("Settings stored!");
    }
  }
}

// Version infomation
GUI_RECT lcdFullRect = {0, 0, LCD_WIDTH_PIXEL, LCD_HEIGHT_PIXEL};
void menuCallBackInfo(void)
{
  KEY_VALUES key_num = (KEY_VALUES)KEY_GetValue(1, &lcdFullRect);
  if (key_num != KEY_IDLE)
  {
   infoMenu.cur--;
  }
}

void menuInfo(void)
{
  const char* hardware = "Board   : " BOARD_INFO_NAME;
  const char* firmware = "Firmware: " SHORT_BUILD_VERSION " " __DATE__;
  
  uint16_t HW_X = (LCD_WIDTH_PIXEL - GUI_StrPixelWidth((uint8_t *)hardware))/2;
  uint16_t FW_X = (LCD_WIDTH_PIXEL - GUI_StrPixelWidth((uint8_t *)firmware))/2;
  uint16_t centerY = LCD_HEIGHT_PIXEL/2;
  uint16_t startX = min(HW_X, FW_X);  
  GUI_Clear(BK_COLOR);
  GUI_DispString(startX, centerY - BYTE_HEIGHT, (uint8_t *)hardware);
  GUI_DispString(startX, centerY, (uint8_t *)firmware);
  GUI_DispStringInRect(20, LCD_HEIGHT_PIXEL - (BYTE_HEIGHT*2), LCD_WIDTH_PIXEL-20, LCD_HEIGHT_PIXEL, textSelect(LABEL_TOUCH_TO_EXIT));
  
  TSC_ReDrawIcon = 0; // invalid icon redraw function
  menuSetFrontCallBack(menuCallBackInfo);
}


MENUITEMS settingsItems = {
// title
LABEL_SETTINGS,
// icon                       label
 {{ICON_SCREEN_SETTINGS,      LABEL_SCREEN_SETTINGS},
  {ICON_MACHINE_SETTINGS,     LABEL_MACHINE_SETTINGS},
  {ICON_FEATURE_SETTINGS,     LABEL_DEFAULT},
  {ICON_SCREEN_INFO,          LABEL_SCREEN_INFO},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACK,                 LABEL_BACK},}
};

void menuCallBackIsDefault(void)
{
  uint16_t key_num = KEY_GetValue(2, doubleBtnRect);
  switch(key_num)
  {
    case KEY_POPUP_CONFIRM:
      storeCmd("M502");
      storeCmd("M500");
      infoSettingsReset();
     volumeReminderMessage(LABEL_DEFAULT_OK, STATUS_NORMAL);
     _delay_ms(300);
     infoMenu.cur--;
     //infoMenu.menu[infoMenu.cur] = menuSettings;
      break;

    case KEY_POPUP_CANCEL:	
     infoMenu.cur--;
      break;		
  }
}

void menuIsDefault(void)
{
  popupDrawPage(bottomDoubleBtn, textSelect(LABEL_WARNING), textSelect(LABEL_IS_DEFAULT), textSelect(LABEL_CONFIRM), textSelect(LABEL_CANNEL));
  menuSetFrontCallBack(menuCallBackIsDefault);
}

void menuCallBackSettings(void)
{
  KEY_VALUES key_num = menuKeyGetValue();
  switch(key_num)
  {
    case KEY_ICON_0:
      infoMenu.menu[++infoMenu.cur] = menuScreenSettings;
      break;
    
    case KEY_ICON_1: 
      infoMenu.menu[++infoMenu.cur] = menuMachineSettings;
      break;
    
    case KEY_ICON_2:

      infoMenu.menu[++infoMenu.cur] = menuIsDefault;
     // infoMenu.menu[++infoMenu.cur] = menuFeatureSettings;
        //  storeCmd("M502");
        //  storeCmd("M500");
      break;
    
    case KEY_ICON_3:
      infoMenu.menu[++infoMenu.cur] = menuInfo;
      break;

    case KEY_ICON_7:
      infoMenu.cur--;
      break;
    
    default:
      break;
  }
}

void menuSettings(void)
{
  menuDrawPage(&settingsItems);
  menuSetFrontCallBack(menuCallBackSettings);
}

#endif

