#if defined(ARDUINO_ARCH_STM32)
#include "share.h"
#if LCD_HAL == LCD_HAL_STM32F407
#include "../../../../../inc/MarlinConfig.h"
#if ENABLED(BTT_UI_SPI)
#include "SPI.h"
#include "stm32f4xx_hal_spi.h"
#include "stm32f4xx_hal_dma.h"
#include "pinconfig.h"

#if  MOTHERBOARD == BOARD_BTT_SKR_2
		DMA_HandleTypeDef LCD_TX_DMA,LCD_RX_DMA;

		void SPI_INIT(void)
		{
			__HAL_RCC_GPIOA_IS_CLK_ENABLED();	 
			__HAL_RCC_SPI1_CLK_ENABLE();
			
			pinmap_pinout(PA_7, PinMap_SPI_MOSI);
			pinmap_pinout(PA_6, PinMap_SPI_MISO);
			pinmap_pinout(PA_5, PinMap_SPI_SCLK);

			SPI1->CR1|=0<<10;		
			SPI1->CR1|=1<<9; 	
			SPI1->CR1|=1<<8;  

			SPI1->CR1|=1<<2; 	
		
			SPI1->CR1|=0<<11;	
			SPI1->CR1|=0<<1; 		
			SPI1->CR1|=0<<0; 		
		
			SPI1->CR1|=3<<3; 		
			SPI1->CR1|=0<<7; 		
			SPI1->CR1|=1<<6; 	
			
			SPI_SetSpeed(0);
			SPI_ReadWriteByte(0xff);
		}

 void HAL_InitSPIDmaStream(DMA_HandleTypeDef& hdma, DMA_Stream_TypeDef *inst, uint32_t chan, IRQn_Type irq, uint32_t dir, uint32_t minc) {
    hdma.Instance                 = inst;
    hdma.Init.Channel             = chan;
    hdma.Init.Direction           = dir;
    hdma.Init.PeriphInc           = DMA_PINC_DISABLE;
    hdma.Init.MemInc              = minc;
    hdma.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
    hdma.Init.Mode                = DMA_NORMAL;
    hdma.Init.Priority            = DMA_PRIORITY_MEDIUM;
    hdma.Init.FIFOMode            = DMA_FIFOMODE_ENABLE;
    hdma.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
    hdma.Init.MemBurst            = DMA_MBURST_SINGLE;
    hdma.Init.PeriphBurst         = DMA_PBURST_SINGLE;

    HAL_DMA_DeInit(&hdma);
    HAL_DMA_Init(&hdma);
    HAL_NVIC_EnableIRQ(irq);
  }
        void SPI_DMA_INIT(void)
		{   
		  
		}


		void SPI_SetSpeed(uint8_t SpeedSet)
		{
			SpeedSet&=0X07;		
			SPI1->CR1&=0XFFC7; 
			SPI1->CR1|=SpeedSet<<3;	  
			SPI1->CR1|=1<<6; 		  
		}

		uint8_t SPI_ReadWriteByte(uint8_t TxData)
		{		
			uint16_t retry=0;				 
			while((SPI1->SR&1<<1)==0)		
			{
				retry++;
				if(retry>=0XFFFE)return 0; 	
			}			  
			SPI1->DR=TxData;	 	  	
			retry=0;
			while((SPI1->SR&1<<0)==0) 		
			{
				retry++;
				if(retry>=0XFFFE)return 0;	
			}	  						    
			return SPI1->DR;        
		}


		uint16_t SPI_ReadWrite2Byte(uint16_t TxData)
		{		
			uint16_t retry=0;				 
			while((SPI1->SR&1<<1)==0)		
			{
				retry++;
				if(retry>=0XFFFE)return 0; 	
			}			  
			SPI1->DR=TxData;	 	  	
			retry=0;
			while((SPI1->SR&1<<0)==0) 		
			{
				retry++;
				if(retry>=0XFFFE)return 0;	
			}	  						    
			return SPI1->DR;          		
		}

		void  SPI8BITMODE(void)
		{
		   uint16_t tempreg=0;
			tempreg|=0<<10;			
			tempreg|=1<<9;			
			tempreg|=1<<8;			 
			tempreg|=1<<2;			
			tempreg|=0<<11;			
			tempreg|=1<<1;			
			tempreg|=1<<0;			
			tempreg|=7<<3;			
			tempreg|=0<<7;			
			tempreg|=1<<6;			
			SPI1->CR1=tempreg; 		
			SPI1->I2SCFGR&=~(1<<11);
			SPI_SetSpeed(0);
		}

		void  SPI16BITMODE(void)
		{
		   uint16_t tempreg=0;
			tempreg|=0<<10;			
			tempreg|=1<<9;			
			tempreg|=1<<8;			 
			tempreg|=1<<2;			
			tempreg|=1<<11;			
			tempreg|=1<<1;			
			tempreg|=1<<0;			
			tempreg|=7<<3;			
			tempreg|=0<<7;			
			tempreg|=1<<6;			
			SPI1->CR1=tempreg; 		
			SPI1->I2SCFGR&=~(1<<11);
			SPI_SetSpeed(0);
		}

		void SPIReadBuff(uint8_t* buff,uint32_t len)
		{
			uint16_t retry=0;	
		for(uint32_t i=0;i<len;i++)
		{
				retry=0;				 
					while((SPI1->SR&1<<1)==0)		
					{
						retry++;
						if(retry>=0XFFFE)break; 	
					}			  
					SPI1->DR=0XFF;	 	  	
					retry=0;
					while((SPI1->SR&1<<0)==0) 		
					{
						retry++;
						if(retry>=0XFFFE)break;	
					}	  						    
			*buff=(SPI1->DR);
				buff++;
		}

		}

	    void SPIReadBuff(uint16_t* buff,uint32_t len)
		{
			uint16_t retry=0;	
		for(uint32_t i=0;i<len;i++)
		{
				retry=0;				 
					while((SPI1->SR&1<<1)==0)		
					{
						retry++;
						if(retry>=0XFFFE)break; 	
					}			  
					SPI1->DR=0XFFFF;	 	  	
					retry=0;
					while((SPI1->SR&1<<0)==0) 		
					{
						retry++;
						if(retry>=0XFFFE)break;	
					}	  						    
			*buff=(SPI1->DR);
				buff++;
		}

		}

		void SPIWriteBuff(uint8_t* buff,uint32_t len)
		{
				uint16_t retry=0;
				uint8_t temp=0;	
			for(uint32_t i=0;i<len;i++)
			{				 
						while((SPI1->SR&1<<1)==0)		
						{
							retry++;
							if(retry>=0XFFFE)break; 	
						}			  
						SPI1->DR=*buff;	 	  	
						retry=0;
						while((SPI1->SR&1<<0)==0) 		
						{
							retry++;
							if(retry>=0XFFFE)break;	
						}	  						    
					temp=(SPI1->DR);
					buff++;
			}
		
		}

#elif MOTHERBOARD == BOARD_BTT_GTR_V1_0
		void SPI_INIT(void)
		{

			__HAL_RCC_GPIOB_IS_CLK_ENABLED();	 
			__HAL_RCC_SPI2_CLK_ENABLE();
			
			pinmap_pinout(PB_15, PinMap_SPI_MOSI);
			pinmap_pinout(PB_14, PinMap_SPI_MISO);
			pinmap_pinout(PB_13, PinMap_SPI_SCLK);

			SPI2->CR1|=0<<10;		
			SPI2->CR1|=1<<9; 	
			SPI2->CR1|=1<<8;  

			SPI2->CR1|=1<<2; 	
			SPI2->CR1|=0<<11;	
			SPI2->CR1|=1<<1; 		
			SPI2->CR1|=1<<0; 		
		
			SPI2->CR1|=3<<3; 		
			SPI2->CR1|=0<<7; 		
			SPI2->CR1|=1<<6; 	
			
			SPI_SetSpeed(0);
			SPI_ReadWriteByte(0xff);
		}

		void SPI_SetSpeed(uint8_t SpeedSet)
		{
			SpeedSet&=0X07;		
			SPI2->CR1&=0XFFC7; 
			SPI2->CR1|=SpeedSet<<3;	  
			SPI2->CR1|=1<<6; 		  
		}

		uint8_t SPI_ReadWriteByte(uint8_t TxData)
		{		
			uint16_t retry=0;				 
			while((SPI2->SR&1<<1)==0)		
			{
				retry++;
				if(retry>=0XFFFE)return 0; 	
			}			  
			SPI2->DR=TxData;	 	  	
			retry=0;
			while((SPI2->SR&1<<0)==0) 		
			{
				retry++;
				if(retry>=0XFFFE)return 0;	
			}	  						    
			return SPI2->DR;        
		}


		uint16_t SPI_ReadWrite2Byte(uint16_t TxData)
		{		
			uint16_t retry=0;				 
			while((SPI2->SR&1<<1)==0)		
			{
				retry++;
				if(retry>=0XFFFE)return 0; 	
			}			  
			SPI2->DR=TxData;	 	  	
			retry=0;
			while((SPI2->SR&1<<0)==0) 		
			{
				retry++;
				if(retry>=0XFFFE)return 0;	
			}	  						    
			return SPI2->DR;          		
		}

		void  SPI8BITMODE(void)
		{
		SPI2->CR1|=(0<<10)|(1<<9)|(1<<8)|(1<<2)|(0<<11)|(1<<1)|(1<<0)|(3<<3)|(0<<7)|(1<<6);
		}

		void  SPI16BITMODE(void)
		{
		SPI2->CR1|=(0<<10)|(1<<9)|(1<<8)|(1<<2)|(1<<11)|(1<<1)|(1<<0)|(3<<3)|(0<<7)|(1<<6);		 
		}

		void SPIReadBuff(uint8_t* buff,uint32_t len)
		{
			uint16_t retry=0;	
		for(uint32_t i=0;i<len;i++)
		{
				retry=0;				 
					while((SPI2->SR&1<<1)==0)		
					{
						retry++;
						if(retry>=0XFFFE)break; 	
					}			  
					SPI2->DR=0XFF;	 	  	
					retry=0;
					while((SPI2->SR&1<<0)==0) 		
					{
						retry++;
						if(retry>=0XFFFE)break;	
					}	  						    
			*buff=(SPI2->DR);
				buff++;
		}

		}

		void SPIWriteBuff(uint8_t* buff,uint32_t len)
		{
				uint16_t retry=0;
				uint8_t temp=0;	
		for(uint32_t i=0;i<len;i++)
		{				 
					while((SPI2->SR&1<<1)==0)		
					{
						retry++;
						if(retry>=0XFFFE)break; 	
					}			  
					SPI2->DR=*buff;	 	  	
					retry=0;
					while((SPI2->SR&1<<0)==0) 		
					{
						retry++;
						if(retry>=0XFFFE)break;	
					}	  						    
				temp=(SPI2->DR);
				buff++;
		}
		
		}
#endif

        void SPI_Write2Byte(uint16_t TxData)
		{
			SPI_ReadWriteByte(TxData>>8);
			SPI_ReadWriteByte(TxData&0xFF);
		}
		void SPIWriteBuff(uint16_t* buff,uint32_t len)
		{
			for(uint32_t i=0;i<len;i++)
			{
			// SPI_ReadWriteByte((*buff)>>8);
			// SPI_ReadWriteByte((*buff)&0xFF);
			SPI_ReadWrite2Byte(*buff);
			buff++;
			}
		}
		void SPIWriteBuff(uint16_t buff,uint32_t len)
		{
			for(uint32_t i=0;i<len;i++)
			{
			SPI_ReadWriteByte(buff>>8);
			SPI_ReadWriteByte(buff&0xFF);
			}
		}
        void SPIWriteBuffmarlin(uint16_t buff,uint32_t len)
		{
	      for(uint32_t i=0;i<len;i++)
			{
			SPI_ReadWriteByte(buff>>8);
			SPI_ReadWriteByte(buff&0xFF);
			}
		}
#endif
#endif
#endif

