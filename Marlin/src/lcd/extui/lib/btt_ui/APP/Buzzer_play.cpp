#include "../TSC_Menu.h"
#include "../../../../../inc/MarlinConfig.h"

#if ENABLED(BTT_UI_SPI)
#if defined(ARDUINO_ARCH_STM32)
  #ifdef __STM32F1__
    void BuzzerTone(const uint16_t fre,const uint16_t ms)
    {
      uint32_t nowT = millis();
      uint16_t fredT=(uint16_t)(fre/2);
      
      for(;millis()<(nowT+ms);)
      {
        WRITE(BEEPER_PIN, HIGH);
        delay_us(fredT);
        WRITE(BEEPER_PIN, LOW);
        delay_us(fredT);
     }
    }
    #define Buzzer_TurnOn(fre, ms) BuzzerTone(fre,ms)
  #else
    #define Buzzer_TurnOn(fre, ms) BUZZ(ms, fre)
  #endif
#else
 #define Buzzer_TurnOn(fre, ms) BUZZ(ms, fre)
#endif

void Buzzer_play(SOUND sound){
switch (sound)
{
case sound_ok:
  Buzzer_TurnOn(3800,40);
  Buzzer_TurnOn(0,20);
  Buzzer_TurnOn(5500,50);
  break;
case sound_success:

  Buzzer_TurnOn(3500,50);
  Buzzer_TurnOn(0,50);
  Buzzer_TurnOn(3500,50);
  Buzzer_TurnOn(0,50);
  Buzzer_TurnOn(3500,50);
  break;
case sound_cancel:
  Buzzer_TurnOn(5500,50);
  Buzzer_TurnOn(0,20);
  Buzzer_TurnOn(3800,40);
  break;
  case sound_notify:
  Buzzer_TurnOn(3090,50);
  Buzzer_TurnOn(0,50);
  Buzzer_TurnOn(4190,50);
  break;
case sound_error:
   Buzzer_TurnOn(2200,200);
   Buzzer_TurnOn(0,60);
   Buzzer_TurnOn(2200,200);
   Buzzer_TurnOn(0,60);
   Buzzer_TurnOn(2200,200);
  break;
case sound_keypress:
default:
  Buzzer_TurnOn(LCD_FEEDBACK_FREQUENCY_HZ, LCD_FEEDBACK_FREQUENCY_DURATION_MS);
  break;
}
}
#endif
