#include "../TSC_Menu.h"
#include "../../../../../inc/MarlinConfig.h"

#if ENABLED(BTT_UI_SPI)
#ifdef LCD_ENCODER_SUPPORT

#define ENCODER_PULSES_PER_STEP   4
enum {
  LCD_ENCA_INDEX = 0,
  LCD_ENCB_INDEX,
  LCD_BTN_INDEX,
};

int8_t  LCD_encoderDirection = 1;
volatile int8_t  LCD_encoderDiff; // Updated in update_buttons, added to encoderPosition every LCD update
int16_t LCD_encoderPosition = 0;
uint8_t LCD_buttons = 0;
uint8_t LCD_encLastBtn =0;


const uint16_t encPin[]  = {LCD_ENCA_PIN,  LCD_ENCB_PIN,  LCD_BTN_PIN};

void HW_EncoderInit(void)
{
 //MarlinAPI_EncoderInit();
  
  for(uint8_t i = 0; i < COUNT(encPin); i++)
  {
    SET_INPUT_PULLUP(encPin[i]);
  }
  LCD_encLastBtn= encoder_GetPos();
}
bool MarlinAPI_Encoder_ReadStep(uint8_t io_index)
{
  return !READ(encPin[io_index]);
}
bool encoder_ReadStep(uint8_t io_index)
{
  return MarlinAPI_Encoder_ReadStep(io_index);
}
uint8_t encoder_GetPos(void)
{
  uint8_t LCD_newbutton = 0;
  if(encoder_ReadStep(LCD_ENCA_INDEX)) LCD_newbutton |= LCD_EN_A;
  if(encoder_ReadStep(LCD_ENCB_INDEX)) LCD_newbutton |= LCD_EN_B;

  return LCD_newbutton;
}

// #define LCD_encrot0 0
//   #define LCD_encrot1 2
//   #define LCD_encrot2 3
//   #define LCD_encrot3 1
//   #define ENCODER_SPIN(_E1, _E2) switch (LCD_lastEncoderBits) { case _E1: LCD_encoderDiff += LCD_encoderDirection; break; case _E2: LCD_encoderDiff -= LCD_encoderDirection; }
         
void loopCheckEncoderSteps(void)
{
  static uint8_t LCD_lastEncoderBits = 0;
   LCD_buttons = encoder_GetPos();
  #define LCD_encrot0 0
  #define LCD_encrot1 2
  #define LCD_encrot2 3
  #define LCD_encrot3 1
  // Manage encoder rotation
 #define ENCODER_SPIN(_E1, _E2) switch (LCD_lastEncoderBits) { case _E1: LCD_encoderDiff += LCD_encoderDirection; break; case _E2: LCD_encoderDiff -= LCD_encoderDirection; }
         
          #if ENABLED(USE_WATCHDOG)
        HAL_watchdog_refresh();
        #endif

  if (LCD_buttons != LCD_lastEncoderBits)
  {
    switch (LCD_buttons)
    {
      case LCD_encrot0:
       ENCODER_SPIN(LCD_encrot3, LCD_encrot1);
        break;
      case LCD_encrot1: 
        ENCODER_SPIN(LCD_encrot0, LCD_encrot2);
        break;
      case LCD_encrot2:
        ENCODER_SPIN(LCD_encrot1, LCD_encrot3);
         break;
      case LCD_encrot3: 
        ENCODER_SPIN(LCD_encrot2, LCD_encrot0); 
        break;
    }
    LCD_lastEncoderBits = LCD_buttons;
  }

   const float LCD_abs_diff = ABS(LCD_encoderDiff);
   const bool LCD_encoderPastThreshold = (LCD_abs_diff >= (ENCODER_PULSES_PER_STEP));
  if (LCD_encoderPastThreshold)
  {
    LCD_encoderPosition += LCD_encoderDiff / (ENCODER_PULSES_PER_STEP);

    // SERIAL_ECHO("encoderDiff:");
    // SERIAL_ECHO(LCD_encoderPosition);
    // SERIAL_ECHO("\r\n");
    
    LCD_encoderDiff = 0;

  }
}


// read hardware encoder button for select btn press
bool encoder_ReadBtn(uint16_t intervals)
{
  static uint32_t nowTime = 0;

  if(MarlinAPI_Encoder_ReadStep(LCD_BTN_INDEX))
  {
    if(millis() - nowTime > intervals)
    {
      return true;
    }
  }
  else
  {
    nowTime = millis();
  }
  return false;
}


#endif
#endif
