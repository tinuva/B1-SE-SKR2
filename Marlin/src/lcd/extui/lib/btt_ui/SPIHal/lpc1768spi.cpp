#include "share.h"
#if LCD_HAL == LCD_HAL_LPC1768
#include "../../../../../inc/MarlinConfig.h"
#if ENABLED(BTT_UI_SPI)||ENABLED(BTT_TFT35_SPIV1_0)
#include "LPC17xx.h"

void  SPI_INIT(void)//SPI0
{  
   	LPC_SC->PCONP &= (~(1<<8)) & 0xEFEFF7DE;
   	LPC_PINCON->PINSEL0	|= 	0xc0000000;			
    LPC_PINCON->PINSEL1	|=	(0x03 << 2) | (0x03 << 4);
	
	LPC_SC->PCONP |= (1<<8) & 0xEFEFF7DE;			
	LPC_SPI->SPCCR =	0x04;		       			
     
    LPC_SPI->SPCR  = 	(0 << 2) |		
 				(0 << 3) |				// CPHA = 0, Êý¾ÝÔÚSCK µÄµÚÒ»¸öÊ±ÖÓÑØ²ÉÑù
 				(1 << 4) |				// CPOL = 0, SCK ÎªµÍÓÐÐ§
 				(1 << 5) |				// MSTR = 1, SPI ´¦ÓÚÖ÷Ä£Ê½
 				(0 << 6) |				// LSBF = 0, SPI Êý¾Ý´«ÊäMSB (Î»7)ÔÚÏÈ
 				(0 << 7);				// SPIE = 0, SPI ÖÐ¶Ï±»½ûÖ¹	   
}    

void SPI8BITMODE()
{
LPC_SPI->SPCR  = 	(0 << 2) |		
 				(0 << 3) |				// CPHA = 0, Êý¾ÝÔÚSCK µÄµÚÒ»¸öÊ±ÖÓÑØ²ÉÑù
 				(1 << 4) |				// CPOL = 0, SCK ÎªµÍÓÐÐ§
 				(1 << 5) |				// MSTR = 1, SPI ´¦ÓÚÖ÷Ä£Ê½
 				(0 << 6) |				// LSBF = 0, SPI Êý¾Ý´«ÊäMSB (Î»7)ÔÚÏÈ
 				(0 << 7);				// SPIE = 0, SPI ÖÐ¶Ï±»½ûÖ¹	
}

void SPI16BITMODE()
{
LPC_SPI->SPCR  = 	(1 << 2) |		
 				(0 << 3) |				// CPHA = 0, Êý¾ÝÔÚSCK µÄµÚÒ»¸öÊ±ÖÓÑØ²ÉÑù
 				(1 << 4) |				// CPOL = 0, SCK ÎªµÍÓÐÐ§
 				(1 << 5) |				// MSTR = 1, SPI ´¦ÓÚÖ÷Ä£Ê½
 				(0 << 6) |				// LSBF = 0, SPI Êý¾Ý´«ÊäMSB (Î»7)ÔÚÏÈ
 				(0 << 7);				// SPIE = 0, SPI ÖÐ¶Ï±»½ûÖ¹	
}

void SPI_Write2Byte(uint16_t TxData)
{		
	   uint16_t retry=0;	
  		
	  LPC_SPI->SPDR = TxData;							
    while ((LPC_SPI->SPSR & 0x80)==0)			
	{
		retry++;
		if(retry>=0XFE)break; 	
	}			  		
}

uint16_t SPI_ReadWrite2Byte(uint16_t TxData)
{		
	   uint16_t retry=0;	
  		
	  LPC_SPI->SPDR = TxData;							
    while ((LPC_SPI->SPSR & 0x80)==0)			
	{
		retry++;
		if(retry>=0XFE)return 0; 	
	}			  		
    return (uint16_t)(LPC_SPI->SPDR);	
}
void SPIReadBuff(uint16_t* buff,uint32_t len)
{
     uint16_t retry=0;	
 LPC_SPI->SPCR  = (1 << 2)|(0 << 3) |(1 << 4) |(1 << 5) |(0 << 6) |(0 << 7);
   
   for(uint32_t i=0;i<len;i++)
   {
	    LPC_SPI->SPDR = 0xFFFF;	
     while ((LPC_SPI->SPSR & 0x80)==0)			
	  {
		retry++;
		if(retry>=0XFFFE)break; 	
	  }			  		
	*buff=(uint16_t)(LPC_SPI->SPDR);
	    buff++;
   }
   LPC_SPI->SPCR  = (0 << 2)|(0 << 3) |(1 << 4) |(1 << 5) |(0 << 6) |(0 << 7);
}

void SPIReadBuff(uint16_t* buff,uint32_t len,uint16_t mode)
{
     uint16_t retry=0;	
 LPC_SPI->SPCR  = (1 << 2)|(0 << 3) |(1 << 4) |(1 << 5) |(0 << 6) |(0 << 7);
   
   for(uint32_t i=0;i<len;i++)
   {
	    LPC_SPI->SPDR = 0xFFFF;	
	  while ((LPC_SPI->SPSR & 0x80)==0)			
	{
		retry++;
		if(retry>=0XFFFE)break; 	
	}			  		
	*buff=(uint16_t)(LPC_SPI->SPDR)&mode;
	    buff++;
   }
   LPC_SPI->SPCR  = (0 << 2)|(0 << 3) |(1 << 4) |(1 << 5) |(0 << 6) |(0 << 7);
}


void SPIWriteBuff(uint16_t* buff,uint32_t len)
{
     uint16_t retry=0;	
 LPC_SPI->SPCR  = (1 << 2)|(0 << 3) |(1 << 4) |(1 << 5) |(0 << 6) |(0 << 7);
   for(uint32_t i=0;i<len;i++)
   {
	    LPC_SPI->SPDR = *buff;							
	   while ((LPC_SPI->SPSR & 0x80)==0)			
		{
			retry++;
			if(retry>=0XFFFE)break; 	
		}				
		buff++;
   }
   LPC_SPI->SPCR  = (0 << 2)|(0 << 3) |(1 << 4) |(1 << 5) |(0 << 6) |(0 << 7);
}


void SPIWriteBuff(uint16_t buff,uint32_t len)
{
     uint16_t retry=0;	
 LPC_SPI->SPCR  = (1 << 2)|(0 << 3) |(1 << 4) |(1 << 5) |(0 << 6) |(0 << 7);
   for(uint32_t i=0;i<len;i++)
   {
	    LPC_SPI->SPDR = buff;							
	   while ((LPC_SPI->SPSR & 0x80)==0)			
		{
			retry++;
			if(retry>=0XFFFE)break; 	
		}				
   }
   LPC_SPI->SPCR  = (0 << 2)|(0 << 3) |(1 << 4) |(1 << 5) |(0 << 6) |(0 << 7);
}

uint8_t SPI_ReadWriteByte(uint8_t TxData)
{		
	   uint16_t retry=0;	
  		
	  LPC_SPI->SPDR = TxData;							
    while ((LPC_SPI->SPSR & 0x80)==0)			
	{
		retry++;
		if(retry>=0XFE)return 0; 	
	}			  		
    return (uint8_t)(LPC_SPI->SPDR);	
}
#endif
#endif

