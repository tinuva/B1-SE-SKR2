#include "../../../../../inc/MarlinConfigPre.h"
#if ENABLED(BTT_UI_SPI)
#include "Print.h"
#include "../TSC_Menu.h"
#include "../../../../../feature/powerloss.h"
#include "../../../../../sd/cardreader.h"

static ExtUI::FileList filelist;


LISTITEMS printItems = {
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
#define NUM_PER_PAGE	5

SCROLL   titleScroll;
GUI_RECT titleRect={10, (TITLE_END_Y - 10 - BYTE_HEIGHT) / 2, LCD_WIDTH_PIXEL-10, (TITLE_END_Y -10 - BYTE_HEIGHT) / 2 + BYTE_HEIGHT};

SCROLL   gcodeScroll;
uint8_t  curPage = 0;

char title[1024] = "SD:";
char U_Disk_title[1024] = "U:";

bool EnterDir(const char *nextdir)
{
  // if(strlen(infoFile.title)+strlen(nextdir)+2>=MAX_PATH_LEN) return 0;
  if(SD_SUPPORT==1)
  {
  strcat(title, "/");
  strcat(title, nextdir);
  }
  else
  {
  strcat(U_Disk_title, "/");
  strcat(U_Disk_title, nextdir);
  }
  return 1;
}
void ExitDir(void)
{
  if(SD_SUPPORT==1)
  {
    int i = strlen(title);
    for(; i > 0 && title[i] != '/';i--)
    {	
    }
    title[i]=0;
  }
  else
  {
    int i = strlen(U_Disk_title);
    for(; i > 0 && U_Disk_title[i] != '/';i--)
    {	
    }
    U_Disk_title[i]=0;
  }
  
}


void gocdeListDraw(void)
{
  if(SD_SUPPORT==1)
  {
  Scroll_CreatePara(&titleScroll, (uint8_t* )title, &titleRect);
  printItems.title.address = (uint8_t* )title;
  }
  else
  {
  Scroll_CreatePara(&titleScroll, (uint8_t* )U_Disk_title, &titleRect);
  printItems.title.address = (uint8_t* )U_Disk_title;
  }

  GUI_SetBkColor(TITLE_COLOR);
  GUI_ClearPrect(&titleRect);
  GUI_SetBkColor(BK_COLOR);

  uint8_t i = 0;
  //TSC_ReDrawIcon = NULL;// itemDrawIconPress;
  for(i=0;(i + curPage * NUM_PER_PAGE < filelist.count())&&(i < NUM_PER_PAGE); i++)  // folder
  {
    if (!filelist.seek(i + curPage * NUM_PER_PAGE)) continue;
    if (filelist.isDir()) 
      printItems.items[i].icon = CHAR_FOLDER;
    else 
      printItems.items[i].icon = CHAR_FILE;
    printItems.items[i].label = (uint8_t* )filelist.filename();
    menuDrawListItem(&printItems.items[i], i);
  }

  for(; (i<NUM_PER_PAGE); i++)			//background
  {		
    printItems.items[i].icon = NULL;
    menuDrawListItem(&printItems.items[i], i);
  }
}

const int16_t labelVolumeError[] = {LABEL_READ_TFTSD_ERROR, LABEL_READ_U_DISK_ERROR, LABEL_READ_ONBOARDSD_ERROR};

void menuCallBackGocdeList(void)
{
  static bool lock = false;
  KEY_VALUES key_num = menuKeyGetValue();
  uint8_t update = 0;

  GUI_SetBkColor(TITLE_COLOR);
  Scroll_DispString(&titleScroll, LEFT);    //
  GUI_SetBkColor(BK_COLOR);
  #ifndef GCODE_LIST_MODE
    Scroll_DispString(&gcodeScroll, CENTER); //
  #endif

  if (lock) return;
  lock = true;
  switch(key_num)
  {
    case KEY_ICON_5:	
      if(curPage > 0)
      {
        curPage--;
        update=1;
      }
      break;

    case KEY_ICON_6:	
      if(curPage+1 < (filelist.count()+(NUM_PER_PAGE-1))/NUM_PER_PAGE)
      {
        curPage++;
        update=1;
      }
      break;

    case KEY_ICON_7:
      curPage = 0;
      if(filelist.isAtRootDir())
      {
        SD_SUPPORT=0;
        infoMenu.cur--;
        break;
      }
      else
      {
        ExitDir();
        filelist.upDir();
        update = 1;
      }
      break;

    case KEY_IDLE:
      break;

    default:                   
      if(key_num <= KEY_ICON_4)
      {	
        if (filelist.seek(curPage * NUM_PER_PAGE + key_num))
        {
          u8 ck=filelist.isDir();
          if (ck) {
            if(EnterDir(filelist.longFilename()) == false) break;
            update = 1;
            curPage = 0;
            filelist.changeDir(filelist.shortFilename());
          } else { //gcode
            ExtUI::printFile(filelist.shortFilename());
          }
        }
      }
      break;
  }
    
  if(update)
  {
    update = 0;
    gocdeListDraw();
  }
  if(!IS_SD_INSERTED())
  {
   // resetInfoFile();
    infoMenu.cur--;
  }
  lock = false;
}

#define NUM_ICON_PAGE	5
uint8_t  Icon_curPage = 0;

const GUI_RECT gcodeRect[NUM_ICON_PAGE] = {
  {BYTE_WIDTH/2+0*SPACE_X_PER_ICON,  1*ICON_HEIGHT+0*SPACE_Y+ICON_LIST_START_Y+(SPACE_Y-BYTE_HEIGHT)/2,
  1*SPACE_X_PER_ICON-BYTE_WIDTH/2,  1*ICON_HEIGHT+0*SPACE_Y+ICON_LIST_START_Y+(SPACE_Y-BYTE_HEIGHT)/2+BYTE_HEIGHT},

  {BYTE_WIDTH/2+1*SPACE_X_PER_ICON,  1*ICON_HEIGHT+0*SPACE_Y+ICON_LIST_START_Y+(SPACE_Y-BYTE_HEIGHT)/2,
  2*SPACE_X_PER_ICON-BYTE_WIDTH/2,  1*ICON_HEIGHT+0*SPACE_Y+ICON_LIST_START_Y+(SPACE_Y-BYTE_HEIGHT)/2+BYTE_HEIGHT},

  {BYTE_WIDTH/2+2*SPACE_X_PER_ICON,  1*ICON_HEIGHT+0*SPACE_Y+ICON_LIST_START_Y+(SPACE_Y-BYTE_HEIGHT)/2,
  3*SPACE_X_PER_ICON-BYTE_WIDTH/2,  1*ICON_HEIGHT+0*SPACE_Y+ICON_LIST_START_Y+(SPACE_Y-BYTE_HEIGHT)/2+BYTE_HEIGHT},

  {BYTE_WIDTH/2+3*SPACE_X_PER_ICON,  1*ICON_HEIGHT+0*SPACE_Y+ICON_LIST_START_Y+(SPACE_Y-BYTE_HEIGHT)/2,
  4*SPACE_X_PER_ICON-BYTE_WIDTH/2,  1*ICON_HEIGHT+0*SPACE_Y+ICON_LIST_START_Y+(SPACE_Y-BYTE_HEIGHT)/2+BYTE_HEIGHT},

  {BYTE_WIDTH/2+0*SPACE_X_PER_ICON,  2*ICON_HEIGHT+1*SPACE_Y+ICON_LIST_START_Y+(SPACE_Y-BYTE_HEIGHT)/2,
  1*SPACE_X_PER_ICON-BYTE_WIDTH/2,  2*ICON_HEIGHT+1*SPACE_Y+ICON_LIST_START_Y+(SPACE_Y-BYTE_HEIGHT)/2+BYTE_HEIGHT},
};


MENUITEMS gocdeIconItems = {
//title
LABEL_READY,
//icon
{
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_PAGE_UP,              LABEL_PAGE_UP},
  {ICON_PAGE_DOWN,            LABEL_PAGE_DOWN},    
  {ICON_BACK,                 LABEL_BACK},
}
};

void normalNameDisp(const GUI_RECT *rect, uint8_t *name)
{
  if(name == NULL) return;

  GUI_ClearPrect(rect);
  GUI_SetRange(rect->x0, rect->y0, rect->x1, rect->y1);
  GUI_DispStringInPrect(rect, name);
  GUI_CancelRange();
}

void gocdeIconDraw(void)
{
  if(SD_SUPPORT==1)
  {
    menuDrawTitle((uint8_t* )title);
  }
  else
  {
    menuDrawTitle((uint8_t* )U_Disk_title);
  }
 
  uint8_t i = 0;
    TSC_ReDrawIcon = NULL;// itemDrawIconPress;
    //menuDrawPage(&gocdeIconItems);
  for(i=0;(i + Icon_curPage * NUM_ICON_PAGE < filelist.count())&&(i < NUM_ICON_PAGE); i++)  // folder
  {
    if (!filelist.seek(i + Icon_curPage * NUM_ICON_PAGE)) continue;

    if (filelist.isDir()) 
    {
      gocdeIconItems.items[i].icon=ICON_FOLDER;
      menuDrawItem(&gocdeIconItems.items[i],i);
    }
    else 
    {

  SERIAL_ECHO(" filelist.shortFilename()");
  SERIAL_ECHO((char *)filelist.shortFilename());
  SERIAL_ECHO("\r\n");
      if(preview_file_Decode((char *)filelist.shortFilename(),i))
      {
       gocdeIconItems.items[i].icon=ICON_BACKGROUND; 
      }
      else
      {
      gocdeIconItems.items[i].icon=ICON_FILE; 
      menuDrawItem(&gocdeIconItems.items[i],i);
      }
    }
  
    gocdeIconItems.items[i].label.index=LABEL_BACKGROUND;
    normalNameDisp(&gcodeRect[i], (uint8_t* )filelist.filename());
  }

  for(; (i<NUM_ICON_PAGE); i++)			//background
  {		
    gocdeIconItems.items[i].icon = ICON_BACKGROUND;
    gocdeIconItems.items[i].label.index=LABEL_BACKGROUND;
    menuDrawItem(&gocdeIconItems.items[i],i);
  }

  menuDrawItem(&gocdeIconItems.items[5],5);
  menuDrawItem(&gocdeIconItems.items[6],6);
  menuDrawItem(&gocdeIconItems.items[7],7);
}


void menuCallBackGocdeIcon(void)
{
  static bool lock = false;
  KEY_VALUES key_num = menuKeyGetValue();
  uint8_t update = 0;
  
  if (lock) return;
  lock = true;
  switch(key_num)
  {
    case KEY_ICON_5:	
      if(Icon_curPage > 0)
      {
        Icon_curPage--;
        update=1;
      }
      break;

    case KEY_ICON_6:	
      if(Icon_curPage+1 < (filelist.count()+(NUM_ICON_PAGE-1))/NUM_ICON_PAGE)
      {
        Icon_curPage++;
        update=1;
      }
      break;

    case KEY_ICON_7:
      
      Icon_curPage = 0;
      if(filelist.isAtRootDir())
      {
        SD_SUPPORT=0;
        infoMenu.cur--;
        break;
      }
      else
      {
        ExitDir();
        filelist.upDir();
        update = 1;
      }
    
      break;

    case KEY_IDLE:
      break;

    default:                   
      if(key_num <= KEY_ICON_4)
        {	
          if (filelist.seek(Icon_curPage * NUM_ICON_PAGE + key_num))
          {
            u8 ck=filelist.isDir();
            if (ck) {
              if(EnterDir(filelist.longFilename()) == false) break;
              update = 1;
              Icon_curPage = 0;
              filelist.changeDir(filelist.shortFilename());
            } else { //gcode
              ExtUI::printFile(filelist.shortFilename());
            }
          }
        }
      break;
  }

  if(update)
  {
    update = 0;
    gocdeIconDraw();
  }
  if(!IS_SD_INSERTED())
  {
    infoMenu.cur--;
  }
  lock = false;
}

// menuPrintFromSource
void menuPrint(void)
{
  GUI_Clear(BK_COLOR);
  // card.remount();
  // GUI_DispStringInRect(0, 0, LCD_WIDTH_PIXEL, LCD_HEIGHT_PIXEL, textSelect(LABEL_LOADING));
  if (ExtUI::isMediaInserted())
  {
    filelist.refresh();
    if(infoSettings.file_mode==LIST_MODE)
    {
      for(uint8_t i = 0; i < NUM_PER_PAGE; i++) printItems.items[i].icon = NULL;
      menuDrawListPage(&printItems);
      gocdeListDraw();
      menuSetFrontCallBack(menuCallBackGocdeList);
    }
    else if(infoSettings.file_mode==ICON_MODE)
    {
      gocdeIconDraw();
      menuSetFrontCallBack(menuCallBackGocdeIcon);
    }
  }
  else
  {
    if( SD_SUPPORT==1)
    {
    GUI_DispStringInRect(0, 0, LCD_WIDTH_PIXEL, LCD_HEIGHT_PIXEL, textSelect(LABEL_READ_ONBOARDSD_ERROR));
    }
    else
    {
    GUI_DispStringInRect(0, 0, LCD_WIDTH_PIXEL, LCD_HEIGHT_PIXEL, textSelect(LABEL_READ_U_DISK_ERROR));      
    }

    ExtUI::delay_ms(1000);
    infoMenu.cur--;
  }
}
#endif

