#ifndef _SPILCDCOM_H
#define _SPILCDCOM_H
#include "../../../../../inc/MarlinConfigPre.h"
#if ENABLED(BTT_UI_SPI)

#include "../SPIHal/share.h"
#define SPI4RWLCD_RS(V)   digitalWrite(SPI4LINLCD_RS,V) //SPI4LCD_RS(V)
#define SPI4RWLCD_CS0(V)  digitalWrite(SPI4LINLCD_CS0,V) //SPI4LCD_CS(V)
#define SPI4RWLCD_SCL(V)  digitalWrite(SPI4LINLCD_SCL,V)
#define SPI4RWLCD_SDA(V)  digitalWrite(SPI4LINLCD_SDA,V)

#define SPILINE_LCD_SendData(n)   SPI_ReadWriteByte(n)

  void SPI4LINE_LCD_INITCOM(void);//IO+init
  void SPI4LINE_LCD_INIT(void);    
  void SPI4LINE_LCD_CLEAR(uint16_t clearcorl);
  void LCD_IO_Init(); 
  void SPILINE_LCD_WriteData(unsigned char i);
  void SPILINE_LCD_WriteComm(unsigned char i);

  void SPI4LINE_LCD_DISPLAY(uint16_t* buffer, uint16_t count);
  void SPILINE_LCD_Write16Data(uint16_t color);
  void PRINT_SPI_TFT_Write16Data(u_int16_t color);
 
#endif
#endif
