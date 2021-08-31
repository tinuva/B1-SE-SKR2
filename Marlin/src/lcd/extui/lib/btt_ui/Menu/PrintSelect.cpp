#include "../../../../../inc/MarlinConfigPre.h"
#if ENABLED(BTT_UI_SPI)
#include "../TSC_Menu.h"

#include "../../../../../sd/cardreader.h"
#include "../../../../../feature/powerloss.h"

MENUITEMS printfselectItems = {
//title
LABEL_READY,
//icon
{
  {ICON_BSD_SOURCE,           LABEL_ONBOARDSD},
  {ICON_U_DISK,               LABEL_U_DISK},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},    
  {ICON_BACK,                 LABEL_BACK},
}
};

#define ITEM_FILE_MODE_NUM 2
const ITEM itemFileMode[ITEM_FILE_MODE_NUM] = {
// icon                       label
  {ICON_LISTMODE,           LABEL_LIST_MODE},
  {ICON_ICONMODE,           LABEL_ICON_MODE},
};
const  uint8_t item_FileMode[ITEM_FILE_MODE_NUM] = {0, 1};
static uint8_t item_FileMode_i = 0;

void menuCallBackPrintSelect(void)
{
  static bool lock=false;
  KEY_VALUES key_num = menuKeyGetValue();
  switch (key_num)
  {
  case KEY_ICON_0:
      SD_SUPPORT=1;
      card.remount();
      recovery.check();
      infoMenu.menu[++infoMenu.cur] = menuPrint;
    break;
  case KEY_ICON_1:
      SD_SUPPORT=0;
      card.remount();
      recovery.check();
      infoMenu.menu[++infoMenu.cur] = menuPrint;
    break;
  case KEY_ICON_6:
      item_FileMode_i = (item_FileMode_i + 1) % ITEM_FILE_MODE_NUM;     
      printfselectItems.items[key_num]=itemFileMode[item_FileMode_i];           
      menuDrawItem(&printfselectItems.items[key_num], key_num);
      infoSettings.file_mode = item_FileMode[item_FileMode_i];
      lock=true;
    break;
  case KEY_ICON_7: 
      if(lock)
      {
       infoMenu.menu[++infoMenu.cur]=menuIsSettings;
      }
      else
      {
       infoMenu.cur--;
      }
      lock=false;
     break;  
   default: break;
  }


}

void menuPrintSelct(void)
{
  item_FileMode_i=infoSettings.file_mode;
  printfselectItems.items[6]=itemFileMode[infoSettings.file_mode];

  menuDrawPage(&printfselectItems);
  menuSetFrontCallBack(menuCallBackPrintSelect);
}



#endif