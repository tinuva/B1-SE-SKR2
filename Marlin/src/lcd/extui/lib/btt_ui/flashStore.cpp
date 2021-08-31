#include "TSC_Menu.h"

#include "../../../../inc/MarlinConfigPre.h"
#include "../../../../core/serial.h"
#if ENABLED(BTT_UI_SPI)


extern uint32_t TSC_Para[7];        //
extern SETTINGS infoSettings;  //

uint8_t  BTT_SaveData[PARA_SIZE]; 

void wordToByte(uint32_t word, uint8_t *bytes)  //
{
  uint8_t len = 4;
  uint8_t i = 0;
  for(i = 0; i < len; i++)
  {
    bytes[i] = (word >> 24) & 0xFF;
    word <<= 8;
  }
}

uint32_t byteToWord(uint8_t *bytes, uint8_t len)
{
  uint32_t word = 0;
  uint8_t i = 0;
  for(i = 0; i < len; i++)
  {
    word <<= 8;
    word |= bytes[i];
  }
  return word;
}

// Read settings parameter if exist, or reset settings parameter
// return value: whether the touch screen calibration parameter exists
bool readStoredPara(void)
{
  bool paraExist = true;
  //uint8_t data[PARA_SIZE];
  uint32_t index = 0;
  uint32_t sign = 0;
  //ExtUI::onLoadSettings();
  // storeCmd(PSTR("M503"));
  FlashRead(BTT_SaveData, PARA_SIZE);
 
  sign = byteToWord(BTT_SaveData + (index += 4), 4);
 
  if(sign != TSC_SIGN) 
  {
   paraExist = false;    // If the touch screen calibration parameter does not exist
  }
  else
  {
    for(uint32_t i=0; i<sizeof(TSC_Para)/sizeof(TSC_Para[0]); i++)
    {
      TSC_Para[i] = byteToWord(BTT_SaveData + (index += 4), 4);
    }
  }
  sign = byteToWord(BTT_SaveData + (index += 4), 4);
  if(sign != PARA_SIGN) // If the settings parameter is illegal, reset settings parameter
  {
    infoSettingsReset();
  }
  else
  {
    infoSettings.baudrate     = byteToWord(BTT_SaveData + (index += 4), 4);
    infoSettings.language     = byteToWord(BTT_SaveData + (index += 4), 4);
    infoSettings.mode         = byteToWord(BTT_SaveData + (index += 4), 4);
    infoSettings.runout       = byteToWord(BTT_SaveData + (index += 4), 4);
    infoSettings.rotate_ui    = byteToWord(BTT_SaveData + (index += 4), 4);
    infoSettings.bg_color     = byteToWord(BTT_SaveData + (index += 4), 4);
    infoSettings.font_color   = byteToWord(BTT_SaveData + (index += 4), 4);
    infoSettings.silent       = byteToWord(BTT_SaveData + (index += 4), 4);
    infoSettings.auto_off     = byteToWord(BTT_SaveData + (index += 4), 4);
    infoSettings.power_loss   = byteToWord(BTT_SaveData + (index += 4), 4);
    infoSettings.babystep     = byteToWord(BTT_SaveData + (index += 4), 4);
    infoSettings.hot_homing   = byteToWord(BTT_SaveData + (index += 4), 4);
    infoSettings.file_mode    = byteToWord(BTT_SaveData + (index += 4), 4);
  }
  return paraExist;
}

void storePara(void)
{
  // uint8_t  data[PARA_SIZE]; 
  uint32_t index = 0;
  
  wordToByte(TSC_SIGN, BTT_SaveData + (index += 4));
  for(uint32_t i=0; i<sizeof(TSC_Para)/sizeof(TSC_Para[0]); i++)
  {
    wordToByte(TSC_Para[i], BTT_SaveData + (index += 4));
  }
  wordToByte(PARA_SIGN, BTT_SaveData + (index += 4));
  wordToByte(infoSettings.baudrate,   BTT_SaveData + (index += 4));
  wordToByte(infoSettings.language,   BTT_SaveData + (index += 4));
  wordToByte(infoSettings.mode,       BTT_SaveData + (index += 4));
  wordToByte(infoSettings.runout,     BTT_SaveData + (index += 4));
  wordToByte(infoSettings.rotate_ui,  BTT_SaveData + (index += 4));
  wordToByte(infoSettings.bg_color,   BTT_SaveData + (index += 4));
  wordToByte(infoSettings.font_color, BTT_SaveData + (index += 4));
  wordToByte(infoSettings.silent,     BTT_SaveData + (index += 4));
  wordToByte(infoSettings.auto_off,   BTT_SaveData + (index += 4));
  wordToByte(infoSettings.power_loss, BTT_SaveData + (index += 4));
  wordToByte(infoSettings.babystep,   BTT_SaveData + (index += 4));
  wordToByte(infoSettings.hot_homing, BTT_SaveData + (index += 4)); 
  wordToByte(infoSettings.file_mode,  BTT_SaveData + (index += 4)); 
  FlashWrite(BTT_SaveData, PARA_SIZE);
}

#endif

