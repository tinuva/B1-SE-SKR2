#include "../../../../../inc/MarlinConfigPre.h"
#if ENABLED(BTT_UI_SPI)
//#if ENABLED(BTT_UI_SPI)||ENABLED(BTT_TFT35_SPIV1_0)
#include "../../../../../HAL/shared/HAL_SPI.h"
#include "../SPIHal/share.h"
#include "w25qxx.h"

/*************************** W25Qxx SPI ***************************/

void W25Qxx_SPI_CS_Set(uint8_t level)
{
  WRITE(W25Qxx_CS_PIN, level);
}

uint8_t W25Qxx_SPI_Read_Write_Byte(uint8_t data)
{
  return SPI_ReadWriteByte(data);
}

void W25Qxx_SPI_Read_Buf(uint8_t* buf, uint16_t nbyte) 
{
   uint16_t i;  
  for(i=0;i<nbyte;i++)
    buf[i]=SPI_ReadWriteByte(0XFF);   	  
}

void W25Qxx_SPI_Write_Buf(uint8_t* pBuffer, uint32_t len)
{
   	uint16_t i;  
    for(i=0;i<len;i++)
    SPI_ReadWriteByte(pBuffer[i]);
}

void W25Qxx_Init(void)
{
  SET_OUTPUT(W25Qxx_CS_PIN);
  W25Qxx_SPI_CS_Set(1);
  SPI_INIT();
}

void W25Qxx_WriteEnable(void)
{
  W25Qxx_SPI_CS_Set(0);
  W25Qxx_SPI_Read_Write_Byte(CMD_WRITE_ENABLE);
  W25Qxx_SPI_CS_Set(1);
}

void W25Qxx_WaitForWriteEnd(void)
{
  uint8_t flashstatus = 0;

  W25Qxx_SPI_CS_Set(0);
  W25Qxx_SPI_Read_Write_Byte(CMD_READ_REGISTER1);
  do
  {
    flashstatus = W25Qxx_SPI_Read_Write_Byte(W25QXX_DUMMY_BYTE);
  }
  while (flashstatus & 0x01); 
  W25Qxx_SPI_CS_Set(1);
}

void W25Qxx_WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
  W25Qxx_WriteEnable();
  W25Qxx_SPI_CS_Set(0);
	
  W25Qxx_SPI_Read_Write_Byte(CMD_PAGE_PROGRAM);
  W25Qxx_SPI_Read_Write_Byte((WriteAddr & 0xFF0000) >> 16);
  W25Qxx_SPI_Read_Write_Byte((WriteAddr & 0xFF00) >> 8);
  W25Qxx_SPI_Read_Write_Byte(WriteAddr & 0xFF);
	
  W25Qxx_SPI_Write_Buf(pBuffer, NumByteToWrite);
  
  W25Qxx_SPI_CS_Set(1);
  W25Qxx_WaitForWriteEnd();
}
void W25Qxx_WriteBuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
  uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

  Addr = WriteAddr % W25QXX_SPI_PAGESIZE;//������һҳ���ĸ���ַ��ʼд����
  count = W25QXX_SPI_PAGESIZE - Addr;//������ҳ������д��������
  NumOfPage =  NumByteToWrite / W25QXX_SPI_PAGESIZE;//����Ҫ��������ݳ��ȿ���д����ҳ
  NumOfSingle = NumByteToWrite % W25QXX_SPI_PAGESIZE;//����������ҳ�����⣬������д�����ݳ���

  if (Addr == 0) /*!< WriteAddr is sFLASH_PAGESIZE aligned  */
  {
     
    if (NumOfPage == 0) /*!< NumByteToWrite < sFLASH_PAGESIZE */
    {
 
      W25Qxx_WritePage(pBuffer, WriteAddr, NumByteToWrite);
    }
    else /*!< NumByteToWrite > sFLASH_PAGESIZE */
    {
       
      while(NumOfPage--)
      {
        W25Qxx_WritePage(pBuffer, WriteAddr, W25QXX_SPI_PAGESIZE);
        WriteAddr +=  W25QXX_SPI_PAGESIZE;
        pBuffer += W25QXX_SPI_PAGESIZE;
      }
      W25Qxx_WritePage(pBuffer, WriteAddr, NumOfSingle);
    }
  }
  else /*!< WriteAddr is not sFLASH_PAGESIZE aligned  */
  {
   //  SERIAL_ECHO("Addr!=0");
    if (NumOfPage == 0) /*!< NumByteToWrite < sFLASH_PAGESIZE */
    {
       //SERIAL_ECHO("NumOfPage=0");
      if (NumOfSingle > count) /*!< (NumByteToWrite + WriteAddr) > sFLASH_PAGESIZE */
      {
      //   SERIAL_ECHO("NumOfSingle > count");
        temp = NumOfSingle - count;

        W25Qxx_WritePage(pBuffer, WriteAddr, count);
        WriteAddr +=  count;
        pBuffer += count;

        W25Qxx_WritePage(pBuffer, WriteAddr, temp);
      }
      else
      {
      //   SERIAL_ECHO("NumOfSingle <= count");
        W25Qxx_WritePage(pBuffer, WriteAddr, NumByteToWrite);
      }
    }
    else /*!< NumByteToWrite > sFLASH_PAGESIZE */
    {
      // SERIAL_ECHO("NumOfPage!=0");
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / W25QXX_SPI_PAGESIZE;
      NumOfSingle = NumByteToWrite % W25QXX_SPI_PAGESIZE;

      W25Qxx_WritePage(pBuffer, WriteAddr, count);
      WriteAddr +=  count;
      pBuffer += count;

      while (NumOfPage--)
      {
        W25Qxx_WritePage(pBuffer, WriteAddr, W25QXX_SPI_PAGESIZE);
        WriteAddr +=  W25QXX_SPI_PAGESIZE;
        pBuffer += W25QXX_SPI_PAGESIZE;
      }

      if (NumOfSingle != 0)
      {
        W25Qxx_WritePage(pBuffer, WriteAddr, NumOfSingle);
      }
    }
  }
}

void W25Qxx_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
  W25Qxx_SPI_CS_Set(0);

  W25Qxx_SPI_Read_Write_Byte(CMD_READ_DATA);

  W25Qxx_SPI_Read_Write_Byte((ReadAddr & 0xFF0000) >> 16);
  W25Qxx_SPI_Read_Write_Byte((ReadAddr& 0xFF00) >> 8);
  W25Qxx_SPI_Read_Write_Byte(ReadAddr & 0xFF);

  W25Qxx_SPI_Read_Buf(pBuffer, NumByteToRead);
	
  W25Qxx_SPI_CS_Set(1);
}

uint32_t W25Qxx_ReadID(void)
{
  uint32_t Temp = 0;

  W25Qxx_SPI_CS_Set(0);
  W25Qxx_SPI_Read_Write_Byte(CMD_READ_ID);

  Temp = W25Qxx_SPI_Read_Write_Byte(W25QXX_DUMMY_BYTE);
  Temp = (Temp<<8) | W25Qxx_SPI_Read_Write_Byte(W25QXX_DUMMY_BYTE);
  Temp = (Temp<<8) | W25Qxx_SPI_Read_Write_Byte(W25QXX_DUMMY_BYTE);

  W25Qxx_SPI_CS_Set(1);

  return Temp;
}

//��������
void W25Qxx_EraseSector(uint32_t SectorAddr)
{
  W25Qxx_WriteEnable();

  W25Qxx_SPI_CS_Set(0);
  W25Qxx_SPI_Read_Write_Byte(CMD_SECTOR_ERASE);
  W25Qxx_SPI_Read_Write_Byte((SectorAddr & 0xFF0000) >> 16);
  W25Qxx_SPI_Read_Write_Byte((SectorAddr & 0xFF00) >> 8);
  W25Qxx_SPI_Read_Write_Byte(SectorAddr & 0xFF);
  W25Qxx_SPI_CS_Set(1);

  W25Qxx_WaitForWriteEnd();
}

//�����
void W25Qxx_EraseBlock(uint32_t BlockAddr)
{
  W25Qxx_WriteEnable();
	
  W25Qxx_SPI_CS_Set(0);
  W25Qxx_SPI_Read_Write_Byte(CMD_BLOCK_ERASE);
  W25Qxx_SPI_Read_Write_Byte((BlockAddr & 0xFF0000) >> 16);
  W25Qxx_SPI_Read_Write_Byte((BlockAddr & 0xFF00) >> 8);
  W25Qxx_SPI_Read_Write_Byte(BlockAddr & 0xFF);
  W25Qxx_SPI_CS_Set(1);

  W25Qxx_WaitForWriteEnd();
}

//ȫƬ����
void W25Qxx_EraseBulk(void)
{
  W25Qxx_WriteEnable();

  W25Qxx_SPI_CS_Set(0);
  W25Qxx_SPI_Read_Write_Byte(CMD_FLASH__BE);
  W25Qxx_SPI_CS_Set(1);

  W25Qxx_WaitForWriteEnd();
}
#endif

