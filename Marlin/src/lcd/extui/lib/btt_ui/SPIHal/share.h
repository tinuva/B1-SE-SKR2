#ifndef _SHARE_H
#define _SHARE_H
#include "../../../../../inc/MarlinConfig.h"
#if ENABLED(BTT_UI_SPI)||ENABLED(BTT_TFT35_SPIV1_0)

#define LCD_HAL_STM32F407   1
#define LCD_HAL_LPC1768     2
#define LCD_HAL_STM32F103   3

#if defined(ARDUINO_ARCH_STM32)
    #ifdef __STM32F1__
        #define LCD_HAL  LCD_HAL_STM32F103
    #else
        #define LCD_HAL  LCD_HAL_STM32F407
    #endif
#elif  defined(TARGET_LPC1768)
     #define LCD_HAL  LCD_HAL_LPC1768
#endif

void SPI8BITMODE();
void SPI16BITMODE();
void SPI_INIT(void);
uint8_t SPI_ReadWriteByte(uint8_t TxData);
void SPI_Write2Byte(uint16_t TxData);

#if LCD_HAL == LCD_HAL_STM32F407
    void SPI_SetSpeed(uint8_t SpeedSet);
    uint16_t SPI_ReadWrite2Byte(uint16_t TxData);
    void SPIReadBuff(uint8_t* buff,uint32_t len);
    void SPIReadBuff(uint16_t* buff,uint32_t len);
    void SPIWriteBuff(uint8_t* buff,uint32_t len);
    void SPIWriteBuff(uint16_t* buff,uint32_t len);
    void SPIWriteBuff(uint16_t buff,uint32_t len);
    void SPIWriteBuffmarlin(uint16_t buff,uint32_t len);
#elif LCD_HAL == LCD_HAL_STM32F103
    void SPI_SetSpeed(uint8_t SpeedSet);
    uint16_t SPI_ReadWrite2Byte(uint16_t TxData);
    void SPIReadBuff(uint8_t* buff,uint32_t len);
    void SPIWriteBuff(uint8_t* buff,uint32_t len);
    void SPIWriteBuff(uint16_t* buff,uint32_t len);
    void SPIWriteBuff(uint16_t buff,uint32_t len);
    void SPIReadBuff(uint16_t* buff,uint32_t len);
#elif LCD_HAL == LCD_HAL_LPC1768 
   void SPIWriteBuff(uint16_t buff,uint32_t len);
   void SPIWriteBuff(uint16_t* buff,uint32_t len);
   uint16_t SPI_ReadWrite2Byte(uint16_t TxData);
   void SPIReadBuff(uint16_t* buff,uint32_t len,uint16_t mode);
   void SPIReadBuff(uint16_t* buff,uint32_t len);
#endif

#endif
#endif
