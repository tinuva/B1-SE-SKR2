#include "share.h"
#if LCD_HAL == LCD_HAL_STM32F103
#include "../../../../../inc/MarlinConfig.h"
#if ENABLED(BTT_UI_SPI)
	#include "SPI.h"
    SPIClass  F1_SPI(3);


	void SPI_INIT(void)
	{
	  //F1_SPI.setModule(1);
	  F1_SPI.begin();
	  F1_SPI.setClockDivider(SPI_BAUD_PCLK_DIV_2);
	//   F1_SPI.setBitOrder(MSBFIRST);
	//   F1_SPI.setDataMode(SPI_MODE0);
	}

	uint8_t SPI_ReadWriteByte(uint8_t TxData)
	{	//F1_SPI.transfer16
	   return F1_SPI.transfer(TxData);
	}
	uint16_t SPI_ReadWrite2Byte(uint16_t TxData)
	{	//F1_SPI.transfer16
	   return F1_SPI.transfer16(TxData);
	}

    void SPIReadBuff(uint8_t* buff,uint32_t len)
   {
     F1_SPI.dmaTransfer(0, const_cast<uint8_t*>(buff), len);
	// F1_SPI.
   }
    void SPIReadBuff(uint16_t* buff,uint32_t len)
   {
	 F1_SPI.setDataSize(DATA_SIZE_16BIT);
     F1_SPI.dmaTransfer(0, const_cast<uint16_t*>(buff), len);
	 F1_SPI.setDataSize(DATA_SIZE_8BIT);
   }

	void SPIWriteBuff(uint8_t* buff,uint32_t len)
   {
	 //F1_SPI.send(buff, len);
	  F1_SPI.dmaSend(const_cast<uint8_t*>(buff),len,1);
   }
	void SPIWriteBuff(uint16_t* buff,uint32_t len)
	{
	   F1_SPI.setDataSize(DATA_SIZE_16BIT);
	   F1_SPI.dmaSend(const_cast<uint16_t*>(buff),len,1);
	   F1_SPI.setDataSize(DATA_SIZE_8BIT);
	}
	void SPIWriteBuff(uint16_t buff,uint32_t len)
	{
		uint8_t buff1=(uint8_t)(buff>>8);
		uint8_t buff2=(uint8_t)(buff&0X00FF);
		for(uint32_t i=0;i<len;i++)
		{
         F1_SPI.transfer(buff1);
         F1_SPI.transfer(buff2);
		}
	}

void SPI8BITMODE()
{
	  F1_SPI.setDataSize(DATA_SIZE_8BIT);
}

void SPI16BITMODE()
{
      F1_SPI.setDataSize(DATA_SIZE_16BIT);
}

#endif
#endif
