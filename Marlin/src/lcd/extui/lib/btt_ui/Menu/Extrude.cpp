#include "../../../../../inc/MarlinConfigPre.h"
#if ENABLED(BTT_UI_SPI)
#include "../TSC_Menu.h"
#include "Extrude.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

//1title, ITEM_PER_PAGE items
MENUITEMS extrudeItems = {
// title
LABEL_EXTRUDE,
// icon                       label
 {{ICON_UNLOAD,               LABEL_UNLOAD},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_LOAD,                 LABEL_LOAD},
  {ICON_NOZZLE,               LABEL_NOZZLE},
  {ICON_NORMAL_SPEED,         LABEL_NORMAL_SPEED},
  {ICON_E_5_MM,               LABEL_5_MM},
  {ICON_BACK,                 LABEL_BACK},}
};

#define ITEM_EXTRUDER_NUM EXTRUDERS
static ExtUI::extruder_t item_extruder_i = ExtUI::E0;

#define ITEM_SPEED_NUM 3
const ITEM itemSpeed[ITEM_SPEED_NUM] = {
// icon                       label
  {ICON_SLOW_SPEED,           LABEL_SLOW_SPEED},
  {ICON_NORMAL_SPEED,         LABEL_NORMAL_SPEED},
  {ICON_FAST_SPEED,           LABEL_FAST_SPEED},
};
const  uint32_t item_speed[ITEM_SPEED_NUM] = {EXTRUDE_SLOW_SPEED, EXTRUDE_NORMAL_SPEED, EXTRUDE_FAST_SPEED};
static uint8_t  item_speed_i = 1;

#define ITEM_LEN_NUM 3
const ITEM itemLen[ITEM_LEN_NUM] = {
// icon                       label
  {ICON_E_1_MM,               LABEL_1_MM},
  {ICON_E_5_MM,               LABEL_5_MM},
  {ICON_E_10_MM,              LABEL_10_MM},
};
const  uint8_t item_len[ITEM_LEN_NUM] = {1, 5, 10};
static uint8_t item_len_i = 1;static float eTemp  = 0.0f;
float extrudeCoordinate=0.0f;

void extrudeCoordinateReDraw(void)
{
  const GUI_RECT rect = {exhibitRect.x0, CENTER_Y, exhibitRect.x1, (int16_t)(CENTER_Y+BYTE_HEIGHT)};
  char buf[36]="";
  float  test=extrudeCoordinate;
  int temp;

  temp=(abs)((test-(int)test)*100);
  sprintf(buf, "%d.%d", (int)test,temp);
  GUI_ClearPrect(&rect);
  GUI_DispStringInPrect(&rect, (uint8_t *)buf);
}

const char* extruderDisplayID[] = EXTRUDER_ID;

void showExtrudeCoordinate(void)
{
   const GUI_RECT rect = {exhibitRect.x0, (int16_t)(CENTER_Y-BYTE_HEIGHT), exhibitRect.x1, CENTER_Y};
  // item_extruder_i = ExtUI::getActiveTool();
   SERIAL_ECHO("item_extruder_i-2:");
   SERIAL_ECHO(item_extruder_i);
   SERIAL_ECHO("\r\n");
   extrudeCoordinate = ExtUI::getAxisPosition_mm(item_extruder_i);
   GUI_ClearPrect(&rect);
   GUI_DispStringInPrect(&rect, (uint8_t *)extruderDisplayID[item_extruder_i]);
   extrudeCoordinateReDraw();
}
char BTT_ex_dat[50]="";
void menuCallBackExtrude(void)
{
  KEY_VALUES key_num = menuKeyGetValue();
  switch(key_num)
  {
    case KEY_ICON_0:
      if(thermalManager.degHotend(0)>=EXTRUDE_MINTEMP)
      {
       eTemp -= item_len[item_len_i];
      }
      else
      {
       infoMenu.menu[++infoMenu.cur] = menuColdExtrude;
      }
      break;
    
    case KEY_ICON_3:
      if(thermalManager.degHotend(0)>=EXTRUDE_MINTEMP)
      {
        eTemp += item_len[item_len_i];
      }
      else
      {
        infoMenu.menu[++infoMenu.cur] = menuColdExtrude;
      }

      break;
    
    case KEY_ICON_4:

        #if ITEM_EXTRUDER_NUM >= (2)
          item_extruder_i = (ExtUI::extruder_t)((item_extruder_i + 1) % ITEM_EXTRUDER_NUM);
            // SERIAL_ECHO("item_extruder_i-1:");
            // SERIAL_ECHO(item_extruder_i);
            // SERIAL_ECHO("\r\n");
          showExtrudeCoordinate();
        #else
          if(thermalManager.degTargetHotend(0)<EXTRUDE_MINTEMP)
          {
          thermalManager.setTargetHotend(EXTRUDE_MINTEMP+10, 0);
          }
        #endif

      break;

    case KEY_ICON_5:
      item_speed_i = (item_speed_i+1) % ITEM_SPEED_NUM;
      extrudeItems.items[key_num] = itemSpeed[item_speed_i];
      menuDrawItem(&extrudeItems.items[key_num], key_num);
      break;
    
    case KEY_ICON_6:
      item_len_i = (item_len_i+1) % ITEM_LEN_NUM;
      extrudeItems.items[key_num] = itemLen[item_len_i];
      menuDrawItem(&extrudeItems.items[key_num], key_num);
      break;

    case KEY_ICON_7: 
      infoMenu.cur--; 
      break;
    
    default:

      #ifdef LCD_ENCODER_SUPPORT
        #if ENABLED(USE_WATCHDOG)
        HAL_watchdog_refresh();
        #endif
        if(LCD_encoderPosition)
        {    

          if(LCD_encoderPosition > 0)
          {
            if(thermalManager.degHotend(0)>=EXTRUDE_MINTEMP)
            {
              eTemp += item_len[item_len_i];
            }
            else
            {
            infoMenu.menu[++infoMenu.cur] = menuColdExtrude;
            }
          }
          if(LCD_encoderPosition < 0)
          {
           if(thermalManager.degHotend(0)>=EXTRUDE_MINTEMP)
            {
             eTemp -= item_len[item_len_i];
            }
            else
            {
            infoMenu.menu[++infoMenu.cur] = menuColdExtrude;       
            }
          }
          LCD_encoderPosition = 0;
        }
      #endif
       
      break;
  }

  if(extrudeCoordinate != eTemp)
  {
    extrudeCoordinate = eTemp;
    extrudeCoordinateReDraw();
    ExtUI::setAxisPosition_mm(extrudeCoordinate, item_extruder_i, item_speed[item_speed_i]);
  }
}

void menuExtrude()
{
  menuDrawPage(&extrudeItems);
  showExtrudeCoordinate();
  menuSetFrontCallBack(menuCallBackExtrude);
}



void menuCallBackColdExtrude(void)
{
  uint16_t key_num = KEY_GetValue(2, doubleBtnRect);
  switch(key_num)
  {
    case KEY_POPUP_CONFIRM:
      thermalManager.setTargetHotend(EXTRUDE_MINTEMP+10, 0);
      infoMenu.cur--;
      break;

    case KEY_POPUP_CANCEL:	
     infoMenu.cur--;
      break;		
  }
}

void  menuColdExtrude()
{
  popupDrawPage(bottomDoubleBtn, textSelect(LABEL_WARNING), textSelect(LABEL_COLDEXTRUD), textSelect(LABEL_COLDEXTRUD170), textSelect(LABEL_CANNEL));
  menuSetFrontCallBack(menuCallBackColdExtrude);
}


#endif

