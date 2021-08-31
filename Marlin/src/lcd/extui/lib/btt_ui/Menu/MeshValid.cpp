#include "../../../../../inc/MarlinConfigPre.h"
#if ENABLED(BTT_UI_SPI)
#include "../TSC_Menu.h"

#define VALID_CLEAR_PREHEAT_TEMP 190
#define VALID_PREHEAT_BED     50

bool pTest = false;

MENUITEMS meshValidItems = {
// title
LABEL_MESH_VALID,
// icon                          label
{
    {ICON_BACKGROUND,              LABEL_BACKGROUND},
    {ICON_BACKGROUND,              LABEL_BACKGROUND},
    {ICON_BACKGROUND,              LABEL_BACKGROUND},
    {ICON_BACKGROUND,              LABEL_BACKGROUND},
    {ICON_BACKGROUND,              LABEL_BACKGROUND},
    {ICON_BACKGROUND,              LABEL_BACKGROUND},
    {ICON_BACKGROUND,              LABEL_BACKGROUND},
    {ICON_BACKGROUND,              LABEL_BACKGROUND},
}
};

void menuCallBackIsMeshValid(void)
{
  uint16_t key_num = KEY_GetValue(2, doubleBtnRect);
  switch(key_num)
  {
    case KEY_POPUP_CONFIRM:
    
    thermalManager.setTargetHotend(0, 0);
    thermalManager.setTargetBed(0);
    infoMenu.cur--;
    infoMenu.cur--;

    break;

    case KEY_POPUP_CANCEL:	

    infoMenu.cur--;
      break;		
  }
}

void menuIsMeshValid(void)
{
    popupDrawPage(bottomDoubleBtn, textSelect(LABEL_WARNING), textSelect(LABEL_STOP_PLANE_TEST), textSelect(LABEL_CONFIRM), textSelect(LABEL_CANNEL));
    menuSetFrontCallBack(menuCallBackIsMeshValid);
}

void menuCallBackMeshValid()
{
    const GUI_RECT menuRect = {0, 0, LCD_WIDTH_PIXEL, LCD_HEIGHT_PIXEL};
    uint16_t key_num = KEY_GetValue(1, &menuRect);

    if (key_num == 0) {
    infoMenu.menu[++infoMenu.cur] = menuIsMeshValid;
    }
    
    if((ExtUI::getActualTemp_celsius(ExtUI::E0)>=(VALID_CLEAR_PREHEAT_TEMP-1))&&(ExtUI::getActualTemp_celsius(ExtUI::BED)>=(VALID_PREHEAT_BED)-1))
    {
     infoMenu.menu[infoMenu.cur] = menuBabyStep;
     pTest = true;
    }
}

void menuMeshValid(void)
{
  menuDrawPage(&meshValidItems);
  GUI_DispStringInRect(0, 0, LCD_WIDTH_PIXEL, LCD_HEIGHT_PIXEL, textSelect(LABEL_WAIT_FOR_TEMP));
  GUI_DispStringInRect(20, LCD_HEIGHT_PIXEL - (BYTE_HEIGHT*2), LCD_WIDTH_PIXEL-20, LCD_HEIGHT_PIXEL, textSelect(LABEL_TOUCH_TO_EXIT));
  thermalManager.setTargetHotend(VALID_CLEAR_PREHEAT_TEMP, 0);
  thermalManager.setTargetBed(VALID_PREHEAT_BED);
  TSC_ReDrawIcon = NULL;
  menuSetFrontCallBack(menuCallBackMeshValid);
}

#endif
