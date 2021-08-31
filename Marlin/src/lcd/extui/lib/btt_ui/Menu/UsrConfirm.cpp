#include "../../../../../inc/MarlinConfigPre.h"
#if ENABLED(BTT_UI_SPI)
#include "../TSC_Menu.h"

void menuCallUsrConfirm(void)
{
  uint16_t key_num = KEY_GetValue(1, &singleBtnRect);
  switch(key_num)
  {
    case KEY_POPUP_CONFIRM:
      //storeCmd("M1000");
    //   if(!(isPrinting()))
    // {
     ExtUI::setUserConfirmed();
    //}
      infoMenu.cur--;
      break;
  }
}

void menuUsrConfirm()
{
  //GUI_Clear(BK_COLOR);

  menuSetFrontCallBack(menuCallUsrConfirm);
}

#endif

