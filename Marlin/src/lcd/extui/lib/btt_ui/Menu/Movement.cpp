#include "../../../../../inc/MarlinConfigPre.h"
#if ENABLED(BTT_UI_SPI)
#include "../TSC_Menu.h"
 
MENUITEMS movementItems = {
    //title
    LABEL_MOVEMENT,
    //icon
    {
    {ICON_MOVE,                 LABEL_MOVE},
    {ICON_HOME,                 LABEL_HOME},
    {ICON_BACKGROUND,           LABEL_BACKGROUND},
    {ICON_BACKGROUND,           LABEL_BACKGROUND},
    {ICON_BACKGROUND,           LABEL_BACKGROUND},
    {ICON_BACKGROUND,           LABEL_BACKGROUND},
    {ICON_BACKGROUND,           LABEL_BACKGROUND},
    {ICON_BACK,                 LABEL_BACK},
    }
};

void menuCallBackMovement()
{
 KEY_VALUES key_num = menuKeyGetValue();

  switch (key_num)
  {
  case KEY_ICON_0:
     infoMenu.menu[++infoMenu.cur] = menuMove;
    break;
  case KEY_ICON_1:
     infoMenu.menu[++infoMenu.cur] = menuHome;
   break;
  case KEY_ICON_7: infoMenu.cur--; break;  
     default:break;
  }
}

void menuMovement()
{
menuDrawPage(&movementItems);
menuSetFrontCallBack(menuCallBackMovement);
}


#endif
