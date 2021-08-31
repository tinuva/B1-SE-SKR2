#include "../TSC_Menu.h"
#include "../../../../../inc/MarlinConfig.h"

#if ENABLED(BTT_UI_SPI)

#define LCD_CHANGE_MODE_INTERVALS 1500 //long pressed 1.5s
#define LCD_BUTTON_INTERVALS      20   //20ms

#define LCD_BUTTON_INTERVALS      20   //20ms
#define LCD_EN_A (1<<0)
#define LCD_EN_B (1<<1)
#define LCD_EN_C (1<<2)

extern  int16_t LCD_encoderPosition;

void HW_EncoderInit(void);
bool encoder_ReadBtn(uint16_t intervals);
bool LCD_ReadPen(uint16_t intervals);
bool LCD_BtnTouch(uint16_t intervals);
uint8_t LCD_ReadTouch(void);
void loopCheckEncoderSteps(void);
uint8_t encoder_GetPos(void);
bool encoder_CheckState(void);
void sendEncoder(uint8_t num);

#endif
