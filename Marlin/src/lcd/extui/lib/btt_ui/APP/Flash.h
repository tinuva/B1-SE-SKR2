#ifndef _Flash_H
#define _Flash_H

#include "../../../../../inc/MarlinConfig.h"
#if ENABLED(BTT_UI_SPI)
 #include <stdint.h>

#define STM32_FLASH_BASE 0x08000000 

typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;


#define FLASH_Sector_0     ((uint16_t)0x0000) /*!< Sector Number 0   */
#define FLASH_Sector_1     ((uint16_t)0x0008) /*!< Sector Number 1   */
#define FLASH_Sector_2     ((uint16_t)0x0010) /*!< Sector Number 2   */
#define FLASH_Sector_3     ((uint16_t)0x0018) /*!< Sector Number 3   */
#define FLASH_Sector_4     ((uint16_t)0x0020) /*!< Sector Number 4   */
#define FLASH_Sector_5     ((uint16_t)0x0028) /*!< Sector Number 5   */
#define FLASH_Sector_6     ((uint16_t)0x0030) /*!< Sector Number 6   */
#define FLASH_Sector_7     ((uint16_t)0x0038) /*!< Sector Number 7   */
#define FLASH_Sector_8     ((uint16_t)0x0040) /*!< Sector Number 8   */
#define FLASH_Sector_9     ((uint16_t)0x0048) /*!< Sector Number 9   */
#define FLASH_Sector_10    ((uint16_t)0x0050) /*!< Sector Number 10  */
#define FLASH_Sector_11    ((uint16_t)0x0058) /*!< Sector Number 11  */
#define FLASH_Sector_12    ((uint16_t)0x0080) /*!< Sector Number 12  */
#define FLASH_Sector_13    ((uint16_t)0x0088) /*!< Sector Number 13  */
#define FLASH_Sector_14    ((uint16_t)0x0090) /*!< Sector Number 14  */
#define FLASH_Sector_15    ((uint16_t)0x0098) /*!< Sector Number 15  */
#define FLASH_Sector_16    ((uint16_t)0x00A0) /*!< Sector Number 16  */
#define FLASH_Sector_17    ((uint16_t)0x00A8) /*!< Sector Number 17  */
#define FLASH_Sector_18    ((uint16_t)0x00B0) /*!< Sector Number 18  */
#define FLASH_Sector_19    ((uint16_t)0x00B8) /*!< Sector Number 19  */
#define FLASH_Sector_20    ((uint16_t)0x00C0) /*!< Sector Number 20  */
#define FLASH_Sector_21    ((uint16_t)0x00C8) /*!< Sector Number 21  */
#define FLASH_Sector_22    ((uint16_t)0x00D0) /*!< Sector Number 22  */
#define FLASH_Sector_23    ((uint16_t)0x00D8) /*!< Sector Number 23  */



#define ADDR_FLASH_SECTOR_0     ((u32)0x08000000) 	//16 Kbytes  
#define ADDR_FLASH_SECTOR_1     ((u32)0x08004000) 	//16 Kbytes  
#define ADDR_FLASH_SECTOR_2     ((u32)0x08008000) 
#define ADDR_FLASH_SECTOR_3     ((u32)0x0800C000) 	
#define ADDR_FLASH_SECTOR_4     ((u32)0x08010000) 	
#define ADDR_FLASH_SECTOR_5     ((u32)0x08020000) 	
#define ADDR_FLASH_SECTOR_6     ((u32)0x08040000) 	
#define ADDR_FLASH_SECTOR_7     ((u32)0x08060000) 
#define ADDR_FLASH_SECTOR_8     ((u32)0x08080000) 
#define ADDR_FLASH_SECTOR_9     ((u32)0x080A0000) 
#define ADDR_FLASH_SECTOR_10    ((u32)0x080C0000) 
#define ADDR_FLASH_SECTOR_11    ((u32)0x080E0000) 

#define VoltageRange_1        ((uint8_t)0x00)  /*!< Device operating range: 1.8V to 2.1V */
#define VoltageRange_2        ((uint8_t)0x01)  /*!<Device operating range: 2.1V to 2.7V */
#define VoltageRange_3        ((uint8_t)0x02)  /*!<Device operating range: 2.7V to 3.6V */
#define VoltageRange_4        ((uint8_t)0x03)  /*!<Device operating range: 2.7V to 3.6V + External Vpp */

//#define FLASH_FLAG_EOP                 ((uint32_t)0x00000001)  /*!< FLASH End of Operation flag               */
// #define FLASH_FLAG_OPERR               ((uint32_t)0x00000002)  /*!< FLASH operation Error flag                */
// #define FLASH_FLAG_WRPERR              ((uint32_t)0x00000010)  /*!< FLASH Write protected error flag          */
//#define FLASH_FLAG_PGAERR              ((uint32_t)0x00000020)  /*!< FLASH Programming Alignment error flag    */
//#define FLASH_FLAG_PGPERR              ((uint32_t)0x00000040)  /*!< FLASH Programming Parallelism error flag  */
//#define FLASH_FLAG_PGSERR              ((uint32_t)0x00000080)  /*!< FLASH Programming Sequence error flag     */
 #define FLASH_FLAG_RDERR               ((uint32_t)0x00000100)  /*!< Read Protection error flag (PCROP)        */
// #define FLASH_FLAG_BSY                 ((uint32_t)0x00010000)  /*!< FLASH Busy flag                           */ 
// #define IS_FLASH_CLEAR_FLAG(FLAG) ((((FLAG) & (uint32_t)0xFFFFFE0C) == 0x00000000) && ((FLAG) != 0x00000000))

#define SECTOR_MASK                     ((uint32_t)0xFFFFFF07)

typedef enum
{ 
  FLASH_BUSY = 1,
  FLASH_ERROR_RDF,
  FLASH_ERROR_PGSF,
  FLASH_ERROR_PGPF,
  FLASH_ERROR_PGAF,
  FLASH_ERROR_WRPF,
  FLASH_ERROR_PROGRAM,
  FLASH_ERROR_OPERATIONF,
  FLASH_COMPLETE
}FLASH_Status;


#define EEPROM_PAGE_SIZE     (0x10000)     // 2KB, but will use 2x more (4KB)
#define SIGN_ADDRESS (0x8000000UL + (1024 * 1024) - 5 * EEPROM_PAGE_SIZE)


// void STM32_FlashRead(uint8_t *data, uint32_t len);
// void STM32_FlashWrite(uint8_t *data, uint32_t len);

void STMFLASH_Read(u32 ReadAddr,u32 *pBuffer,u32 NumToRead);
void STMFLASH_Write(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite);


 void FlashRead(uint8_t *data, uint32_t len);
 void FlashWrite(uint8_t *data, uint32_t len);

#endif
#endif
