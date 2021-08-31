#include "../../../../inc/MarlinConfig.h"
#if ENABLED(BTT_UI_SPI)
#include "LCD_Init.h"
#include "APP/SPILCD.h"
#include "Menu/Settings.h"

void LCD_WriteMultiple(uint16_t color, uint32_t count)
{
   #if defined(ARDUINO_ARCH_STM32)
    uint32_t i=0;
      SPI4RWLCD_CS0(LOW);
      SPI4RWLCD_RS(HIGH); 
      for(i=0;i<count;i++)
      {
        SPI_ReadWriteByte(color>>8);
        SPI_ReadWriteByte(color&0x00ff);
      } 
      SPI4RWLCD_CS0(HIGH);
  #else
      //uint32_t i=0;
      SPI4RWLCD_CS0(LOW);
      SPI4RWLCD_RS(HIGH); 
      SPIWriteBuff(color,count);
      SPI4RWLCD_CS0(HIGH);
  #endif

 
}
void LCD_RefreshDirection(void)
{
  SPILINE_LCD_WriteComm(0x36); 
  //SPILINE_LCD_WriteData(infoSettings.rotate_ui ? 0x28 : 0xE8); 
   SPILINE_LCD_WriteData(infoSettings.rotate_ui ? 0xE8 : 0x28); 
}

void LCD_Init(void)
{
  SPI4LINE_LCD_INITCOM();
  LCD_RefreshDirection();
}

#endif









