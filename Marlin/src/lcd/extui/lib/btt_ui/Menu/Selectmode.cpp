#include "../../../../../inc/MarlinConfigPre.h"
#if ENABLED(BTT_UI_SPI)
#include "../TSC_Menu.h"
#ifdef BTT_DUAL_MODE
int8_t  LCD_nowMode;

const GUI_RECT rect_of_mode[MODE_COUNT]={
  //2 select icon
  {1*SPACE_SELEX+0*ICON_WIDTH, SPACE_SELEY, 1*SPACE_SELEX+1*ICON_WIDTH, SPACE_SELEY+ICON_HEIGHT},
  {3*SPACE_SELEX+1*ICON_WIDTH, SPACE_SELEY, 3*SPACE_SELEX+2*ICON_WIDTH, SPACE_SELEY+ICON_HEIGHT},
};
const uint8_t icon_mode [MODE_COUNT]={
  ICON_MARLIN,
  ICON_BTT,
};

bool LCD_ReadPen(uint16_t intervals)
{
  static uint32_t TouchTime = 0;
  if(!XPT2046_Read_Pen())
  {
    if(millis()  - TouchTime > intervals)
    {
      return true;
    }
  }
  else
  {
    TouchTime = millis() ;
  }
  return false;
}

void loopCheckMode(void)
{
//  #ifndef CLEAN_MODE_SWITCHING_SUPPORT
//  IDEALLY I would like to be able to swap even when the TFT is in printing mode
//  but before I can allow that I need a way to make sure that we swap back into the right mode (and correct screen)
//  and I really want a reliable way to DETECT that the TFT should be in printing mode even when the print was started externally.
  if(isPrinting())
    return;

  if(infoMenu.menu[infoMenu.cur] == menuMode)
    return;

  if(LCD_ReadPen(LCD_CHANGE_MODE_INTERVALS) || encoder_ReadBtn(LCD_CHANGE_MODE_INTERVALS))
  {
    infoMenu.menu[++infoMenu.cur] = menuMode;
  }
}

uint16_t MODE_Y=rect_of_mode[0].y1 + BYTE_HEIGHT/2;
void drawModeIcon(void)
{
  for(uint8_t i = 0;i < MODE_COUNT; i++)
  {
    ICON_ReadDisplay(rect_of_mode[i].x0, rect_of_mode[i].y0, icon_mode[i]);
  }

  const GUI_RECT mode_title_rect[MODE_COUNT] = {
    {0,           MODE_Y,   text_startx,  MODE_Y + BYTE_HEIGHT},
    {text_startx, MODE_Y,   LCD_WIDTH_PIXEL,    MODE_Y + BYTE_HEIGHT},
  };

  GUI_RestoreColorDefault();

  GUI_DispStringInPrect(&mode_title_rect[0],(uint8_t *)"marlin");

  GUI_DispStringInPrect(&mode_title_rect[1],(uint8_t *)"BTT");
}

MKEY_VALUES MKeyGetValue(void)
{
  return (MKEY_VALUES)KEY_GetValue(COUNT(rect_of_mode), rect_of_mode);
}

void drawSelectedMode(int8_t nowMode)
{
  const uint8_t border_off = 4;
  for (uint8_t i = 0; i < MODE_COUNT; i++)
  {
    GUI_SetColor(i == nowMode ? BLACK : BLUE);
    GUI_DrawRect(rect_of_mode[i].x0 - border_off, rect_of_mode[i].y0 - border_off,
                 rect_of_mode[i].x1 + border_off, rect_of_mode[i].y1 + border_off);
  }
  GUI_RestoreColorDefault();
}

void menuModeCallBack(void)
{
  // #ifdef LCD_ENCODER_SUPPORT
  //   while((!XPT2046_Read_Pen()) || encoder_ReadBtn(LCD_BUTTON_INTERVALS)) {MarlinAPI_MainLoop();}      //wait for button release
  // #else
  //   while((!XPT2046_Read_Pen())){MarlinAPI_MainLoop();}      //wait for touch release
  // #endif

  MKEY_VALUES key_num = MKeyGetValue();

  if(key_num == MKEY_0 || key_num == MKEY_1)
  {
    LCD_nowMode = key_num;
    infoMenu.cur--;
  }

  #ifdef LCD_ENCODER_SUPPORT
    if(LCD_encoderPosition)
    {   
         if(LCD_encoderPosition < 0)
          {
            LCD_nowMode =SERIAL_TSC;
          }
          if(LCD_encoderPosition > 0)
          {
            LCD_nowMode =MARLIN;
          }
        drawSelectedMode(LCD_nowMode);
        LCD_encoderPosition = 0;
    }

    if(encoder_ReadBtn(0))
    {
        infoMenu.cur--;
    }
    loopCheckEncoderSteps();
  #endif
  if (infoMenu.menu[infoMenu.cur] != menuMode)
  {
    if (infoSettings.mode != LCD_nowMode)
    {
      infoSettings.mode = LCD_nowMode;
      storePara();
    }    
    infoMenuSelect();
  }
  
}

void menuMode(void)
{
  #ifdef LCD_ENCODER_SUPPORT
    LCD_encoderPosition = 0;
  #endif
  
  XPT2046_Init();
  W25Qxx_Init();
  LCD_Init();
  GUI_Clear(BLACK);
  drawModeIcon();
  LCD_nowMode = infoSettings.mode;
  
  //menuDrawTitle(&LABEL_MODE_SELECT);
 
  drawSelectedMode(LCD_nowMode);
  TSC_ReDrawIcon = NULL; // Disable icon redraw callback function
  menuSetFrontCallBack(menuModeCallBack);

}
#endif
#endif
