#include "../../../../../inc/MarlinConfigPre.h"
#if ENABLED(BTT_UI_SPI)
#include "../TSC_Menu.h"

LISTITEMS languageItems = {
//  title
LABEL_BACKGROUND,
  {
// icon                       label
   {LIST_LABEL,  NULL,           NULL},
   {LIST_LABEL,  NULL,           NULL},
   {LIST_LABEL,  NULL,           NULL},
   {LIST_LABEL,  NULL,           NULL},
   {LIST_LABEL,  NULL,           NULL},
   {LIST_BUTTON, CHAR_PAGEUP,    NULL},
   {LIST_BUTTON, CHAR_PAGEDOWN,  NULL},
   {LIST_BUTTON, CHAR_BACK,      NULL},
  }
};

// File list number per page
#define NUM_LAN_PAGE	5
SCROLL   LAN_titleScroll;
GUI_RECT LAN_titleRect={10, (TITLE_END_Y - 10 - BYTE_HEIGHT) / 2, LCD_WIDTH_PIXEL/2, (TITLE_END_Y -10 - BYTE_HEIGHT) / 2 + BYTE_HEIGHT};
uint8_t LanPage = 0;
uint16_t savelanguage=infoSettings.language;
uint16_t languageselect=0;

void menuCallBackIsLanguage(void)
{
  uint16_t key_num = KEY_GetValue(2, doubleBtnRect);
  switch(key_num)
  {
    case KEY_POPUP_CONFIRM:
     infoSettings.language=languageselect;
     storePara();
     volumeReminderMessage(LABEL_EEPROM_SAVE_SUCCESS, STATUS_NORMAL);
     infoMenu.cur--;
     infoMenu.cur--;
    break;

    case KEY_POPUP_CANCEL:	
     infoMenu.cur--;
    break;		
  }
}

void menuIsLanguage(void)
{
  popupDrawPage(bottomDoubleBtn, textSelect(LABEL_WARNING), textSelect(LABEL_IS_LANGUAGE), textSelect(LABEL_CONFIRM), textSelect(LABEL_CANNEL));
  menuSetFrontCallBack(menuCallBackIsLanguage);
}

void languageListDraw(void)
{
  savelanguage=infoSettings.language;
  menuDrawTitle(textSelect(LABEL_LANGUAGE));

  uint8_t i = 0;

  for (i=0;(i + LanPage * NUM_LAN_PAGE < LANGUAGE_NUM)&&(i<NUM_LAN_PAGE);i++)
  {
  languageItems.items[i].icon = CHAR_FILE;
  infoSettings.language=i + LanPage * NUM_LAN_PAGE;
  languageItems.items[i].label=textSelect(LABEL_LANGUAGE);
  menuDrawListItem(&languageItems.items[i], i);
  }
  infoSettings.language=savelanguage;
  for(; (i<NUM_LAN_PAGE); i++)			//background
  {		
    languageItems.items[i].icon = NULL;
    menuDrawListItem(&languageItems.items[i], i);
  }
}

void menuCallBackLanguage(void)
{
  KEY_VALUES key_num = menuKeyGetValue();
  uint8_t update = 0;
  switch(key_num)
  {
  
  case KEY_ICON_5:	
      if(LanPage > 0)
      {
        LanPage--;
        update=1;
      }
    break;

  case KEY_ICON_6:	
      if(LanPage+1 < (LANGUAGE_NUM+(NUM_LAN_PAGE-1))/NUM_LAN_PAGE)
      {
        LanPage++;
        update=1;
      }
    break;

   case KEY_ICON_7:
       infoMenu.cur--;
    break;

   default:    
     if(key_num <= KEY_ICON_4)
      {	
       languageselect=key_num + LanPage * NUM_LAN_PAGE;
       infoMenu.menu[++infoMenu.cur] = menuIsLanguage;
      }

    break;
  }

  if(update)
  {
    update = 0;
    languageListDraw();
  }
}

void menuLanguageSelect(void)
{
  for(uint8_t i = 0; i < NUM_LAN_PAGE; i++) languageItems.items[i].icon = NULL;
  menuDrawListPage(&languageItems);
  languageListDraw();
  menuSetFrontCallBack(menuCallBackLanguage);
}

#endif
