#include "../../../../../inc/MarlinConfigPre.h"
#if ENABLED(BTT_UI_SPI)
#include "menu.h"
#include "../TSC_Menu.h"


// exhibitRect is 2 ICON Space in the Upper Row and 2 Center Coloum. 
const GUI_RECT exhibitRect = {
   1*ICON_WIDTH+1*SPACE_X+START_X,  0*ICON_HEIGHT+0*SPACE_Y+TITLE_END_Y,  3*ICON_WIDTH+2*SPACE_X+START_X,  1*ICON_HEIGHT+0*SPACE_Y+TITLE_END_Y
};

const GUI_RECT rect_of_key[ITEM_PER_PAGE*2]={
  //8 icons area
  {0*ICON_WIDTH+0*SPACE_X+START_X,  0*ICON_HEIGHT+0*SPACE_Y+TITLE_END_Y,  1*ICON_WIDTH+0*SPACE_X+START_X,  1*ICON_HEIGHT+0*SPACE_Y+TITLE_END_Y},
  {1*ICON_WIDTH+1*SPACE_X+START_X,  0*ICON_HEIGHT+0*SPACE_Y+TITLE_END_Y,  2*ICON_WIDTH+1*SPACE_X+START_X,  1*ICON_HEIGHT+0*SPACE_Y+TITLE_END_Y},
  {2*ICON_WIDTH+2*SPACE_X+START_X,  0*ICON_HEIGHT+0*SPACE_Y+TITLE_END_Y,  3*ICON_WIDTH+2*SPACE_X+START_X,  1*ICON_HEIGHT+0*SPACE_Y+TITLE_END_Y},
  {3*ICON_WIDTH+3*SPACE_X+START_X,  0*ICON_HEIGHT+0*SPACE_Y+TITLE_END_Y,  4*ICON_WIDTH+3*SPACE_X+START_X,  1*ICON_HEIGHT+0*SPACE_Y+TITLE_END_Y},
  {0*ICON_WIDTH+0*SPACE_X+START_X,  1*ICON_HEIGHT+1*SPACE_Y+TITLE_END_Y,  1*ICON_WIDTH+0*SPACE_X+START_X,  2*ICON_HEIGHT+1*SPACE_Y+TITLE_END_Y},
  {1*ICON_WIDTH+1*SPACE_X+START_X,  1*ICON_HEIGHT+1*SPACE_Y+TITLE_END_Y,  2*ICON_WIDTH+1*SPACE_X+START_X,  2*ICON_HEIGHT+1*SPACE_Y+TITLE_END_Y},
  {2*ICON_WIDTH+2*SPACE_X+START_X,  1*ICON_HEIGHT+1*SPACE_Y+TITLE_END_Y,  3*ICON_WIDTH+2*SPACE_X+START_X,  2*ICON_HEIGHT+1*SPACE_Y+TITLE_END_Y},
  {3*ICON_WIDTH+3*SPACE_X+START_X,  1*ICON_HEIGHT+1*SPACE_Y+TITLE_END_Y,  4*ICON_WIDTH+3*SPACE_X+START_X,  2*ICON_HEIGHT+1*SPACE_Y+TITLE_END_Y},

  //8 labels area
  {0*SPACE_X_PER_ICON,  1*ICON_HEIGHT+0*SPACE_Y+TITLE_END_Y,  1*SPACE_X_PER_ICON,  1*ICON_HEIGHT+1*SPACE_Y+TITLE_END_Y},
  {1*SPACE_X_PER_ICON,  1*ICON_HEIGHT+0*SPACE_Y+TITLE_END_Y,  2*SPACE_X_PER_ICON,  1*ICON_HEIGHT+1*SPACE_Y+TITLE_END_Y},
  {2*SPACE_X_PER_ICON,  1*ICON_HEIGHT+0*SPACE_Y+TITLE_END_Y,  3*SPACE_X_PER_ICON,  1*ICON_HEIGHT+1*SPACE_Y+TITLE_END_Y},
  {3*SPACE_X_PER_ICON,  1*ICON_HEIGHT+0*SPACE_Y+TITLE_END_Y,  4*SPACE_X_PER_ICON,  1*ICON_HEIGHT+1*SPACE_Y+TITLE_END_Y},
  {0*SPACE_X_PER_ICON,  2*ICON_HEIGHT+1*SPACE_Y+TITLE_END_Y,  1*SPACE_X_PER_ICON,  2*ICON_HEIGHT+2*SPACE_Y+TITLE_END_Y},
  {1*SPACE_X_PER_ICON,  2*ICON_HEIGHT+1*SPACE_Y+TITLE_END_Y,  2*SPACE_X_PER_ICON,  2*ICON_HEIGHT+2*SPACE_Y+TITLE_END_Y},
  {2*SPACE_X_PER_ICON,  2*ICON_HEIGHT+1*SPACE_Y+TITLE_END_Y,  3*SPACE_X_PER_ICON,  2*ICON_HEIGHT+2*SPACE_Y+TITLE_END_Y},
  {3*SPACE_X_PER_ICON,  2*ICON_HEIGHT+1*SPACE_Y+TITLE_END_Y,  4*SPACE_X_PER_ICON,  2*ICON_HEIGHT+2*SPACE_Y+TITLE_END_Y},
};


MENU_TYPE menuType = MENU_TYPE_ICON;
static const LISTITEMS *curListItems = NULL;   //current listmenu
//Clean up the gaps outside icons
void menuClearGaps(void)
{
  const GUI_RECT gaps[]={
  {0,                               0,              LCD_WIDTH_PIXEL,                 TITLE_END_Y-10},
  {0,                               TITLE_END_Y-10, LCD_WIDTH_PIXEL,                 TITLE_END_Y},
  {0,                               TITLE_END_Y,    0*ICON_WIDTH+0*SPACE_X+START_X,  LCD_HEIGHT_PIXEL},
  {1*ICON_WIDTH+0*SPACE_X+START_X,  TITLE_END_Y,    1*ICON_WIDTH+1*SPACE_X+START_X,  LCD_HEIGHT_PIXEL},
  {2*ICON_WIDTH+1*SPACE_X+START_X,  TITLE_END_Y,    2*ICON_WIDTH+2*SPACE_X+START_X,  LCD_HEIGHT_PIXEL},
  {3*ICON_WIDTH+2*SPACE_X+START_X,  TITLE_END_Y,    3*ICON_WIDTH+3*SPACE_X+START_X,  LCD_HEIGHT_PIXEL},
  {4*ICON_WIDTH+3*SPACE_X+START_X,  TITLE_END_Y,    LCD_WIDTH_PIXEL,                 LCD_HEIGHT_PIXEL}};

  //GUI_SetBkColor(TITLE_COLOR);
  GUI_ClearPrect(gaps);
  //GUI_SetBkColor(BK_COLOR);
  for(uint8_t i = 1; i < COUNT(gaps); i++)
    GUI_ClearPrect(gaps + i);
}

static const MENUITEMS *curMenuItems = 0;   //current menu

uint8_t *labelGetAddress(const LABEL *label)
{
  if (label->index == LABEL_BACKGROUND) return 0;  // No content in label
  if (label->index < LABEL_NUM) // Index of language
    return textSelect(label->index);
  else // Address of string
    return label->address;
}

void menuDrawItem(const ITEM *item, uint8_t positon)
{
  uint8_t *content = labelGetAddress(&item->label);
  const GUI_RECT *rect = rect_of_key + positon;
  if(item->icon != ICON_BACKGROUND)
    if(item->icon != ICON_PREVIEW)
    ICON_ReadDisplay(rect->x0, rect->y0, item->icon);
    else
    {}
  else
    GUI_ClearPrect(rect);

  rect = rect_of_key + ITEM_PER_PAGE + positon;
  GUI_ClearPrect(rect);
  if (content)
    GUI_DispStringInPrect(rect, content);
}


static REMINDER reminder = {{0, 0, LCD_WIDTH_PIXEL, TITLE_END_Y-10}, 0, STATUS_IDLE, LABEL_BACKGROUND};
static REMINDER volumeReminder = {{0, 0, LCD_WIDTH_PIXEL, TITLE_END_Y-10}, 0, STATUS_IDLE, LABEL_BACKGROUND};

void reminderMessage(int16_t inf, SYS_STATUS status)
{
  reminder.inf = inf;
  GUI_SetColor(RED);
  GUI_DispStringInPrect(&reminder.rect, textSelect(reminder.inf));
  GUI_SetColor(FK_COLOR);
  reminder.status = status;
  reminder.time = millis();
}

void volumeReminderMessage(int16_t inf, SYS_STATUS status)
{ 
  volumeReminder.inf = inf;
   //(infoMenu.menu[infoMenu.cur] != menuMarlinMode)&&(infoMenu.menu[infoMenu.cur]!=menuMode)&&
  if((infoMenu.menu[infoMenu.cur]!=menuSendGcode)&&(infoMenu.menu[infoMenu.cur]!=menuTerminal))
  {
  GUI_SetColor(GBLUE);
  GUI_DispStringInPrect(&volumeReminder.rect, textSelect(volumeReminder.inf));
  GUI_SetColor(FK_COLOR);
  }
  volumeReminder.status = status;
  volumeReminder.time = millis();
}

void loopReminderClear(void)
{	
  switch(reminder.status)
  {
    case STATUS_IDLE:
      return;
    
    case STATUS_BUSY:
      if(queue.ring_buffer.length>= BUFSIZE)
        return;
      break;
      
    case STATUS_NORMAL:
      if(millis() < reminder.time + 2000)
        return;
      break;
    default:
      return;
  }

  /* Clear warning message */		
  reminder.status = STATUS_IDLE;
  if(curMenuItems == NULL)
    return;
  menuDrawTitle(labelGetAddress(&curMenuItems->title));
}

void loopVolumeReminderClear(void)
{	
  switch(volumeReminder.status)
  {
    case STATUS_NORMAL:
      if(millis() < volumeReminder.time + 2000)
        return;
      break;
    default:
      return;
  }

  /* Clear warning message */		
  volumeReminder.status = STATUS_IDLE;
  if(curMenuItems == NULL)
    return;
    
  menuDrawTitle(labelGetAddress(&curMenuItems->title));

}

void menuDrawTitle(const uint8_t *content)
{
  uint16_t start_y = (TITLE_END_Y - 10 - BYTE_HEIGHT) / 2;
  if((infoMenu.menu[infoMenu.cur]!=menuSendGcode)&&(infoMenu.menu[infoMenu.cur]!=menuTerminal))
  {
    GUI_FillRectColor(10, (TITLE_END_Y - 10 - BYTE_HEIGHT) / 2, LCD_WIDTH_PIXEL, start_y+BYTE_HEIGHT, TITLE_COLOR);

    if (content)
    {
      GUI_SetTextMode(GUI_TEXTMODE_TRANS);
      GUI_DispLenString(10, start_y, content, (LCD_WIDTH_PIXEL-20)/2);
      GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
    }
    #ifdef TEMP_LOAG
    temp_logo_display();
    #endif

    if(reminder.status == STATUS_IDLE) return;
    GUI_SetColor(RED);
    GUI_DispStringInPrect(&reminder.rect, textSelect(reminder.inf));
    GUI_SetColor(FK_COLOR);


  }
}

//Draw the entire interface
void menuDrawPage(const MENUITEMS *menuItems)
{
  menuType = MENU_TYPE_ICON;
  curMenuItems = menuItems;
  TSC_ReDrawIcon = itemDrawIconPress;

  //GUI_Clear(BLACK);
  menuClearGaps(); //Use this function instead of GUI_Clear to eliminate the splash screen when clearing the screen.
  menuDrawTitle(labelGetAddress(&menuItems->title));
  for (volatile uint8_t i = 0; i < ITEM_PER_PAGE; i++)
  {
    menuDrawItem(&menuItems->items[i], i);
  }
}

#define LIST_ICON_WIDTH   85
#define LIST_ICON_HEIGHT  70
#define ICON_START_Y (TITLE_END_Y+10)
#define LISTITEM_WIDTH (LCD_WIDTH_PIXEL-(3*START_X)-LIST_ICON_WIDTH)
#define LISTITEM_HEIGHT ((LCD_HEIGHT_PIXEL-ICON_START_Y-START_X)/5)
#define LISTICON_SPACE_Y ((LCD_HEIGHT_PIXEL-ICON_START_Y-START_X-(3*LIST_ICON_HEIGHT))/ 2)
const GUI_RECT rect_of_keyListView[ITEM_PER_PAGE]={
  //8 icons area
  {START_X,  0*LISTITEM_HEIGHT+ICON_START_Y,    LISTITEM_WIDTH+START_X,  1*LISTITEM_HEIGHT+ICON_START_Y},
  {START_X,  1*LISTITEM_HEIGHT+ICON_START_Y,    LISTITEM_WIDTH+START_X,  2*LISTITEM_HEIGHT+ICON_START_Y},
  {START_X,  2*LISTITEM_HEIGHT+ICON_START_Y,    LISTITEM_WIDTH+START_X,  3*LISTITEM_HEIGHT+ICON_START_Y},
  {START_X,  3*LISTITEM_HEIGHT+ICON_START_Y,    LISTITEM_WIDTH+START_X,  4*LISTITEM_HEIGHT+ICON_START_Y},
  {START_X,  4*LISTITEM_HEIGHT+ICON_START_Y,    LISTITEM_WIDTH+START_X,  5*LISTITEM_HEIGHT+ICON_START_Y},

  {2*START_X + LISTITEM_WIDTH,  0*LIST_ICON_HEIGHT+0*LISTICON_SPACE_Y+ICON_START_Y,  2*START_X + LISTITEM_WIDTH + 1*LIST_ICON_WIDTH,  1*LIST_ICON_HEIGHT+0*LISTICON_SPACE_Y+ICON_START_Y},
  {2*START_X + LISTITEM_WIDTH,  1*LIST_ICON_HEIGHT+1*LISTICON_SPACE_Y+ICON_START_Y,  2*START_X + LISTITEM_WIDTH + 1*LIST_ICON_WIDTH,  2*LIST_ICON_HEIGHT+1*LISTICON_SPACE_Y+ICON_START_Y},
  {2*START_X + LISTITEM_WIDTH,  2*LIST_ICON_HEIGHT+2*LISTICON_SPACE_Y+ICON_START_Y,  2*START_X + LISTITEM_WIDTH + 1*LIST_ICON_WIDTH,  3*LIST_ICON_HEIGHT+2*LISTICON_SPACE_Y+ICON_START_Y},
};
#define LISTBTN_BKCOLOR 0x2187
#define MAT_RED         0xE124
#define MAT_YELLOW      0xED80
#define MAT_GREEN       0x1DC4
#define MAT_BLUE        0x24BD
#define MAT_ORANGE      0xF3A0
#define MATT_DARKGRAY   0x52AA
#define MAT_LOWWHITE    0xCE79
#define MATT_PURPLE     0x9135
void menuDrawListItem(const LISTITEM *item, uint8_t position)
{
  const GUI_RECT *rect = rect_of_keyListView + position;
  int16_t sy = (rect->y1 - rect->y0 - BYTE_HEIGHT) / 2 + rect->y0;

  GUI_ClearRect(rect->x0, rect->y0+1, rect->x1, rect->y1+1);
  if (item->icon == NULL) return;

  if(item->type == LIST_BUTTON) {
    GUI_SetBkColor(GRAY);
    GUI_ClearPrect(rect);
    GUI_DispStringInPrect(rect, item->icon);
    GUI_SetBkColor(BK_COLOR);
  } else if (item->type == LIST_LABEL) {
    GUI_SetColor(GRAY);
    GUI_HLine(rect->x0, rect->y0, rect->x1);
    GUI_HLine(rect->x0, rect->y1, rect->x1);
    GUI_SetRange(rect->x0, rect->y0, rect->x1, rect->y1);
    if (item->icon == CHAR_FOLDER)
      GUI_SetColor(MAT_YELLOW);
    else if (item->icon == CHAR_FILE)
      GUI_SetColor(MAT_BLUE);
    GUI_DispString(rect->x0 + BYTE_WIDTH / 2, sy, item->icon);

    GUI_SetColor(FK_COLOR);
    GUI_DispString(rect->x0 + BYTE_WIDTH * 3, sy, item->label);
    GUI_CancelRange();
  }
}
//Draw the entire interface
void menuDrawListPage(const LISTITEMS *listItems)
{
  menuType = MENU_TYPE_LISTVIEW;
  TSC_ReDrawIcon = itemDrawIconPress;
  curListItems = listItems;

  GUI_SetBkColor(TITLE_COLOR);
  GUI_ClearRect(0, 0, LCD_WIDTH_PIXEL, TITLE_END_Y-10);
  GUI_SetBkColor(BK_COLOR);
  GUI_ClearRect(0, TITLE_END_Y-10, LCD_WIDTH_PIXEL, LCD_HEIGHT_PIXEL);

  //menuClearGaps(); //Use this function instead of GUI_Clear to eliminate the splash screen when clearing the screen.
  menuDrawTitle(labelGetAddress(&listItems->title));

  for (uint8_t i = 0; i < ITEM_PER_PAGE; i++)
  {
    //const GUI_RECT *rect = rect_of_keyListView + i;
    if (listItems->items[i].icon != NULL)
      menuDrawListItem(&listItems->items[i], i);
  }
}

//When there is a button value, the icon changes color and redraws
void itemDrawIconPress(uint8_t position, uint8_t is_press)
{
  if (position > KEY_ICON_7) return;

  if (menuType == MENU_TYPE_ICON)
  {
    if (curMenuItems == 0) return;
    if (curMenuItems->items[position].icon == ICON_BACKGROUND) return;
    const GUI_RECT *rect = rect_of_key + position;

    if (is_press) // Turn green when pressed
      ICON_PressedDisplay(rect->x0, rect->y0, curMenuItems->items[position].icon);
    else // Redraw normal icon when released
      ICON_ReadDisplay(rect->x0, rect->y0, curMenuItems->items[position].icon);
  }
  else if (menuType == MENU_TYPE_LISTVIEW)
  { //draw rec over list item if pressed
    if (curListItems == NULL) return;

    const GUI_RECT *rect = rect_of_keyListView + position;

    if (curListItems->items[position].icon == NULL)
    {
      GUI_ClearPrect(rect);
      return;
    }
    if (is_press){
      GUI_DrawPrect(rect);
    }
    else{
      GUI_SetColor(BK_COLOR);
      GUI_DrawPrect(rect);
      GUI_SetColor(FK_COLOR);
    }

  }
}

static void (* curMenuRedrawHandle)(void) = NULL; //current custom menu
static LABEL * curTitle = NULL;
static const GUI_RECT *curRect = NULL; //current menu layout grid
static u16 curRectCount = 0;    //current menu layout rect count

// Get button value
KEY_VALUES menuKeyGetValue(void)
{
  KEY_VALUES tempkey = KEY_IDLE;
  if (menuType == MENU_TYPE_ICON)
  {
    return (KEY_VALUES)KEY_GetValue(COUNT(rect_of_key), rect_of_key); // for normal menu
  }
  else if (menuType == MENU_TYPE_LISTVIEW)
  {
    return (KEY_VALUES)KEY_GetValue(COUNT(rect_of_keyListView), rect_of_keyListView); //for listview
  }
  else if(menuType == MENU_TYPE_FULLSCREEN)
  {
    return tempkey = (KEY_VALUES)KEY_GetValue(curRectCount, curRect);
  }
  // catch titlebar press to enable it on every menu
  if (tempkey == KEY_TITLEBAR)
  {
    return KEY_IDLE;
  }
  else
  {
    return tempkey;
  }
}

KEY_VALUES PrintmenuKeyGetValue(void)
{
    return (KEY_VALUES)Print_KEY_GetValue(COUNT(rect_of_key), rect_of_key);
}

bool CheckMenuDis()
{// (infoMenu.menu[infoMenu.cur]!=menuMarlinMode)&&(infoMenu.menu[infoMenu.cur]!=menuMode)&&
  return(infoMenu.menu[infoMenu.cur]!=menuStopMotor)&&(infoMenu.menu[infoMenu.cur]!=menuPrint)&&(infoMenu.menu[infoMenu.cur]!=menuTerminal)&&(infoMenu.menu[infoMenu.cur]!=menuPrinting)&&(infoMenu.menu[infoMenu.cur]!=menuStatus)&&((infoMenu.menu[infoMenu.cur]!=menuMove))&&(infoMenu.menu[infoMenu.cur]!=menuSendGcode);
}
#ifdef TEMP_LOAG

 #define TEMP_T_X        PEN_LOAG_X- 6*BYTE_WIDTH
 #define TEMP_BED_X      BED_LOAG_X- 6*BYTE_WIDTH
 #define TEMP_MINIICON_Y TEMP_LOAG_Y-3

 void temp_logo_display(void)
 {
 if(CheckMenuDis())
     {      
      GUI_DispDec(PEN_LOAG_X - 3*BYTE_WIDTH, TEMP_LOAG_Y, thermalManager.degHotend(0), 3, RIGHT);
      GUI_DispDec(BED_LOAG_X - 3*BYTE_WIDTH, TEMP_LOAG_Y, thermalManager.degBed(), 3, RIGHT);
      //GUI_DispString(PEN_LOAG_X- 6*BYTE_WIDTH, TEMP_LOAG_Y, (uint8_t *)"T0:"); 
      MINIICON_ReadDisplay(TEMP_T_X,TEMP_MINIICON_Y, MINI_ICON_T0);  
      GUI_DispString(PEN_LOAG_X, TEMP_LOAG_Y, (uint8_t *)"/"); // Ext value
      //GUI_DispString(BED_LOAG_X- 7*BYTE_WIDTH, TEMP_LOAG_Y, (uint8_t *)"Bed:");
      MINIICON_ReadDisplay(TEMP_BED_X,TEMP_MINIICON_Y, MINI_ICON_BED);  
      GUI_DispString(BED_LOAG_X, TEMP_LOAG_Y, (uint8_t *)"/"); // Bed value
      GUI_DispDec(PEN_LOAG_X + BYTE_WIDTH, TEMP_LOAG_Y, thermalManager.degTargetHotend(0), 3, LEFT);
      GUI_DispDec(BED_LOAG_X + BYTE_WIDTH, TEMP_LOAG_Y, thermalManager.degTargetBed(), 3, LEFT);
     }
 }

 void loopTempScan(void)
{
  static int16_t curTempT = 0;
  static int16_t curTempB = 0;
  static uint32_t nowTime_ms = 0;
  static int16_t curT0Tag=10000; 
  static int16_t curBTag=10000; 

  if (millis() - nowTime_ms > 1000) { // Refresh per 1 sec
    nowTime_ms = millis();
   
    if(curTempT != thermalManager.degHotend(0))
    {      
      curTempT = thermalManager.degHotend(0);
     GUI_DispDec(PEN_LOAG_X - 3*BYTE_WIDTH, TEMP_LOAG_Y, curTempT, 3, RIGHT);
    }
    if(curTempB != thermalManager.degBed())
    {      
      curTempB = thermalManager.degBed();
      GUI_DispDec(BED_LOAG_X - 3*BYTE_WIDTH, TEMP_LOAG_Y, curTempB, 3, RIGHT);
    }

    if(curT0Tag!=thermalManager.degTargetHotend(0))
    {
      curT0Tag=thermalManager.degTargetHotend(0);
      GUI_DispDec(PEN_LOAG_X + BYTE_WIDTH, TEMP_LOAG_Y, thermalManager.degTargetHotend(0), 3, LEFT);
    }
    if(curBTag!=thermalManager.degTargetBed())
    {
      curBTag=thermalManager.degTargetBed();
      GUI_DispDec(BED_LOAG_X + BYTE_WIDTH, TEMP_LOAG_Y, thermalManager.degTargetBed(), 3, LEFT);
    }
  }
  
}
#endif

void loopFrontEnd(void)
{
  loopReminderClear(); // If there is a message in the status bar, timed clear
  loopVolumeReminderClear();
}


void setMenu(MENU_TYPE menu_type, LABEL * title, int16_t rectCount, const GUI_RECT * menuRect, void(*action_redraw)(uint8_t position, uint8_t is_press), void (* menu_redraw)(void))
{
  menuType = menu_type;
  curRect = menuRect;
  curRectCount = rectCount;
  curTitle = title;
  TSC_ReDrawIcon = action_redraw;
  curMenuRedrawHandle = menu_redraw;
}



#endif


