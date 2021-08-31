#include "../../../../../inc/MarlinConfigPre.h"
#if ENABLED(BTT_UI_SPI)
#include "menu.h"
#include "../TSC_Menu.h"
#ifdef BTT_DUAL_MODE

void menuMarlinModeCallBack(void)
{
  MarlinAPI_LCDUpdate();
}


void menuMarlinMode(void)
{
//   GUI_Clear(infoSettings.marlin_mode_bg_color);
//   GUI_SetColor(infoSettings.marlin_mode_font_color);
//   GUI_SetBkColor(infoSettings.marlin_mode_bg_color);
//   if(infoSettings.marlin_mode_showtitle == 1){
//     STRINGS_STORE tempST;
//     W25Qxx_ReadBuffer((uint8_t *)&tempST,STRINGS_STORE_ADDR,sizeof(STRINGS_STORE));
//     GUI_DispStringInRect(0, 0, BTT_LCD_WIDTH, BYTE_HEIGHT, (uint8_t *)tempST.marlin_title);
//   }
  menuSetFrontCallBack(menuMarlinModeCallBack);
  MarlinAPI_LCDInit();
}

#endif
#endif
