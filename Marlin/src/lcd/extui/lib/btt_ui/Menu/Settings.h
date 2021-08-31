#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include "../../../../../inc/MarlinConfigPre.h"
#if ENABLED(BTT_UI_SPI)
#include "stdint.h"

typedef enum
{
  MARLIN=0,
  SERIAL_TSC,
  MODE_COUNT  
}LCD_MODE;

typedef struct
{
  uint32_t baudrate;
  uint8_t  language;
  uint8_t  mode;
  uint8_t  runout;
  uint8_t  rotate_ui;
  uint16_t bg_color;
  uint16_t font_color;
  uint8_t  silent;
  uint8_t  auto_off;
  uint8_t  power_loss;
  uint8_t  hot_homing;
  uint8_t  file_mode;  
  int      babystep;
}SETTINGS;


extern SETTINGS infoSettings;

void infoSettingsReset(void);
void loopCheckSettings(void);
void menuSettings(void);


#endif

#endif
