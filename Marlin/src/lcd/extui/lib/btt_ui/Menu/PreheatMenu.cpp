
#include "../../../../../inc/MarlinConfigPre.h"
#if ENABLED(BTT_UI_SPI)
#include "PreheatMenu.h"
#include "../TSC_Menu.h"


MENUITEMS preheatItems = {
// title
LABEL_PREHEAT,
// icon                       label
  {
    {ICON_PREPLA,          LABEL_PREHEAT_PLA},
    {ICON_PREPETG,          LABEL_PREHEAT_PETG},
    {ICON_PREABS,          LABEL_PREHEAT_ABS},
    {ICON_COOLDOWN,          LABEL_COOLDOWN},
    {ICON_BACKGROUND,       LABEL_BACKGROUND},
    {ICON_PBOTH,            LABEL_PREHEAT_BOTH},
    #ifdef UNIFIED_MENU
    {ICON_BACKGROUND,           LABEL_BACKGROUND},
    #else
    {ICON_HEAT,                 LABEL_HEAT},
    #endif
    {ICON_BACK,                 LABEL_BACK},
  }
};

const ITEM itemToolPreheat[] = {
// icon                       label
  {ICON_PBOTH,         LABEL_PREHEAT_BOTH},
  {ICON_BED,                  LABEL_BED},
  {ICON_NOZZLE,               LABEL_NOZZLE},
  {ICON_NOZZLE,               LABEL_NOZZLE},
  {ICON_NOZZLE,               LABEL_NOZZLE},
  {ICON_NOZZLE,               LABEL_NOZZLE},
  {ICON_NOZZLE,               LABEL_NOZZLE},
  {ICON_NOZZLE,               LABEL_NOZZLE},
};   

const uint16_t preheat_bed_temp[] = PREHEAT_BED;
const uint16_t preheat_hotend_temp[] = PREHEAT_HOTEND;

void menuCallBackPreHeat() {
  static TOOLPREHEAT nowHeater = BOTH;
  KEY_VALUES  key_num = menuKeyGetValue();
  switch(key_num)
  {
    case KEY_ICON_0:
    case KEY_ICON_1:
    case KEY_ICON_2:
      switch(nowHeater){
        case BOTH:
          ExtUI::setTargetTemp_celsius(preheat_hotend_temp[key_num],  ExtUI::getActiveTool());
          ExtUI::setTargetTemp_celsius(preheat_bed_temp[key_num],  ExtUI::BED);
          break;
        case BED_PREHEAT:
          ExtUI::setTargetTemp_celsius(preheat_bed_temp[key_num],  ExtUI::BED);
          break;
        case NOZZLE0_PREHEAT:
          ExtUI::setTargetTemp_celsius(preheat_hotend_temp[key_num],  ExtUI::getActiveTool());
          break;
      }
      break;
    case KEY_ICON_3:
      switch(nowHeater){
        case BOTH:
          ExtUI::setTargetTemp_celsius(0,  ExtUI::getActiveTool());
          ExtUI::setTargetTemp_celsius(0,  ExtUI::BED);
          break;
        case BED_PREHEAT:
          ExtUI::setTargetTemp_celsius(0,  ExtUI::BED);
          break;
        case NOZZLE0_PREHEAT:
          ExtUI::setTargetTemp_celsius(0,  ExtUI::getActiveTool());
          break;
      }
      break;
      break;
    case KEY_ICON_5:
      nowHeater = (TOOLPREHEAT)((nowHeater+1) % 3);
      preheatItems.items[key_num] = itemToolPreheat[nowHeater];
      menuDrawItem(&preheatItems.items[key_num], key_num);;
      break;
    
    #ifndef UNIFIED_MENU
    case KEY_ICON_6:
      infoMenu.menu[++infoMenu.cur] = menuHeat;
      break;
    
    #endif
    case KEY_ICON_7:
      infoMenu.cur--; break;
    default:break;
  }
}

#define PREHEAT_PLA_X    BYTE_WIDTH*6+4
#define PREHEAT_PETG_X   BYTE_WIDTH*20+8
#define PREHEAT_ABS_X    BYTE_WIDTH*35+9
#define PREHEAT_T0_Y     BYTE_HEIGHT*6-6
#define PREHEAT_BED_Y    BYTE_HEIGHT*7
#define PREHEAT_BACKCOLOR 0XEE41

void menuDrawPreheatTemp(void)
{
  uint16_t baccolor=GUI_GetBkColor();
  uint16_t color=GUI_GetColor();
  GUI_SetColor(BLACK);
  GUI_SetBkColor(PREHEAT_BACKCOLOR);
   GUI_DispDec(PREHEAT_PLA_X, PREHEAT_T0_Y, preheat_hotend_temp[0], 3, LEFT);
   GUI_DispDec(PREHEAT_PETG_X, PREHEAT_T0_Y, preheat_hotend_temp[1], 3, LEFT);
   GUI_DispDec(PREHEAT_ABS_X, PREHEAT_T0_Y, preheat_hotend_temp[2], 3, LEFT);
   GUI_DispDec(PREHEAT_PLA_X, PREHEAT_BED_Y, preheat_bed_temp[0], 3, LEFT);
   GUI_DispDec(PREHEAT_PETG_X, PREHEAT_BED_Y, preheat_bed_temp[1], 3, LEFT);
   GUI_DispDec(PREHEAT_ABS_X, PREHEAT_BED_Y, preheat_bed_temp[2], 3, LEFT);
  GUI_SetColor(color);
  GUI_SetBkColor(baccolor);
}

void menuPreheat(void)
{
  menuDrawPage(&preheatItems);
  //menuDrawPreheatTemp();
  menuSetFrontCallBack(menuCallBackPreHeat);
}


#endif
