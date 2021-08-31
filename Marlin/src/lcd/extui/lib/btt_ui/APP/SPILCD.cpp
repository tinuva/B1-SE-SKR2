#include "../../../../../inc/MarlinConfig.h"

#if ENABLED(BTT_UI_SPI)
#include "SPILCD.h"




void SPILINE_LCD_SW_SendData(uint8_t dat)
{
//        unsigned char i;

//    for(i=0; i<8; i++)			
//    {  
//        if((dat&0x80)>>7)
//        {
//           SPI4RWLCD_SDA(HIGH);
//        }
//        else
//        {
//           SPI4RWLCD_SDA(LOW);
//        }
//         dat <<= 1;
//         SPI4RWLCD_SCL(LOW);
//         SPI4RWLCD_SCL(HIGH);
//    }
}
    void SPILINE_LCD_WriteComm(unsigned char i)
    {
      SPI4RWLCD_CS0(LOW);
      SPI4RWLCD_RS(LOW);
      SPILINE_LCD_SendData(i);
      SPI4RWLCD_CS0(HIGH);
    }
    void SPILINE_LCD_WriteData(unsigned char i)
    {
      SPI4RWLCD_CS0(LOW);
      SPI4RWLCD_RS(HIGH);
      SPILINE_LCD_SendData(i);
      SPI4RWLCD_CS0(HIGH);
    }
    void LCD_IO_Init() {
    
        pinMode(SPI4LINLCD_RS,OUTPUT);
        pinMode(SPI4LINLCD_CS0,OUTPUT);
      
        #if ENABLED(SPI4LIN_GRAPHICAL_TFT)
          SPIInitCOM();
          SPI4LINE_LCD_INIT();
        #endif   
    }

    void SPI4LINE_LCD_INIT(void)   
    {   
      // delay(50);
      //ILI9488+CTC3.5
    //************* Start Initial Sequence **********//
      SPILINE_LCD_WriteComm(0xE0); 
      SPILINE_LCD_WriteData(0x00); 
      SPILINE_LCD_WriteData(0x07); 
      SPILINE_LCD_WriteData(0x0f); 
      SPILINE_LCD_WriteData(0x0D); 
      SPILINE_LCD_WriteData(0x1B); 
      SPILINE_LCD_WriteData(0x0A); 
      SPILINE_LCD_WriteData(0x3c); 
      SPILINE_LCD_WriteData(0x78); 
      SPILINE_LCD_WriteData(0x4A); 
      SPILINE_LCD_WriteData(0x07); 
      SPILINE_LCD_WriteData(0x0E); 
      SPILINE_LCD_WriteData(0x09); 
      SPILINE_LCD_WriteData(0x1B); 
      SPILINE_LCD_WriteData(0x1e); 
      SPILINE_LCD_WriteData(0x0f);  
      
      SPILINE_LCD_WriteComm(0xE1); 
      SPILINE_LCD_WriteData(0x00); 
      SPILINE_LCD_WriteData(0x22); 
      SPILINE_LCD_WriteData(0x24); 
      SPILINE_LCD_WriteData(0x06); 
      SPILINE_LCD_WriteData(0x12); 
      SPILINE_LCD_WriteData(0x07); 
      SPILINE_LCD_WriteData(0x36); 
      SPILINE_LCD_WriteData(0x47); 
      SPILINE_LCD_WriteData(0x47); 
      SPILINE_LCD_WriteData(0x06); 
      SPILINE_LCD_WriteData(0x0a); 
      SPILINE_LCD_WriteData(0x07); 
      SPILINE_LCD_WriteData(0x30); 
      SPILINE_LCD_WriteData(0x37); 
      SPILINE_LCD_WriteData(0x0f); 
      
      SPILINE_LCD_WriteComm(0xC0); 
      SPILINE_LCD_WriteData(0x10); 
      SPILINE_LCD_WriteData(0x10); 
      
      SPILINE_LCD_WriteComm(0xC1); 
      SPILINE_LCD_WriteData(0x41); 
      
      SPILINE_LCD_WriteComm(0xC5); 
      SPILINE_LCD_WriteData(0x00); 
      SPILINE_LCD_WriteData(0x22); 
      SPILINE_LCD_WriteData(0x80); 
      
      SPILINE_LCD_WriteComm(0x36); 
      // SPILINE_LCD_WriteData(0x48); 
      SPILINE_LCD_WriteData(0x28); 
      
     
      SPILINE_LCD_WriteComm(0x3A); //Interface Mode Control
      SPILINE_LCD_WriteData(0x55);
     
    
      SPILINE_LCD_WriteComm(0XB0);  //Interface Mode Control  
      SPILINE_LCD_WriteData(0x00); 
      SPILINE_LCD_WriteComm(0xB1);   //Frame rate 70HZ  
      SPILINE_LCD_WriteData(0xB0); 
      SPILINE_LCD_WriteData(0x11); 
      SPILINE_LCD_WriteComm(0xB4); 
      SPILINE_LCD_WriteData(0x02);   
      SPILINE_LCD_WriteComm(0xB6); //RGB/MCU Interface Control
      SPILINE_LCD_WriteData(0x02); 
      SPILINE_LCD_WriteData(0x02); 
      
      SPILINE_LCD_WriteComm(0xB7); 
      SPILINE_LCD_WriteData(0xC6); 
      
      SPILINE_LCD_WriteComm(0xE9); 
      SPILINE_LCD_WriteData(0x00);
      
      SPILINE_LCD_WriteComm(0XF7);    
      SPILINE_LCD_WriteData(0xA9); 
      SPILINE_LCD_WriteData(0x51); 
      SPILINE_LCD_WriteData(0x2C); 
      SPILINE_LCD_WriteData(0x82);
      
      SPILINE_LCD_WriteComm(0x11); 
        delay(120);
      
      SPILINE_LCD_WriteComm(0x29); 
    }
    void SPI4LINE_LCD_INITCOM(void)
    {
       LCD_IO_Init();
       SPI4LINE_LCD_INIT();
    }
    void SPI4LINE_LCD_CLEAR(uint16_t clearcorl)
    { 
     #ifdef SPI4LIN_GRAPHICAL_TFT
      SPI4RWLCD_CS0(LOW);
      SPI4RWLCD_RS(HIGH);
      SPIWriteBuff(clearcorl,LCD_FULL_PIXEL_WIDTH*LCD_FULL_PIXEL_HEIGHT);
      SPI4RWLCD_CS0(HIGH);
     #elif ENABLED(BTT_UI_SPI) 
       
      
     #endif

    }

    void SPILINE_LCD_Write16Data(u_int16_t color)
    {
        SPI4RWLCD_CS0(LOW);
        SPI4RWLCD_RS(HIGH);
        SPI_ReadWriteByte(color>>8);
        SPI_ReadWriteByte(color&0XFF);  
       // SPI_Write2Byte(color);             
        SPI4RWLCD_CS0(HIGH);
    }

    void PRINT_SPI_TFT_Write16Data(u_int16_t color)
    {
        SPI4RWLCD_CS0(LOW);
        SPI4RWLCD_RS(HIGH); 
        SPI_Write2Byte(color);             
        SPI4RWLCD_CS0(HIGH);
    }

  void SPI4LINE_LCD_DISPLAY(uint16_t* buffer, uint16_t count)
    {
      #if defined(ARDUINO_ARCH_STM32)
             SPI4RWLCD_CS0(LOW);
             SPI4RWLCD_RS(HIGH);
             SPIWriteBuff(buffer,(uint32_t)count);
             SPI4RWLCD_CS0(HIGH); 
      #else
             SPI4RWLCD_CS0(LOW);
             SPI4RWLCD_RS(HIGH);
             SPIWriteBuff(buffer,count);
             SPI4RWLCD_CS0(HIGH);        
      #endif
              
    } 

#endif

