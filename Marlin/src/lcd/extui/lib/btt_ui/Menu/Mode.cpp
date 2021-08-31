#include "../../../../../inc/MarlinConfigPre.h"
#if ENABLED(BTT_UI_SPI)
#include "../TSC_Menu.h"
#ifdef BTT_DUAL_MODE
bool freshboot = true;

void infoMenuSelect(void)
{
 #ifdef LOGO_DISPLAY
   if (freshboot)
    {
      LOGO_ReadDisplay();
      freshboot = false;
    }
 #else
  GUI_Clear(BLACK);
 #endif
  infoMenu.cur = 0;
  switch(infoSettings.mode)
  {
    case MARLIN:
     //   card.mount();
    infoMenu.menu[infoMenu.cur] = menuMarlinMode;
      break;

    case SERIAL_TSC:
    {
     //   card.mount();
    GUI_RestoreColorDefault();
    infoMenu.menu[infoMenu.cur] = menuStatus; //status screen as default screen on boot
    }
    break;
  }

}
#endif
#endif
