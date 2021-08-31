#ifndef _MENU_H_
#define _MENU_H_

#include "../../../../../inc/MarlinConfigPre.h"
#if ENABLED(BTT_UI_SPI)

#include "stdint.h"
#include "stdbool.h"
#include "../GUI.h"
#include "SendGcode.h"

#define IDLE_TOUCH	0xFFFF
typedef enum
{
  KEY_ICON_0 = 0,
  KEY_ICON_1,
  KEY_ICON_2,
  KEY_ICON_3,
  KEY_ICON_4,
  KEY_ICON_5,
  KEY_ICON_6,
  KEY_ICON_7,
  KEY_LABEL_0,
  KEY_LABEL_1,
  KEY_LABEL_2,
  KEY_LABEL_3,
  KEY_LABEL_4,
  KEY_LABEL_5,
  KEY_LABEL_6,
  KEY_LABEL_7,
  KEY_TITLEBAR,
  KEY_INFOBOX,
  KEY_IDLE = IDLE_TOUCH,
}KEY_VALUES;

#define ITEM_PER_PAGE  8


typedef union
{
  uint32_t index;    // language index, address = textSelect(index);
  uint8_t *address;
}LABEL;

typedef struct
{
  uint16_t icon;
  LABEL label;
}ITEM;

typedef enum
{
  LIST_LABEL = 0,
  LIST_BUTTON,
}LISTITEM_TYPE;

typedef struct
{
  LISTITEM_TYPE type;
  uint8_t* icon;
  uint8_t* label;
}LISTITEM;

typedef struct
{
  LABEL title;
  //uint16_t titleIconChar;
  LISTITEM  items[ITEM_PER_PAGE];
}LISTITEMS;

typedef struct
{
  LABEL title;
  ITEM  items[ITEM_PER_PAGE];
}MENUITEMS;

// typedef struct
// {
//   ITEM  items[ITEM_PER_PAGE];
// }MENUITEMS;

typedef enum
{
  STATUS_IDLE = 0,
  STATUS_BUSY,
  STATUS_NORMAL
}SYS_STATUS;

typedef struct
{
  GUI_RECT rect;
  uint32_t time;
  uint8_t  status;
  int16_t inf;
}REMINDER;

typedef enum
{
  MENU_TYPE_ICON,
  MENU_TYPE_LISTVIEW,
  MENU_TYPE_DIALOG,
  MENU_TYPE_EDITOR,
  MENU_TYPE_FULLSCREEN,
  MENU_TYPE_OTHER,
} MENU_TYPE;

#define CHAR_FOLDER                (uint8_t *)"\u0862"
#define CHAR_FILE                  (uint8_t *)"\u0863"
#define CHAR_PAGEUP                (uint8_t *)"\u0867\u0868"
#define CHAR_PAGEDOWN              (uint8_t *)"\u0869\u086A"
#define CHAR_BACK                  (uint8_t *)"\u086B\u086C"

#define ITEM_PER_PAGE       8
#define MENU_RECT_COUNT     (ITEM_PER_PAGE*2 + 1) // 8 items + title bar
#define SS_RECT_COUNT       (ITEM_PER_PAGE*2 + 1 + 1) // 8 items + title bar + infobox
#define TM_RECT_COUNT       (ITEM_PER_PAGE*2 + 1 + 1) // 8 items + title bar + tempbox
#define LISTITEM_PER_PAGE   5
#define LIVEICON_LINES      3

extern const GUI_RECT exhibitRect;
extern const GUI_RECT rect_of_key[ITEM_PER_PAGE*2];
#define CENTER_Y  (int16_t)((exhibitRect.y1 - exhibitRect.y0)/2 + exhibitRect.y0)
#define CENTER_X  (int16_t)((exhibitRect.x1 - exhibitRect.x0 - BYTE_WIDTH)/2 + exhibitRect.x0)

uint8_t *labelGetAddress(const LABEL *label);
void reminderMessage(int16_t inf, SYS_STATUS status);
void volumeReminderMessage(int16_t inf, SYS_STATUS status);

void menuDrawListItem(const LISTITEM *item, uint8_t position);
void menuDrawItem (const ITEM *menuItem, uint8_t positon);
void menuDrawTitle(const uint8_t *content);
void menuDrawPage (const MENUITEMS *menuItems);

void menuDrawListPage(const LISTITEMS *listItems);
void itemDrawIconPress(uint8_t positon, uint8_t is_press);
KEY_VALUES menuKeyGetValue(void);
KEY_VALUES PrintmenuKeyGetValue(void);
KEY_VALUES GCODE_menuKeyGetValue(void);
void temp_logo_display(void);

void loopBackEnd(void);
void loopFrontEnd(void);
void loopProcess (void);
void loopTempScan(void);

bool CheckMenuDis();
void setMenu(MENU_TYPE menu_type, LABEL * title, int16_t rectCount, const GUI_RECT * menuRect, void(*action_redraw)(uint8_t position, uint8_t is_press), void (* menu_redraw)(void));

#define PEN_LOAG_X               (LCD_WIDTH_PIXEL/8)*6-2*BYTE_WIDTH
#define BED_LOAG_X               (LCD_WIDTH_PIXEL/8)*7+BYTE_WIDTH*3
#define TEMP_LOAG_Y              BYTE_HEIGHT-2

#endif

#endif
