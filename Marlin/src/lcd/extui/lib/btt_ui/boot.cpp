#include "../../../../inc/MarlinConfig.h"
#if ENABLED(BTT_UI_SPI)
#include "TSC_Menu.h"
#include "boot.h"
#include "../../../../core/macros.h"


const GUI_RECT iconUpdateRect = {(LCD_WIDTH_PIXEL - ICON_WIDTH)/2,              (LCD_HEIGHT_PIXEL - ICON_HEIGHT)/2, 
                                 (LCD_WIDTH_PIXEL - ICON_WIDTH)/2 + ICON_WIDTH, (LCD_HEIGHT_PIXEL - ICON_HEIGHT)/2 + ICON_HEIGHT};
const GUI_RECT miniiconUpdateRect = {(LCD_WIDTH_PIXEL - MINIICON_WIDTH)/2,              (LCD_HEIGHT_PIXEL - MINIICON_HEIGHT)/2-2*MINIICON_HEIGHT, 
                                 (LCD_WIDTH_PIXEL - MINIICON_WIDTH)/2 + MINIICON_WIDTH, (LCD_HEIGHT_PIXEL - MINIICON_HEIGHT)/2 };
const GUI_RECT labelUpdateRect = {0,              (LCD_HEIGHT_PIXEL - ICON_HEIGHT)/2 + ICON_HEIGHT, 
                                 LCD_WIDTH_PIXEL, (LCD_HEIGHT_PIXEL - ICON_HEIGHT)/2 + ICON_HEIGHT + BYTE_HEIGHT};

const char iconBmpName[][32]={
"Heat", "Move", "Home", "Print", "Extrude", "Fan", "Settings", "Leveling", "Inc", "Dec",
"Nozzle", "Hotbed", "Temp_1", "Temp_5", "Temp_10", "Stop", "Back", "Inc_X", "Inc_Y", "Inc_Z",
"Mmm_01", "Mmm_1", "Mmm_10", "Dec_X", "Dec_Y","Dec_Z", "Home_X", "Home_Y", "Home_Z", "Folder",
"File", "Page_up", "Page_d~1", "Pause", "Resume", "Load", "Unload", "Slow", "Normal", "Fast",  
"Emm_1", "Emm_5", "Emm_10", "Full", "Half", "Rotate", "Language", "TP_Adj~1", "ListMo", "About",
"ABLR1", "ABLR2", "ABLR3", "ABLR4", "ABLB1", "BabyStep", "Mmm_001", "SD", "record", "U_Disk",
"RoutON", "RoutOFF", "PowerON", "PowerOFF", "Munlock", "Marlin", "BTT", "Gcode", "BLTouch", "BLTouc~1",
"BLTouc~2", "BLTouc~3", "BLTouchRepeat", "TSCSet~1", "Machin~1", "Featur~1", "ProbeO~1", "EEPROM~1", "SilentOn", "ShutDown",
"RGB_Se~1", "RGB_Red", "RGB_Gr~1", "RGB_Blue", "RGB_Wh~1", "RGB_Off", "PBOTH", "PrePLA", "PrePETG", "PreABS",
"PSON", "PSOFF", "ABLB2", "ABLB3", "ABLB4", "mvalid", "IconMo", "M001", "M100", "Home_Move", "Heat_Fan",
"ManualLevel", "CoolDown", "Silent~1","Status~3","Status~1","Status~2","MainMenu","Status~4","StatusFlow","Flowper", "Speedper", "EM_Stop",
};

const char miniiconBmpName[][32]={
  "Baby","Bed","Fa","Fl","Pro","Sp","T","T0",
};

#define LCD_DMA_MAX_TRANS	65535		// DMA 65535 bytes one frame

#if LCD_HAL == LCD_HAL_STM32F407|| LCD_HAL == LCD_HAL_STM32F103
   uint16_t display[9025];  
#elif LCD_HAL == LCD_HAL_LPC1768 

#endif

void lcd_frame_segment_display(uint16_t size, uint32_t addr)
{  
  #if LCD_HAL == LCD_HAL_STM32F407 || LCD_HAL == LCD_HAL_STM32F103
     
      uint32_t i=0;
      uint16_t color=0;
      for(i=0;i<size;i++)//size
      {
        W25Qxx_SPI_CS_Set(0);                      
        SPI_ReadWriteByte(CMD_READ_DATA);          
        SPI_ReadWriteByte((u8)((addr+i*2)>>16));   
        SPI_ReadWriteByte((u8)((addr+i*2)>>8));   
        SPI_ReadWriteByte((u8)(addr+i*2));  
        SPI16BITMODE(); 
        color=SPI_ReadWrite2Byte(W25QXX_DUMMY_2BYTE);
        W25Qxx_SPI_CS_Set(1); 
        
        SPI4RWLCD_CS0(LOW);
        SPI4RWLCD_RS(HIGH);
        SPI_ReadWrite2Byte(color);
        SPI4RWLCD_CS0(HIGH);
        SPI8BITMODE();
      }  
  #elif LCD_HAL == LCD_HAL_LPC1768 
    uint32_t i=0;
    uint16_t color=0;

      for(i=0;i<size;i++)//size
      {
        W25Qxx_SPI_CS_Set(0);                      
        SPI_ReadWriteByte(CMD_READ_DATA);          
        SPI_ReadWriteByte((u8)((addr+i*2)>>16));   
        SPI_ReadWriteByte((u8)((addr+i*2)>>8));   
        SPI_ReadWriteByte((u8)(addr+i*2));
        SPI16BITMODE();
        color=SPI_ReadWrite2Byte(W25QXX_DUMMY_2BYTE);   
        W25Qxx_SPI_CS_Set(1); 
        
        SPI4RWLCD_CS0(LOW);
        SPI4RWLCD_RS(HIGH);
        SPI_Write2Byte(color);
        SPI4RWLCD_CS0(HIGH);
        SPI8BITMODE();  
       }     
  #endif
  
}

void lcd_frame_display(uint16_t sx, uint16_t sy, uint16_t w, uint16_t h, uint32_t addr)
{
  uint32_t cur=0;
  uint32_t segmentSize;
  uint32_t totalSize = w*h;

  LCD_SetWindow(sx,sy,sx+w-1,sy+h-1);

  for(cur = 0; cur < totalSize; cur += LCD_DMA_MAX_TRANS)
  {
    segmentSize = (cur+LCD_DMA_MAX_TRANS)<=totalSize ? LCD_DMA_MAX_TRANS : totalSize-cur;
    lcd_frame_segment_display(segmentSize, addr+cur*(2));
  }
}

void manage_inactivity(const bool ignore_stepper_queue=false);
void ICOlcd_frame_display(uint16_t sx, uint16_t sy, uint16_t w, uint16_t h, uint32_t addr)
{
  LCD_SetWindow(sx,sy,sx+w-1,sy+h-1);
   
  #if LCD_HAL == LCD_HAL_STM32F407
      W25Qxx_SPI_CS_Set(0);                      
      SPI_ReadWriteByte(CMD_READ_DATA);          
      SPI_ReadWriteByte((u8)(addr>>16));   
      SPI_ReadWriteByte((u8)(addr>>8));   
      SPI_ReadWriteByte((u8)(addr));
      SPI16BITMODE();
      SPIReadBuff(display,9025);
      W25Qxx_SPI_CS_Set(1);
           
      SPI4RWLCD_CS0(LOW);
      SPI4RWLCD_RS(HIGH);
      SPIWriteBuff(display,9025);  
      SPI4RWLCD_CS0(HIGH);
      SPI8BITMODE();
  #elif LCD_HAL == LCD_HAL_STM32F103
      W25Qxx_SPI_CS_Set(0);                      
      SPI_ReadWriteByte(CMD_READ_DATA);          
      SPI_ReadWriteByte((u8)(addr>>16));   
      SPI_ReadWriteByte((u8)(addr>>8));   
      SPI_ReadWriteByte((u8)(addr));
      SPIReadBuff(display,9025);
      W25Qxx_SPI_CS_Set(1);
          
      SPI4RWLCD_CS0(LOW);
      SPI4RWLCD_RS(HIGH);
      SPIWriteBuff(display,9025);  
      SPI4RWLCD_CS0(HIGH);
  #elif LCD_HAL == LCD_HAL_LPC1768 
      uint16_t display[9025];
      W25Qxx_SPI_CS_Set(0);                      
      SPI_ReadWriteByte(CMD_READ_DATA);          
      SPI_ReadWriteByte((u8)(addr>>16));   
      SPI_ReadWriteByte((u8)(addr>>8));   
      SPI_ReadWriteByte((u8)(addr));
      SPIReadBuff(display,size);
      W25Qxx_SPI_CS_Set(1);
      
      SPI4RWLCD_CS0(LOW);
      SPI4RWLCD_RS(HIGH);
      SPIWriteBuff(display,size);  
      SPI4RWLCD_CS0(HIGH);
  #endif
}

void LOGO_ReadDisplay(void)
{
  lcd_frame_display(0, 0, LCD_WIDTH_PIXEL, LCD_HEIGHT_PIXEL, LOGO_ADDR);
}

void ICON_ReadDisplay(uint16_t sx, uint16_t sy, uint8_t icon)
{
  ICOlcd_frame_display(sx, sy, ICON_WIDTH, ICON_HEIGHT, ICON_ADDR(icon));
}

void MINIICON_ReadDisplay(uint16_t sx, uint16_t sy, uint8_t icon)
{ 
#if LCD_HAL == LCD_HAL_STM32F407
      LCD_SetWindow(sx,sy,sx+MINIICON_WIDTH-1,sy+MINIICON_HEIGHT-1);
      W25Qxx_SPI_CS_Set(0);                      
      SPI_ReadWriteByte(CMD_READ_DATA);          
      SPI_ReadWriteByte((u8)(MINI_ICON_ADDR(icon)>>16));   
      SPI_ReadWriteByte((u8)(MINI_ICON_ADDR(icon)>>8));   
      SPI_ReadWriteByte((u8)(MINI_ICON_ADDR(icon)));
      SPI16BITMODE();
      SPIReadBuff(display,484);
      W25Qxx_SPI_CS_Set(1);
    
      SPI4RWLCD_CS0(LOW);
      SPI4RWLCD_RS(HIGH);
      SPIWriteBuff(display,484);  
      SPI4RWLCD_CS0(HIGH);
      SPI8BITMODE();
#elif LCD_HAL == LCD_HAL_STM32F103

#elif LCD_HAL == LCD_HAL_LPC1768 
      uint16_t display[484];

      LCD_SetWindow(sx,sy,sx+MINIICON_WIDTH-1,sy+MINIICON_HEIGHT-1);
      W25Qxx_SPI_CS_Set(0);                      
      SPI_ReadWriteByte(CMD_READ_DATA);          
      SPI_ReadWriteByte((u8)(MINI_ICON_ADDR(icon)>>16));   
      SPI_ReadWriteByte((u8)(MINI_ICON_ADDR(icon)>>8));   
      SPI_ReadWriteByte((u8)(MINI_ICON_ADDR(icon)));
      SPIReadBuff(display,484);
      W25Qxx_SPI_CS_Set(1);
    
      SPI4RWLCD_CS0(LOW);
      SPI4RWLCD_RS(HIGH);
      SPIWriteBuff(display,484);  
      SPI4RWLCD_CS0(HIGH);
#endif
      

}

void STATUS_ReadDisplay(uint16_t sx, uint16_t sy)
{
  #if LCD_HAL == LCD_HAL_STM32F407
     LCD_SetWindow(sx,sy,sx+ICON_STATUS_WIDTH-1,sy+ICON_STATUS_HEIGHT-1);

      W25Qxx_SPI_CS_Set(0);                      
      SPI_ReadWriteByte(CMD_READ_DATA);          
      SPI_ReadWriteByte((u8)(STATUS_ADDR>>16));   
      SPI_ReadWriteByte((u8)(STATUS_ADDR>>8));   
      SPI_ReadWriteByte((u8)(STATUS_ADDR));
      SPI16BITMODE();
      SPIReadBuff(display,9025);
      W25Qxx_SPI_CS_Set(1);
      
        SPI4RWLCD_CS0(LOW);
        SPI4RWLCD_RS(HIGH);
        SPIWriteBuff(display,9025);  
        SPI4RWLCD_CS0(HIGH);
      SPI8BITMODE();

      W25Qxx_SPI_CS_Set(0);                      
      SPI_ReadWriteByte(CMD_READ_DATA);          
      SPI_ReadWriteByte((u8)((STATUS_ADDR+18050)>>16));   
      SPI_ReadWriteByte((u8)((STATUS_ADDR+18050)>>8));   
      SPI_ReadWriteByte((u8)((STATUS_ADDR+18050)));
      SPI16BITMODE();
      SPIReadBuff(display,9025);
      W25Qxx_SPI_CS_Set(1);
      
        SPI4RWLCD_CS0(LOW);
        SPI4RWLCD_RS(HIGH);
        SPIWriteBuff(display,9025);  
        SPI4RWLCD_CS0(HIGH);
       SPI8BITMODE();
      W25Qxx_SPI_CS_Set(0);                      
      SPI_ReadWriteByte(CMD_READ_DATA);          
      SPI_ReadWriteByte((u8)((STATUS_ADDR+18050*2)>>16));   
      SPI_ReadWriteByte((u8)((STATUS_ADDR+18050*2)>>8));   
      SPI_ReadWriteByte((u8)((STATUS_ADDR+18050*2)));
      SPI16BITMODE();
      SPIReadBuff(display,1995);
      W25Qxx_SPI_CS_Set(1);
      
        SPI4RWLCD_CS0(LOW);
        SPI4RWLCD_RS(HIGH);
        SPIWriteBuff(display,1995);  
        SPI4RWLCD_CS0(HIGH);
       SPI8BITMODE();
  #elif LCD_HAL == LCD_HAL_STM32F103
   
  #elif LCD_HAL == LCD_HAL_LPC1768 
       uint16_t display[6682];
    LCD_SetWindow(sx,sy,sx+ICON_STATUS_WIDTH-1,sy+ICON_STATUS_HEIGHT-1);

      W25Qxx_SPI_CS_Set(0);                      
      SPI_ReadWriteByte(CMD_READ_DATA);          
      SPI_ReadWriteByte((u8)(STATUS_ADDR>>16));   
      SPI_ReadWriteByte((u8)(STATUS_ADDR>>8));   
      SPI_ReadWriteByte((u8)(STATUS_ADDR));
      SPIReadBuff(display,6682);
      W25Qxx_SPI_CS_Set(1);
      
        SPI4RWLCD_CS0(LOW);
        SPI4RWLCD_RS(HIGH);
        SPIWriteBuff(display,6682);  
        SPI4RWLCD_CS0(HIGH);

      W25Qxx_SPI_CS_Set(0);                      
      SPI_ReadWriteByte(CMD_READ_DATA);          
      SPI_ReadWriteByte((u8)((STATUS_ADDR+6682*2)>>16));   
      SPI_ReadWriteByte((u8)((STATUS_ADDR+6682*2)>>8));   
      SPI_ReadWriteByte((u8)((STATUS_ADDR+6682*2)));
      SPIReadBuff(display,6682);
      W25Qxx_SPI_CS_Set(1);
      
        SPI4RWLCD_CS0(LOW);
        SPI4RWLCD_RS(HIGH);
        SPIWriteBuff(display,6682);  
        SPI4RWLCD_CS0(HIGH);

      W25Qxx_SPI_CS_Set(0);                      
      SPI_ReadWriteByte(CMD_READ_DATA);          
      SPI_ReadWriteByte((u8)((STATUS_ADDR+6682*4)>>16));   
      SPI_ReadWriteByte((u8)((STATUS_ADDR+6682*4)>>8));   
      SPI_ReadWriteByte((u8)((STATUS_ADDR+6682*4)));
      SPIReadBuff(display,6681);
      W25Qxx_SPI_CS_Set(1);
      
        SPI4RWLCD_CS0(LOW);
        SPI4RWLCD_RS(HIGH);
        SPIWriteBuff(display,6681);  
        SPI4RWLCD_CS0(HIGH);
  #endif
}

void ICON_PressedDisplay(uint16_t sx, uint16_t sy, uint8_t icon)
{
  #if LCD_HAL == LCD_HAL_STM32F407
    uint16_t mode = 0x0FF0;
    uint16_t color = 0;
    uint32_t addr = ICON_ADDR(icon),i=0;
      LCD_SetWindow(sx, sy, sx+ICON_WIDTH-1, sy+ICON_HEIGHT-1);
        
        for(i=0;i<(ICON_WIDTH*ICON_HEIGHT);i++)//size
        {
          W25Qxx_SPI_CS_Set(0);                      
          SPI_ReadWriteByte(CMD_READ_DATA);          
          SPI_ReadWriteByte((u8)((addr+i*2)>>16));   
          SPI_ReadWriteByte((u8)((addr+i*2)>>8));   
          SPI_ReadWriteByte((u8)(addr+i*2));   
          SPI16BITMODE(); 
          color=SPI_ReadWrite2Byte(W25QXX_DUMMY_2BYTE)& mode;
          W25Qxx_SPI_CS_Set(1); 
          
          SPI4RWLCD_CS0(LOW);
          SPI4RWLCD_RS(HIGH);
          SPI_ReadWrite2Byte(color);
          SPI4RWLCD_CS0(HIGH);
          SPI8BITMODE(); 
        }      
  #elif LCD_HAL == LCD_HAL_STM32F103
      uint32_t addr = ICON_ADDR(icon),i=0;

      LCD_SetWindow(sx, sy, sx+ICON_WIDTH-1, sy+ICON_HEIGHT-1);
          W25Qxx_SPI_CS_Set(0);                      
        SPI_ReadWriteByte(CMD_READ_DATA);          
        SPI_ReadWriteByte((u8)(addr>>16));   
        SPI_ReadWriteByte((u8)(addr>>8));   
        SPI_ReadWriteByte((u8)(addr));
        SPIReadBuff(display,9025);
        W25Qxx_SPI_CS_Set(1);
        for(i=0;i<9025;i++)
          {
            display[i]&=0X0FF0;
          }
        SPI4RWLCD_CS0(LOW);
        SPI4RWLCD_RS(HIGH);
        SPIWriteBuff(display,9025);  
        SPI4RWLCD_CS0(HIGH);
  #elif LCD_HAL == LCD_HAL_LPC1768 
      uint16_t mode = 0x0FF0;
      uint32_t addr = ICON_ADDR(icon);
      uint16_t display[9025];
      LCD_SetWindow(sx, sy, sx+ICON_WIDTH-1, sy+ICON_HEIGHT-1);
      
        W25Qxx_SPI_CS_Set(0);                      
        SPI_ReadWriteByte(CMD_READ_DATA);          
        SPI_ReadWriteByte((u8)(addr>>16));   
        SPI_ReadWriteByte((u8)(addr>>8));   
        SPI_ReadWriteByte((u8)addr);
        SPIReadBuff(display,ICON_WIDTH*ICON_HEIGHT,mode);
        W25Qxx_SPI_CS_Set(1); 
          
          SPI4RWLCD_CS0(LOW);
          SPI4RWLCD_RS(HIGH);
          SPIWriteBuff(display,ICON_WIDTH*ICON_HEIGHT);
          SPI4RWLCD_CS0(HIGH);
  #endif
}

#include "../../../../sd/cardreader.h"
#if ENABLED(SDSUPPORT)
  extern CardReader card;
#endif

uint8_t TSCBoot::  scanUpdateFile(void) {
  uint8_t rst = 0;
  
  if (!IS_SD_INSERTED()) return rst;
  if (!card.isMounted()) card.mount();
  if (card.isMounted()) {
    if (card.getroot().exists(BMP_ROOT_DIR)) 
    {
       rst |= BMP;
    }
    if (card.getroot().exists(FONT_ROOT_DIR)) 
    {
       rst |= FONT;
    }
  }
  return rst;
}

bool rename_TFT()
{
  //SdFile file;
  //if (!file.open(card.getrootp(), ROOT_DIR, O_READ)) {return false;}
  //file.rename((SdBaseFile *)ROOT_DIR,ROOT_DIR_CUR);
  //file.close();
  card.getroot().exists(ROOT_DIR);
  card.getroot().rename((SdBaseFile *)ROOT_DIR,ROOT_DIR_CUR);
}

bool TSCBoot::bmpDecode(char *bmp, uint32_t addr) {
//  SdBaseFile 
  SdFile file;
  char  magic[2];  
  int   w, h, bytePerLine;  
  short bitDepth; 
  int   offset;
  
  if (!file.open(card.getrootp(), bmp, O_READ)) {return false;}

  if (!file.read(magic, 2) || memcmp(magic, "BM", 2)) return false;

  file.seekSet(10);
  file.read(&offset, sizeof(int));
  file.seekSet(18);
  file.read(&w, sizeof(int));
  file.read(&h, sizeof(int));
  file.seekSet(28);
  file.read(&bitDepth, sizeof(short));
  if (bitDepth < 24) return false;
  bytePerLine = w * bitDepth / 8;     
  if (bytePerLine % 4 !=0) bytePerLine = (bytePerLine / 4 + 1) * 4;  // BMP format 4 bytes alignment
  
  // Erase w25qxx sector before write
  for (int sector=0; sector < (w * h * 2 + W25QXX_SECTOR_SIZE - 1) / W25QXX_SECTOR_SIZE; sector++) {
    W25Qxx_EraseSector(addr + sector * W25QXX_SECTOR_SIZE);
  }
  uint16_t pointIndex=0;
  uint8_t  lcdcolor[4];
  uint8_t  buf[256];
  GUI_COLOR point;
  for(int y = 0; y < h; y++) {
    file.seekSet(offset + (h - y - 1) * bytePerLine);
    for(int x = 0; x < w; x++) {
      file.read((char *)&lcdcolor, bitDepth / 8);

      point.RGB.r=lcdcolor[2]>>3; // 24bits or 32bits convert to 16bits "565" format
      point.RGB.g=lcdcolor[1]>>2;        
      point.RGB.b=lcdcolor[0]>>3;

      //GUI_DrawPixel(x, y, point.color);
      
      buf[pointIndex++]=(uint8_t)(point.color>>8);
      buf[pointIndex++]=(uint8_t)(point.color&0xFF);
      
      if(pointIndex == W25QXX_PAGE_SIZE) {
        W25Qxx_WritePage(buf, addr, W25QXX_PAGE_SIZE);
        addr += W25QXX_PAGE_SIZE;
        pointIndex = 0;
      }
    }
  }    
  W25Qxx_WritePage(buf, addr, pointIndex);
  addr += pointIndex;
  file.close();

  return true;  
}  

void TSCBoot::updateIcon(void) {
  char nowBmp[64];  
  GUI_Clear(BK_COLOR);
  GUI_DispString(100, 5, (uint8_t*)"Icon Updating...!");

  if(bmpDecode((char *)BMP_ROOT_DIR "/Logo.bmp", LOGO_ADDR))
  {
    LOGO_ReadDisplay();
  }
 
  GUI_Clear(BK_COLOR);
  GUI_DispString(100, 5, (uint8_t*)"Status.bmp Updating...!");
   if(bmpDecode((char *)BMP_ROOT_DIR "/Status.bmp", STATUS_ADDR))
  {
    STATUS_ReadDisplay(ICON_STATUS_X,ICON_STATUS_Y);
  }

  GUI_Clear(BK_COLOR);
  for(uint32_t i=0; i < COUNT(iconBmpName); i++)
  {
    sprintf(nowBmp, (char *)BMP_ROOT_DIR "/%s.bmp", iconBmpName[i]);
    if(bmpDecode(nowBmp, ICON_ADDR(i)))
    {
      GUI_ClearRect(labelUpdateRect.x0, labelUpdateRect.y0, labelUpdateRect.x1, labelUpdateRect.y1);
      GUI_DispStringInPrect(&labelUpdateRect, (uint8_t *)nowBmp);
      ICON_ReadDisplay(iconUpdateRect.x0, iconUpdateRect.y0, i);
    }
  }
  GUI_Clear(BK_COLOR);
  for(uint32_t j=0;j<COUNT(miniiconBmpName);j++)
  {
     sprintf(nowBmp, (char *)BMP_ROOT_DIR "/%s.bmp", miniiconBmpName[j]);
    if(bmpDecode(nowBmp, MINI_ICON_ADDR(j)))
    {
      GUI_ClearRect(labelUpdateRect.x0, labelUpdateRect.y0, labelUpdateRect.x1, labelUpdateRect.y1);
      GUI_DispStringInPrect(&labelUpdateRect, (uint8_t *)nowBmp);
      MINIICON_ReadDisplay(miniiconUpdateRect.x0, miniiconUpdateRect.y0, j); 
    }
  }
     
}

void TSCBoot::updateFont(char *font, uint32_t addr) {

  SdFile file;
  char buffer[128];
  uint32_t  progress = 0;
  uint32_t  offset = 0;
  uint8_t*  tempbuf = NULL;
  
  if (!file.open(card.getrootp(), font, O_READ)) return;

  tempbuf = (uint8_t*)malloc(W25QXX_SECTOR_SIZE);
  if (tempbuf == NULL)  return;
  GUI_Clear(BK_COLOR);
  if(addr!=BYTE_ASCII_ADDR)
  {
  sprintf(buffer, "%s Size: %luKB", font, file.fileSize()>>10);
  GUI_DispString(0, 100, (uint8_t*)buffer);
  GUI_DispString(0, 140, (uint8_t*)"Updating:   %");
  }
  while (file.curPosition() < file.fileSize()) {
    uint16_t readBytes = file.read(tempbuf, W25QXX_SECTOR_SIZE);
    if (!readBytes) break;
    
    W25Qxx_EraseSector(addr + offset);
    W25Qxx_WriteBuffer(tempbuf, addr + offset, W25QXX_SECTOR_SIZE);
    offset += readBytes;
    if (progress != offset * 100 / file.fileSize()) {
      progress =  offset * 100 / file.fileSize();
      GUI_DispDec(0 + BYTE_WIDTH*9, 140, progress, 3, RIGHT);
    }
    if(readBytes !=W25QXX_SECTOR_SIZE) break;
  }
  
  file.close();
  free(tempbuf);
}

/*
void scanResetDir(void)
{
  FIL resetfile;
  if (f_open(&resetfile, TFT_RESET_FILE, FA_OPEN_EXISTING | FA_READ) == FR_OK)
  {
    f_close(&resetfile);
    f_rename(TFT_RESET_FILE, TFT_RESET_FILE ".DONE");
    //infoSettingsReset();
    //TSC_Calibration();
  }
}*/

void TSCBoot::scanUpdates(void) {
  volatile uint8_t result = 0;   //must volatile

  result = scanUpdateFile();
  if((result&FONT)&&(result & BMP))
  {
    GUI_Clear(BK_COLOR);
    GUI_DispString(100, 5, (uint8_t*)"update TFT35, Erasing spiflsh...!");
    W25Qxx_EraseBulk();
  }
  if (result & FONT) {
    updateFont((char *)FONT_ROOT_DIR "/byte_a~1.fon", BYTE_ASCII_ADDR);
    updateFont((char *)FONT_ROOT_DIR "/word_u~1.fon", WORD_UNICODE);
  }
  if (result & BMP) {
    updateIcon();
  }
  //rename_TFT();
 // if (result) f_rename(ROOT_DIR, ROOT_DIR".CUR");
 // scanResetDir();
}


// string convert to uint8, MSB
// "2C" to 0x2C
uint8_t string_2_uint8_t(const uint8_t *string)
{
  uint8_t rtv = 0;

  for (unsigned char i = 0; i < 2; i++)
  {
    rtv <<= 4;

    if (string[i] >= '0' && string[i] <= '9')
      rtv |= string[i] - '0';
    else if (string[i] >= 'A' && string[i] <= 'F')
      rtv |= string[i] - 'A' + 0xA;
    else if (string[i] >= 'a' && string[i] <= 'f')
      rtv |= string[i] - 'a' + 0xA;
    else
      rtv |= 0;
  }

  return rtv;
}

// string convert to uint32, MSB
uint32_t string_2_uint32(const uint8_t *string, const uint8_t bytes_num)
{
  uint32_t rtv = 0;

  for (uint8_t i = 0; i < bytes_num; i++)
  {
    rtv <<= 8;
    rtv |= string_2_uint8_t(string + 2 * i);
  }

  return rtv;
}

SdFile preview_file;
int16_t modelFileReadHalfword()
{
  uint8_t  buf1[2],buf2[2];
  uint8_t  rest = 2;
  uint8_t data1=0,data2=0;

  if (preview_file.read(buf1,rest)==-1) return -1;
  if (preview_file.read(buf2,rest)==-1) return -1;

  data1=string_2_uint8_t(buf1);
  data2=string_2_uint8_t(buf2);
  return (data1<<8)|data2;
}

bool preview_file_Decode(char *gcode,uint8_t keyicon) {
  uint16_t w = ICON_WIDTH;
  uint16_t h = ICON_HEIGHT;
  uint32_t offset=0,i=0;
 
  //if (!preview_file.open((SdFile*)filelist.shortFilename(), gcode, O_READ)) {return false;}
   if (!preview_file.open(card.getrootp(), gcode, O_READ))
    {
     preview_file.close();
     return false;
    }
  if(preview_file.fileSize()<0x2B304)
  {
    preview_file.close();
    return false;
  };
  preview_file.seekSet(MODEL_PREVIEW_OFFSET);
  
  if (modelFileReadHalfword() != w || modelFileReadHalfword() != h)
  return false;
  offset=MODEL_PREVIEW_OFFSET+11;
  preview_file.seekSet(offset);

  for (uint16_t y = 0; y < h; y++)
  {
    for (uint16_t x = 0; x < w+1; x++)
    {
      display[i]=modelFileReadHalfword();
      i++;
    } 
    offset=offset+w*4+3; 
    preview_file.seekSet(offset);
  }
  preview_file.seekSet(0);
  preview_file.close();
  const GUI_RECT *rect = rect_of_key + keyicon;
  SPI8BITMODE();
  LCD_SetWindow(rect->x0,rect->y0,rect->x0+ICON_WIDTH,rect->y0+ICON_HEIGHT);
  SPI16BITMODE();
  W25Qxx_SPI_CS_Set(1);       
  SPI4RWLCD_CS0(LOW);
  SPI4RWLCD_RS(HIGH);
  SPIWriteBuff(display,9025);  
  SPI4RWLCD_CS0(HIGH);
  SPI8BITMODE();
  return true;  
}  

#endif

