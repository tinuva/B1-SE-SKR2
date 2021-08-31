#include "../../../../../inc/MarlinConfigPre.h"
#if ENABLED(BTT_UI_SPI)

#include "../TSC_Menu.h"
#include "../../../../../feature/powerloss.h"

#ifdef POWER_LOSS_RECOVERY
void menuCallBackPowerFailed(void)
{
  uint16_t key_num = KEY_GetValue(2, doubleBtnRect);
  switch(key_num)
  {
    case KEY_POPUP_CONFIRM:
    
      infoMenu.cur--;
      infoMenu.cur--;
      storeCmd("M1000");
      //recovery.resume();
    
      //infoMenu.cur--;
      //infoMenu.cur--;
       //card.remount();
      break;
    
    case KEY_POPUP_CANCEL: 
      card.remount();
      _delay_ms(100);
      //SERIAL_ECHO("recovery\r\n");
      card.removeJobRecoveryFile();
      infoMenu.cur--;
        //card.remount();
      break;        
  }

  if(!IS_SD_INSERTED())
  {
    infoMenu.cur--;
  }
}

void menuPowerFailed()
{
  //GUI_Clear(BK_COLOR);
 
  popupDrawPage(bottomDoubleBtn, textSelect(LABEL_POWER_FAILED), (uint8_t *)recovery.info.sd_filename, textSelect(LABEL_CONFIRM), textSelect(LABEL_CANNEL));
    
  menuSetFrontCallBack(menuCallBackPowerFailed);

  
}
#endif
#endif

