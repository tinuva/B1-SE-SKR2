#include "../../../../../inc/MarlinConfigPre.h"
#if ENABLED(BTT_UI_SPI)
#include "../TSC_Menu.h"

//#define TRAM 48
#define TERMINAL_MAX_CHAR 100//NOBEYOND(600, TRAM*50, 4800)
#define MAX_BUFF          20
#define SCROLL_LINE       22
#define SCROLL_PAGE       1

#define CURSOR_START_X 0
#define CURSOR_END_X   LCD_WIDTH_PIXEL
#define CURSOR_START_Y 0
#define CURSOR_END_Y   CTRL_Y0

#define PAGE_ROW_NUM 1
#define PAGE_COL_NUM 5
#define PAGE_HEIGHT  GKEY_HEIGHT
#define PAGE_WIDTH   (LCD_WIDTH_PIXEL / PAGE_COL_NUM)
#define PAGE_X0      0
#define PAGE_Y0      CURSOR_END_Y
// typedef struct
// {
//   char * ptr[MAX_BUFF];  // Pointer into the terminal page buffer, full Screen is one page
//   uint8_t pageHead;      // Buffer top of page
//   uint8_t pageTail;      // Buffer buttom of page
//   uint8_t oldPageHead;
//   uint8_t pageIndex;  // page buffer index
//   uint8_t oldPageIndex;
// } TERMINAL_PAGE;


char * ptr[MAX_BUFF]; 
uint8_t pageHead; 
uint8_t pageTail; 
uint8_t oldPageHead;
uint8_t pageIndex; 
uint8_t oldPageIndex;


CHAR_INFO BTT_info;
uint16_t BTT_lastTerminalIndex = 0;
uint8_t BTT_pageBufIndex = 0;
int16_t BTT_cursorX = CURSOR_START_X;
int16_t BTT_cursorY = CURSOR_START_Y;
char BTT_pageNum[10];

#define TERMINAL_KEYBOARD_COLOR_LAYOUT 2


#define BAR_FONT_COLOR       WHITE
#define BAR_BG_COLOR         0x2174
#define BAR_BORDER_COLOR     0x4b0d
#define BAR_VALUE_FONT_COLOR BLACK
#define BAR_VALUE_BG_COLOR   0xFFF2

#define TERM_FONT_COLOR KEY_FONT_COLOR//infoSettings.marlin_mode_font_color
#define TERM_BG_COLOR   KEY_BG_COLOR//infoSettings.marlin_mode_bg_color


#define LAYOUT_QWERTY 1
#define LAYOUT_QWERTZ 2
#define LAYOUT_AZERTY 3

#define GKEY_WIDTH  (LCD_WIDTH_PIXEL / GKEY_COL_NUM)
#define GKEY_HEIGHT (LCD_HEIGHT_PIXEL / GKEY_ROW_NUM)

// gcode bar sizes
#define GCODE_ROW_NUM 1
#define GCODE_COL_NUM 4
#define GCODE_HEIGHT  (GKEY_HEIGHT / GCODE_ROW_NUM)
#define GCODE_WIDTH   (LCD_WIDTH_PIXEL / GCODE_COL_NUM)
#define GCODE_X0      0
#define GCODE_Y0      0

// keyboard sizes
#define KEY_ROW_NUM GKEY_ROW_NUM - 2
#define KEY_COL_NUM GKEY_COL_NUM
#define KEY_HEIGHT  GKEY_HEIGHT
#define KEY_WIDTH   GKEY_WIDTH
#define KEY_X0      0
#define KEY_Y0      GKEY_HEIGHT

// control bar sizes
#define CTRL_ROW_NUM 1

#if defined(HAS_ABC_KEY)
  typedef enum
  {
    SOFT_KEY_123 = 0,
    SOFT_KEY_ABC,
  } SOFT_KEY_TYPE;

  SOFT_KEY_TYPE gcodeKeyType = SOFT_KEY_123;

  #define GKEY_TYPING_KEYS GKEY_ABC_123
  #define CTRL_COL_NUM     4
#else
  #define GKEY_TYPING_KEYS GKEY_SPACE
  #define CTRL_COL_NUM     3
#endif

#define CTRL_HEIGHT  GKEY_HEIGHT
#define CTRL_WIDTH   (LCD_WIDTH_PIXEL / CTRL_COL_NUM)
#define CTRL_X0      0
#define CTRL_Y0      (LCD_HEIGHT_PIXEL - GKEY_HEIGHT)

// gcode value rectangles
const GUI_RECT gcodeValueRect = {
  GCODE_X0 + 0 * GCODE_WIDTH + 4, GCODE_Y0 + 0 * GCODE_HEIGHT + 4, GCODE_X0 + 3 * GCODE_WIDTH - 4, GCODE_Y0 + 1 * GCODE_HEIGHT - 4
};

// keyboard rectangles
const GUI_RECT gcodeKeyRect[GKEY_KEY_NUM]={
  // row 1
  {KEY_X0 + 0 * KEY_WIDTH, KEY_Y0 + 0 * KEY_HEIGHT, KEY_X0 + 1 * KEY_WIDTH, KEY_Y0 + 1 * KEY_HEIGHT},
  {KEY_X0 + 1 * KEY_WIDTH, KEY_Y0 + 0 * KEY_HEIGHT, KEY_X0 + 2 * KEY_WIDTH, KEY_Y0 + 1 * KEY_HEIGHT},
  {KEY_X0 + 2 * KEY_WIDTH, KEY_Y0 + 0 * KEY_HEIGHT, KEY_X0 + 3 * KEY_WIDTH, KEY_Y0 + 1 * KEY_HEIGHT},
  {KEY_X0 + 3 * KEY_WIDTH, KEY_Y0 + 0 * KEY_HEIGHT, KEY_X0 + 4 * KEY_WIDTH, KEY_Y0 + 1 * KEY_HEIGHT},
  {KEY_X0 + 4 * KEY_WIDTH, KEY_Y0 + 0 * KEY_HEIGHT, KEY_X0 + 5 * KEY_WIDTH, KEY_Y0 + 1 * KEY_HEIGHT},
  {KEY_X0 + 5 * KEY_WIDTH, KEY_Y0 + 0 * KEY_HEIGHT, KEY_X0 + 6 * KEY_WIDTH, KEY_Y0 + 1 * KEY_HEIGHT},
#if GKEY_COL_NUM > LAYOUT_1_COL_NUM
  {KEY_X0 + 6 * KEY_WIDTH, KEY_Y0 + 0 * KEY_HEIGHT, KEY_X0 + 7 * KEY_WIDTH, KEY_Y0 + 1 * KEY_HEIGHT},
#endif
#if GKEY_COL_NUM > LAYOUT_2_COL_NUM
  {KEY_X0 + 7 * KEY_WIDTH, KEY_Y0 + 0 * KEY_HEIGHT, KEY_X0 + 8 * KEY_WIDTH, KEY_Y0 + 1 * KEY_HEIGHT},
  {KEY_X0 + 8 * KEY_WIDTH, KEY_Y0 + 0 * KEY_HEIGHT, KEY_X0 + 9 * KEY_WIDTH, KEY_Y0 + 1 * KEY_HEIGHT},
  {KEY_X0 + 9 * KEY_WIDTH, KEY_Y0 + 0 * KEY_HEIGHT, KEY_X0 + 10 * KEY_WIDTH, KEY_Y0 + 1 * KEY_HEIGHT},
#endif

  // row 2
  {KEY_X0 + 0 * KEY_WIDTH, KEY_Y0 + 1 * KEY_HEIGHT, KEY_X0 + 1 * KEY_WIDTH, KEY_Y0 + 2 * KEY_HEIGHT},
  {KEY_X0 + 1 * KEY_WIDTH, KEY_Y0 + 1 * KEY_HEIGHT, KEY_X0 + 2 * KEY_WIDTH, KEY_Y0 + 2 * KEY_HEIGHT},
  {KEY_X0 + 2 * KEY_WIDTH, KEY_Y0 + 1 * KEY_HEIGHT, KEY_X0 + 3 * KEY_WIDTH, KEY_Y0 + 2 * KEY_HEIGHT},
  {KEY_X0 + 3 * KEY_WIDTH, KEY_Y0 + 1 * KEY_HEIGHT, KEY_X0 + 4 * KEY_WIDTH, KEY_Y0 + 2 * KEY_HEIGHT},
  {KEY_X0 + 4 * KEY_WIDTH, KEY_Y0 + 1 * KEY_HEIGHT, KEY_X0 + 5 * KEY_WIDTH, KEY_Y0 + 2 * KEY_HEIGHT},
  {KEY_X0 + 5 * KEY_WIDTH, KEY_Y0 + 1 * KEY_HEIGHT, KEY_X0 + 6 * KEY_WIDTH, KEY_Y0 + 2 * KEY_HEIGHT},
#if GKEY_COL_NUM > LAYOUT_1_COL_NUM
  {KEY_X0 + 6 * KEY_WIDTH, KEY_Y0 + 1 * KEY_HEIGHT, KEY_X0 + 7 * KEY_WIDTH, KEY_Y0 + 2 * KEY_HEIGHT},
#endif
#if GKEY_COL_NUM > LAYOUT_2_COL_NUM
  {KEY_X0 + 7 * KEY_WIDTH, KEY_Y0 + 1 * KEY_HEIGHT, KEY_X0 + 8 * KEY_WIDTH, KEY_Y0 + 2 * KEY_HEIGHT},
  {KEY_X0 + 8 * KEY_WIDTH, KEY_Y0 + 1 * KEY_HEIGHT, KEY_X0 + 9 * KEY_WIDTH, KEY_Y0 + 2 * KEY_HEIGHT},
  {KEY_X0 + 9 * KEY_WIDTH, KEY_Y0 + 1 * KEY_HEIGHT, KEY_X0 + 10 * KEY_WIDTH, KEY_Y0 + 2 * KEY_HEIGHT},
#endif

  // row 3
  {KEY_X0 + 0 * KEY_WIDTH, KEY_Y0 + 2 * KEY_HEIGHT, KEY_X0 + 1 * KEY_WIDTH, KEY_Y0 + 3 * KEY_HEIGHT},
  {KEY_X0 + 1 * KEY_WIDTH, KEY_Y0 + 2 * KEY_HEIGHT, KEY_X0 + 2 * KEY_WIDTH, KEY_Y0 + 3 * KEY_HEIGHT},
  {KEY_X0 + 2 * KEY_WIDTH, KEY_Y0 + 2 * KEY_HEIGHT, KEY_X0 + 3 * KEY_WIDTH, KEY_Y0 + 3 * KEY_HEIGHT},
  {KEY_X0 + 3 * KEY_WIDTH, KEY_Y0 + 2 * KEY_HEIGHT, KEY_X0 + 4 * KEY_WIDTH, KEY_Y0 + 3 * KEY_HEIGHT},
  {KEY_X0 + 4 * KEY_WIDTH, KEY_Y0 + 2 * KEY_HEIGHT, KEY_X0 + 5 * KEY_WIDTH, KEY_Y0 + 3 * KEY_HEIGHT},
  {KEY_X0 + 5 * KEY_WIDTH, KEY_Y0 + 2 * KEY_HEIGHT, KEY_X0 + 6 * KEY_WIDTH, KEY_Y0 + 3 * KEY_HEIGHT},
#if GKEY_COL_NUM > LAYOUT_1_COL_NUM
  {KEY_X0 + 6 * KEY_WIDTH, KEY_Y0 + 2 * KEY_HEIGHT, KEY_X0 + 7 * KEY_WIDTH, KEY_Y0 + 3 * KEY_HEIGHT},
#endif
#if GKEY_COL_NUM > LAYOUT_2_COL_NUM
  {KEY_X0 + 7 * KEY_WIDTH, KEY_Y0 + 2 * KEY_HEIGHT, KEY_X0 + 8 * KEY_WIDTH, KEY_Y0 + 3 * KEY_HEIGHT},
  {KEY_X0 + 8 * KEY_WIDTH, KEY_Y0 + 2 * KEY_HEIGHT, KEY_X0 + 9 * KEY_WIDTH, KEY_Y0 + 3 * KEY_HEIGHT},
  {KEY_X0 + 9 * KEY_WIDTH, KEY_Y0 + 2 * KEY_HEIGHT, KEY_X0 + 10 * KEY_WIDTH, KEY_Y0 + 3 * KEY_HEIGHT},
#endif

  // row 4
  {KEY_X0 + 0 * KEY_WIDTH, KEY_Y0 + 3 * KEY_HEIGHT, KEY_X0 + 1 * KEY_WIDTH, KEY_Y0 + 4 * KEY_HEIGHT},
  {KEY_X0 + 1 * KEY_WIDTH, KEY_Y0 + 3 * KEY_HEIGHT, KEY_X0 + 2 * KEY_WIDTH, KEY_Y0 + 4 * KEY_HEIGHT},
  {KEY_X0 + 2 * KEY_WIDTH, KEY_Y0 + 3 * KEY_HEIGHT, KEY_X0 + 3 * KEY_WIDTH, KEY_Y0 + 4 * KEY_HEIGHT},
  {KEY_X0 + 3 * KEY_WIDTH, KEY_Y0 + 3 * KEY_HEIGHT, KEY_X0 + 4 * KEY_WIDTH, KEY_Y0 + 4 * KEY_HEIGHT},
  {KEY_X0 + 4 * KEY_WIDTH, KEY_Y0 + 3 * KEY_HEIGHT, KEY_X0 + 5 * KEY_WIDTH, KEY_Y0 + 4 * KEY_HEIGHT},
  {KEY_X0 + 5 * KEY_WIDTH, KEY_Y0 + 3 * KEY_HEIGHT, KEY_X0 + 6 * KEY_WIDTH, KEY_Y0 + 4 * KEY_HEIGHT},
#if GKEY_COL_NUM > LAYOUT_1_COL_NUM
  {KEY_X0 + 6 * KEY_WIDTH, KEY_Y0 + 3 * KEY_HEIGHT, KEY_X0 + 7 * KEY_WIDTH, KEY_Y0 + 4 * KEY_HEIGHT},
#endif
#if GKEY_COL_NUM > LAYOUT_2_COL_NUM
  {KEY_X0 + 7 * KEY_WIDTH, KEY_Y0 + 3 * KEY_HEIGHT, KEY_X0 + 8 * KEY_WIDTH, KEY_Y0 + 4 * KEY_HEIGHT},
  {KEY_X0 + 8 * KEY_WIDTH, KEY_Y0 + 3 * KEY_HEIGHT, KEY_X0 + 9 * KEY_WIDTH, KEY_Y0 + 4 * KEY_HEIGHT},
  {KEY_X0 + 9 * KEY_WIDTH, KEY_Y0 + 3 * KEY_HEIGHT, KEY_X0 + 10 * KEY_WIDTH, KEY_Y0 + 4 * KEY_HEIGHT},
#endif

#if GKEY_COL_NUM > LAYOUT_2_COL_NUM
  // row 5
  {KEY_X0 + 0 * KEY_WIDTH, KEY_Y0 + 4 * KEY_HEIGHT, KEY_X0 + 1 * KEY_WIDTH, KEY_Y0 + 5 * KEY_HEIGHT},
  {KEY_X0 + 1 * KEY_WIDTH, KEY_Y0 + 4 * KEY_HEIGHT, KEY_X0 + 2 * KEY_WIDTH, KEY_Y0 + 5 * KEY_HEIGHT},
  {KEY_X0 + 2 * KEY_WIDTH, KEY_Y0 + 4 * KEY_HEIGHT, KEY_X0 + 3 * KEY_WIDTH, KEY_Y0 + 5 * KEY_HEIGHT},
  {KEY_X0 + 3 * KEY_WIDTH, KEY_Y0 + 4 * KEY_HEIGHT, KEY_X0 + 4 * KEY_WIDTH, KEY_Y0 + 5 * KEY_HEIGHT},
  {KEY_X0 + 4 * KEY_WIDTH, KEY_Y0 + 4 * KEY_HEIGHT, KEY_X0 + 5 * KEY_WIDTH, KEY_Y0 + 5 * KEY_HEIGHT},
  {KEY_X0 + 5 * KEY_WIDTH, KEY_Y0 + 4 * KEY_HEIGHT, KEY_X0 + 6 * KEY_WIDTH, KEY_Y0 + 5 * KEY_HEIGHT},
  {KEY_X0 + 6 * KEY_WIDTH, KEY_Y0 + 4 * KEY_HEIGHT, KEY_X0 + 7 * KEY_WIDTH, KEY_Y0 + 5 * KEY_HEIGHT},
  {KEY_X0 + 7 * KEY_WIDTH, KEY_Y0 + 4 * KEY_HEIGHT, KEY_X0 + 8 * KEY_WIDTH, KEY_Y0 + 5 * KEY_HEIGHT},
  {KEY_X0 + 8 * KEY_WIDTH, KEY_Y0 + 4 * KEY_HEIGHT, KEY_X0 + 9 * KEY_WIDTH, KEY_Y0 + 5 * KEY_HEIGHT},
  {KEY_X0 + 9 * KEY_WIDTH, KEY_Y0 + 4 * KEY_HEIGHT, KEY_X0 + 10 * KEY_WIDTH, KEY_Y0 + 5 * KEY_HEIGHT},

  // row 6
  {KEY_X0 + 0 * KEY_WIDTH, KEY_Y0 + 5 * KEY_HEIGHT, KEY_X0 + 1 * KEY_WIDTH, KEY_Y0 + 6 * KEY_HEIGHT},
  {KEY_X0 + 1 * KEY_WIDTH, KEY_Y0 + 5 * KEY_HEIGHT, KEY_X0 + 2 * KEY_WIDTH, KEY_Y0 + 6 * KEY_HEIGHT},
  {KEY_X0 + 2 * KEY_WIDTH, KEY_Y0 + 5 * KEY_HEIGHT, KEY_X0 + 3 * KEY_WIDTH, KEY_Y0 + 6 * KEY_HEIGHT},
  {KEY_X0 + 3 * KEY_WIDTH, KEY_Y0 + 5 * KEY_HEIGHT, KEY_X0 + 4 * KEY_WIDTH, KEY_Y0 + 6 * KEY_HEIGHT},
  {KEY_X0 + 4 * KEY_WIDTH, KEY_Y0 + 5 * KEY_HEIGHT, KEY_X0 + 5 * KEY_WIDTH, KEY_Y0 + 6 * KEY_HEIGHT},
  {KEY_X0 + 5 * KEY_WIDTH, KEY_Y0 + 5 * KEY_HEIGHT, KEY_X0 + 6 * KEY_WIDTH, KEY_Y0 + 6 * KEY_HEIGHT},
  {KEY_X0 + 6 * KEY_WIDTH, KEY_Y0 + 5 * KEY_HEIGHT, KEY_X0 + 7 * KEY_WIDTH, KEY_Y0 + 6 * KEY_HEIGHT},
  {KEY_X0 + 7 * KEY_WIDTH, KEY_Y0 + 5 * KEY_HEIGHT, KEY_X0 + 8 * KEY_WIDTH, KEY_Y0 + 6 * KEY_HEIGHT},
  {KEY_X0 + 8 * KEY_WIDTH, KEY_Y0 + 5 * KEY_HEIGHT, KEY_X0 + 9 * KEY_WIDTH, KEY_Y0 + 6 * KEY_HEIGHT},
  {KEY_X0 + 9 * KEY_WIDTH, KEY_Y0 + 5 * KEY_HEIGHT, KEY_X0 + 10 * KEY_WIDTH, KEY_Y0 + 6 * KEY_HEIGHT},
#endif

  // row control bar
#if GKEY_COL_NUM < LAYOUT_3_COL_NUM
  {CTRL_X0 + 0 * CTRL_WIDTH, CTRL_Y0 + 0 * CTRL_HEIGHT, CTRL_X0 + 1 * CTRL_WIDTH, CTRL_Y0 + 1 * CTRL_HEIGHT},          // ABC or 123
  {CTRL_X0 + 1 * CTRL_WIDTH, CTRL_Y0 + 0 * CTRL_HEIGHT, CTRL_X0 + 2 * CTRL_WIDTH, CTRL_Y0 + 1 * CTRL_HEIGHT},          // Space
  {CTRL_X0 + 2 * CTRL_WIDTH, CTRL_Y0 + 0 * CTRL_HEIGHT, CTRL_X0 + 3 * CTRL_WIDTH, CTRL_Y0 + 1 * CTRL_HEIGHT},          // Del
  {CTRL_X0 + 3 * CTRL_WIDTH, CTRL_Y0 + 0 * CTRL_HEIGHT, CTRL_X0 + 4 * CTRL_WIDTH, CTRL_Y0 + 1 * CTRL_HEIGHT},          // Back
#else
  {CTRL_X0 + 0 * CTRL_WIDTH, CTRL_Y0 + 0 * CTRL_HEIGHT, CTRL_X0 + 1 * CTRL_WIDTH, CTRL_Y0 + 1 * CTRL_HEIGHT},          // Space
  {CTRL_X0 + 1 * CTRL_WIDTH, CTRL_Y0 + 0 * CTRL_HEIGHT, CTRL_X0 + 2 * CTRL_WIDTH, CTRL_Y0 + 1 * CTRL_HEIGHT},          // Del
  {CTRL_X0 + 2 * CTRL_WIDTH, CTRL_Y0 + 0 * CTRL_HEIGHT, CTRL_X0 + 3 * CTRL_WIDTH, CTRL_Y0 + 1 * CTRL_HEIGHT},          // Back
#endif

  // row gcode bar
  {GCODE_X0 + 3 * GCODE_WIDTH, GCODE_Y0 + 0 * GCODE_HEIGHT, LCD_WIDTH_PIXEL, GCODE_Y0 + 1 * GCODE_HEIGHT},                   // Send
};

// keyboard keys
const char * const gcodeKey[][GKEY_KEY_NUM] = {
  {
#if GKEY_COL_NUM == LAYOUT_1_COL_NUM
    "1", "2", "3", "M", "G", "T",
    "4", "5", "6", "X", "Y", "Z",
    "7", "8", "9", "E", "F", "R",
    ".", "0", "-", "~", "S", "V",
#elif GKEY_COL_NUM == LAYOUT_2_COL_NUM
    "1", "2", "3", "M", "G", "T", "V",
    "4", "5", "6", "X", "Y", "Z", "S",
    "7", "8", "9", "E", "F", "R", "Q",
    ".", "0", "-", "~", "I", "J", "P",
#else
  #if TERMINAL_KEYBOARD_LAYOUT == LAYOUT_QWERTY
    "\\","|", "!", "\"","$", "%", "&", "/", "=", "?",
    "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
    "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P",
    "A", "S", "D", "F", "G", "H", "J", "K", "L", "'",
    "Z", "X", "C", "V", "B", "N", "M", ",", ".", ";",
    ":", "_", "@", "#", "~", "-", "+", "*", "(", ")",
  #elif TERMINAL_KEYBOARD_LAYOUT == LAYOUT_QWERTZ
    "!", "\"", "$", "%", "&", "/", "(", ")", "=", "?",
    "1", "2", "3", "4", "5", "6", "7", "8", "9", "0",
    "Q", "W", "E", "R", "T", "Z", "U", "I", "O", "P",
    "A", "S", "D", "F", "G", "H", "J", "K", "L", "@",
    "Y", "X", "C", "V", "B", "N", "M", ",", ".", "-",
    "|", ";", ":", "_", "#", "~", "+", "*", "'", "\\",
  #elif TERMINAL_KEYBOARD_LAYOUT == LAYOUT_AZERTY
    "#", "@", "~", "&", "(", ")", "_", "'", "\"", "%",
    "1", "2", "3", "4", "5", "6", "7", "8", "9", "0",
    "A", "Z", "E", "R", "T", "Y", "U", "I", "O", "P",
    "Q", "S", "D", "F", "G", "H", "J", "K", "L", "M",
    "W", "X", "C", "V", "B", "N", ".", ",", ":", ";",
    "-", "+", "*", "\\", "|", "/", "?","!", "$", "=",
  #else
    "1", "2", "3", "A", "B", "C", "D", "E", "F", "G",
    "4", "5", "6", "H", "I", "J", "K", "L", "M", "N",
    "7", "8", "9", "O", "P", "Q", "R", "S", "T", "U",
    ".", "0", "-", "~", "V", "W", "X", "Y", "Z", "'",
    "+", "*", "?", "!", "#", "&", "$", ",", ";", ":",
    "/", "=", "(", ")", "@", "_", "%", "|", "\"","\\",
  #endif
#endif

#if defined(HAS_ABC_KEY)
    "ABC",
#endif

    "Space", "Del", "Back",
    "Send",
  },
  {
#if GKEY_COL_NUM == LAYOUT_1_COL_NUM
    "A", "B", "C", "D", "H", "I",
    "J", "K", "L", "N", "O", "P",
    ",", ";", ":", "Q", "U", "W",
    "+", "*", "?", "!", "#", "&",
#elif GKEY_COL_NUM == LAYOUT_2_COL_NUM
    "A", "B", "C", "D", "H", "K", "L",
    ",", ";", ":", "N", "O", "U", "W",
    "+", "*", "?", "!", "#", "&", "$",
    "/", "=", "(", ")", "@", "_", "%",
#endif

#if defined(HAS_ABC_KEY)
    "123",
#endif

    "Space", "Del", "Back",
    "Send",
  }
};

// area rectangles
const GUI_RECT gcodeAreaRect[3] = {
  {GCODE_X0, GCODE_Y0, LCD_WIDTH_PIXEL, GKEY_HEIGHT},            // gcode area
  {KEY_X0, KEY_Y0, LCD_WIDTH_PIXEL, LCD_HEIGHT_PIXEL - GKEY_HEIGHT},   // keyboard area
  {CTRL_X0, CTRL_Y0, LCD_WIDTH_PIXEL, LCD_HEIGHT_PIXEL},               // control area
};

typedef enum
{
  TERM_PAGE_UP = 0,
  TERM_PAGE_DOWN,
  TERM_TOGGLE_ACK,
  TERM_BACK,
  TERM_KEY_NUM,  // number of keys
  TERM_IDLE = IDLE_TOUCH,
} TERMINAL_KEY_VALUES;



const GUI_RECT terminalPageRect = {
  PAGE_X0 + 0 * PAGE_WIDTH, PAGE_Y0 + 0 * PAGE_HEIGHT, PAGE_X0 + 1 * PAGE_WIDTH, PAGE_Y0 + 1 * PAGE_HEIGHT
};

const GUI_RECT terminalKeyRect[TERM_KEY_NUM] = {
  {PAGE_X0 + 1 * PAGE_WIDTH, PAGE_Y0 + 0 * PAGE_HEIGHT, PAGE_X0 + 2 * PAGE_WIDTH, PAGE_Y0 + 1 * PAGE_HEIGHT},
  {PAGE_X0 + 2 * PAGE_WIDTH, PAGE_Y0 + 0 * PAGE_HEIGHT, PAGE_X0 + 3 * PAGE_WIDTH, PAGE_Y0 + 1 * PAGE_HEIGHT},
  {PAGE_X0 + 3 * PAGE_WIDTH, PAGE_Y0 + 0 * PAGE_HEIGHT, PAGE_X0 + 4 * PAGE_WIDTH, PAGE_Y0 + 1 * PAGE_HEIGHT},
  {PAGE_X0 + 4 * PAGE_WIDTH, PAGE_Y0 + 0 * PAGE_HEIGHT, PAGE_X0 + 5 * PAGE_WIDTH, PAGE_Y0 + 1 * PAGE_HEIGHT},
};

const char * terminalKey[TERM_KEY_NUM] = {
  "<",
  ">",
  "ON",
  "Back"
};

const GUI_RECT terminalAreaRect[2] = {
  {CURSOR_START_X, CURSOR_START_Y, CURSOR_END_X, CURSOR_END_Y},  // terminal area
  {PAGE_X0, PAGE_Y0, LCD_WIDTH_PIXEL, LCD_HEIGHT_PIXEL},  // control area
};

char terminalBuf[TERMINAL_MAX_CHAR];
//TERMINAL_PAGE terminal_page = {terminalBuf, 0, 0, 0, 0, 0};
static uint16_t terminalBufTail = 0;
uint8_t buf_full = 0;

void sendGcodeReDrawButton(uint8_t index, uint8_t isPressed)
{
  if (index >= GKEY_KEY_NUM)
    return;

  if (isPressed)
  {
    if (index < GKEY_TYPING_KEYS)
    {
      GUI_SetColor(KEY_BG_COLOR);
      GUI_SetBkColor(KEY_FONT_COLOR);
    }
    else
    {
      GUI_SetColor(BAR_BG_COLOR);
      GUI_SetBkColor(BAR_FONT_COLOR);
    }
  }
  else
  {
    if (index < GKEY_TYPING_KEYS)
    {
      GUI_SetColor(KEY_FONT_COLOR);
      GUI_SetBkColor(KEY_BG_COLOR);
    }
    else
    {
      GUI_SetColor(BAR_FONT_COLOR);
      GUI_SetBkColor(BAR_BG_COLOR);
    }
  }

  if (index != GKEY_SEND)
    setLargeFont(true);

  GUI_ClearRect(gcodeKeyRect[index].x0 + 2, gcodeKeyRect[index].y0 + 2, gcodeKeyRect[index].x1 - 1,
                gcodeKeyRect[index].y1 - 1);

  #if defined(HAS_ABC_KEY)
    GUI_DispStringInRect(gcodeKeyRect[index].x0 + 2, gcodeKeyRect[index].y0 + 2, gcodeKeyRect[index].x1 - 1,
                         gcodeKeyRect[index].y1 - 1, (uint8_t *)gcodeKey[gcodeKeyType][index]);
  #else
    GUI_DispStringInRect(gcodeKeyRect[index].x0 + 2, gcodeKeyRect[index].y0 + 2, gcodeKeyRect[index].x1 - 1,
                         gcodeKeyRect[index].y1 - 1, (uint8_t *)gcodeKey[0][index]);
  #endif

    setLargeFont(false);
}
void sendGcodeDrawGcode(char *gcode)
{
  GUI_SetColor(BAR_VALUE_FONT_COLOR);
  GUI_SetBkColor(BAR_VALUE_BG_COLOR);

  GUI_ClearRect(gcodeValueRect.x0 + 1, gcodeValueRect.y0 + 1, gcodeValueRect.x1 - 1, gcodeValueRect.y1 - 1);

  if (gcode != NULL)
    GUI_DispStringInRect(gcodeValueRect.x0 + 1, gcodeValueRect.y0 + 1, gcodeValueRect.x1 - 1, gcodeValueRect.y1 - 1,
                         (uint8_t *)gcode);
}

void sendGcodeDrawKeyboard(void)
{
  GUI_SetColor(KEY_BORDER_COLOR);

  // draw vertical button borders
  for (int i = 0; i < (KEY_COL_NUM - 1); i++)
  {
    GUI_DrawLine(gcodeKeyRect[i].x1, gcodeKeyRect[i].y0, gcodeKeyRect[i + ((KEY_ROW_NUM - 1) * KEY_COL_NUM)].x1,
                 gcodeKeyRect[i + ((KEY_ROW_NUM - 1) * KEY_COL_NUM)].y1 - 1);
  }

  // draw horizontal button borders
  for (int i = 0; i < (KEY_ROW_NUM - 1); i++)
  {
    GUI_DrawLine(gcodeKeyRect[i * KEY_COL_NUM].x0, gcodeKeyRect[i * KEY_COL_NUM].y1,
                 gcodeKeyRect[(i * KEY_COL_NUM) + (KEY_COL_NUM - 1)].x1 - 1,
                 gcodeKeyRect[(i * KEY_COL_NUM) + (KEY_COL_NUM - 1)].y1);
  }

  // draw shadow border
  GUI_SetColor(KEY_BORDER_COLOR_2);
  GUI_DrawLine(gcodeAreaRect[1].x0, gcodeAreaRect[1].y0, gcodeAreaRect[1].x1, gcodeAreaRect[1].y0);
  GUI_DrawLine(gcodeAreaRect[1].x0, gcodeAreaRect[1].y1 - 1, gcodeAreaRect[1].x1, gcodeAreaRect[1].y1 - 1);

  for (uint8_t i = 0; i < COUNT(gcodeKey[0]); i++)
  {
    sendGcodeReDrawButton(i, false);
  }
}

void sendGcodeDrawMenu(void)
{
  setMenu(MENU_TYPE_FULLSCREEN, NULL, COUNT(gcodeKeyRect), gcodeKeyRect, sendGcodeReDrawButton, NULL);

  GUI_SetBkColor(BAR_BG_COLOR);

  // clear bar area
  GUI_ClearPrect(&gcodeAreaRect[0]);
  GUI_ClearPrect(&gcodeAreaRect[2]);

  GUI_SetBkColor(KEY_BG_COLOR);

  // clear keyboard area
  GUI_ClearPrect(&gcodeAreaRect[1]);

  GUI_SetColor(BAR_BORDER_COLOR);

  // draw gcode value area border
  GUI_DrawPrect(&gcodeValueRect);

  // draw bar area shadow border
  GUI_DrawLine(gcodeAreaRect[0].x0, gcodeAreaRect[0].y1 - 1, gcodeAreaRect[0].x1, gcodeAreaRect[0].y1 - 1);
  GUI_DrawLine(gcodeAreaRect[2].x0, gcodeAreaRect[2].y0, gcodeAreaRect[2].x1, gcodeAreaRect[2].y0);

  GUI_SetTextMode(GUI_TEXTMODE_TRANS);

  // draw keyboard and gcode value
  sendGcodeDrawKeyboard();
  sendGcodeDrawGcode(NULL);
}

//#define BTT_CMD_MAX_CHAR 100
char BTT_gcodeBuf[50] = {0};
uint8_t BTT_nowIndex = 0;
uint8_t BTT_lastIndex = 0;

void menuCallBackSendGcode()
{
  //static char gcodeBuf[CMD_MAX_CHAR] = {0};

  GKEY_VALUES key_num = GKEY_IDLE;


  key_num = (GKEY_VALUES)menuKeyGetValue();

    switch (key_num)
    {
      case GKEY_IDLE:
        break;

      case GKEY_BACK:
        GUI_RestoreColorDefault();
        infoMenu.cur--;
        break;

      case GKEY_SEND:
        if (BTT_nowIndex)
        {
          // storeCmd(BTT_gcodeBuf);
          queue.enqueue_one_now(BTT_gcodeBuf);
        }

        infoMenu.menu[++infoMenu.cur] = menuTerminal;
        break;

      #if defined(HAS_ABC_KEY)
        case GKEY_ABC_123:
          gcodeKeyType = (gcodeKeyType == SOFT_KEY_123) ? SOFT_KEY_ABC : SOFT_KEY_123;
          sendGcodeDrawKeyboard();
          break;
      #endif

      case GKEY_DEL:
        if (BTT_nowIndex)
          BTT_gcodeBuf[--BTT_nowIndex] = 0;
        break;

      case GKEY_SPACE:
        if (BTT_nowIndex < CMD_MAX_CHAR - 1)
        {
          BTT_gcodeBuf[BTT_nowIndex++] = ' ';
          BTT_gcodeBuf[BTT_nowIndex] = 0;
        }
        break;

      default:
        if (BTT_nowIndex < CMD_MAX_CHAR - 1)
        {
          #if defined(HAS_ABC_KEY)
            BTT_gcodeBuf[BTT_nowIndex++] = gcodeKey[gcodeKeyType][key_num][0];
          #else
            BTT_gcodeBuf[BTT_nowIndex++] = gcodeKey[0][key_num][0];
          #endif
          BTT_gcodeBuf[BTT_nowIndex] = 0;
        }
       break;
    }

    if (BTT_lastIndex != BTT_nowIndex)
    {
      BTT_lastIndex = BTT_nowIndex;
      sendGcodeDrawGcode(BTT_gcodeBuf);
    }
   //loopBackEnd();
}

void menuSendGcode(void)
{
  sendGcodeDrawMenu();
  memset(BTT_gcodeBuf,0,sizeof(BTT_gcodeBuf));
  BTT_nowIndex = 0;
  BTT_lastIndex = 0;
  menuSetFrontCallBack(menuCallBackSendGcode);
    
  GUI_RestoreColorDefault();
}


void saveGcodeTerminalCache(uint16_t sign_len, char *str)
{
  uint16_t len = 0;

  if ((terminalBufTail + sign_len) < TERMINAL_MAX_CHAR)
  {
    memcpy(terminalBuf + terminalBufTail, str, sign_len);
    terminalBufTail += sign_len;
  }
  else
  {
    len = (terminalBufTail + sign_len) - TERMINAL_MAX_CHAR;
    memcpy(terminalBuf + terminalBufTail, str, (sign_len - len));
    terminalBufTail = 0;
    memcpy(terminalBuf + terminalBufTail, str + (sign_len - len), len);
    terminalBufTail += len;
    buf_full = 1;
    pageHead++;
  }
}

void sendGcodeTerminalCache(char *stream, TERMINAL_SRC src)
{
  uint16_t sign_len = 0;
  uint16_t stream_len = 0;
  const char *const terminalSign[] = {"Send: ", "Rcv: "};

  if (infoMenu.menu[infoMenu.cur] != menuSendGcode && infoMenu.menu[infoMenu.cur] != menuTerminal)
    return;

  sign_len = strlen(terminalSign[src]);
  saveGcodeTerminalCache(sign_len, (char *) terminalSign[src]);
  stream_len = strlen(stream);
  saveGcodeTerminalCache(stream_len, stream);
}

void terminalReDrawButton(uint8_t index, uint8_t isPressed)
{
  if (index >= TERM_KEY_NUM)
    return;

  if (isPressed)
  {
    GUI_SetColor(BAR_BG_COLOR);
    GUI_SetBkColor(BAR_FONT_COLOR);
  }
  else
  {
    GUI_SetColor(BAR_FONT_COLOR);
    GUI_SetBkColor(BAR_BG_COLOR);
  }

  setLargeFont(true);

  GUI_ClearRect(terminalKeyRect[index].x0 + 2, terminalKeyRect[index].y0 + 2, terminalKeyRect[index].x1 - 1,
                terminalKeyRect[index].y1 - 1);
  GUI_DispStringInRect(terminalKeyRect[index].x0 + 2, terminalKeyRect[index].y0 + 2, terminalKeyRect[index].x1 - 1,
                       terminalKeyRect[index].y1 - 1, (uint8_t *)terminalKey[index]);

  setLargeFont(false);
}

void terminalDrawPage(char *pageNum)
{
  GUI_SetColor(BAR_FONT_COLOR);
  GUI_SetBkColor(BAR_BG_COLOR);
  setLargeFont(true);

  GUI_ClearRect(terminalPageRect.x0 + 1, terminalPageRect.y0 + 1, terminalPageRect.x1 - 1, terminalPageRect.y1 - 1);

  if (pageNum != NULL)
    GUI_DispStringInRect(terminalPageRect.x0 + 1, terminalPageRect.y0 + 1, terminalPageRect.x1 - 1, terminalPageRect.y1 - 1,
                         (uint8_t *)pageNum);

  setLargeFont(false);
}

static uint16_t terminalACK=0;

void terminalDrawMenu(void)
{
  setMenu(MENU_TYPE_FULLSCREEN, NULL, COUNT(terminalKeyRect), terminalKeyRect, terminalReDrawButton, NULL);

  GUI_SetBkColor(TERM_BG_COLOR);

  //clear terminal area
  GUI_ClearPrect(&terminalAreaRect[0]);

  GUI_SetBkColor(BAR_BG_COLOR);

  // clear bar area
  GUI_ClearPrect(&terminalAreaRect[1]);

  GUI_SetColor(BAR_BORDER_COLOR);

  // draw bar area shadow border
  GUI_ClearPrect(&terminalAreaRect[1]);

  GUI_SetTextMode(GUI_TEXTMODE_TRANS);

  // init toggle ack value
  terminalKey[TERM_TOGGLE_ACK] = (const char *) textSelect(LABEL_ON);
  // draw keyboard
  for (uint8_t i = 0; i < COUNT(terminalKey); i++)
  {
    terminalReDrawButton(i, false);
  }

  // if (pageTail >= pageIndex)
  //     BTT_pageBufIndex =  pageTail - pageIndex;
  // else
  //     BTT_pageBufIndex = pageTail + (MAX_BUFF - pageIndex);

  //     BTT_lastTerminalIndex = ptr[BTT_pageBufIndex] - terminalBuf;

  //     BTT_cursorX = CURSOR_START_X;
  //     BTT_cursorY = CURSOR_START_Y;

  //     GUI_SetBkColor(TERM_BG_COLOR);

  //     GUI_ClearRect(CURSOR_START_X, CURSOR_START_Y, CURSOR_END_X, CURSOR_END_Y);
}



void menuCallBackTerminal()
{
  uint16_t key_num = IDLE_TOUCH;

    key_num = menuKeyGetValue();
    switch (key_num)
    {
      // case TERM_PAGE_UP:  // page up
      //   if (pageIndex < (pageTail - pageHead))
      //   {
      //     pageIndex += SCROLL_PAGE;
      //   }
      //   if ((pageTail < pageHead)
      //       && (pageIndex < (pageTail + MAX_BUFF - pageHead)))
      //   {
      //     pageIndex += SCROLL_PAGE;
      //   }
      //   break;

      // case TERM_PAGE_DOWN:  // page down
      //   if (pageIndex > 0)
      //     pageIndex -= SCROLL_PAGE;
      //   break;

      case TERM_TOGGLE_ACK:  // toggle ack in terminal
        terminalACK = (terminalACK + 1) % ITEM_TOGGLE_NUM;
        if(terminalACK)
        terminalKey[TERM_TOGGLE_ACK] = (const char *) textSelect(LABEL_ON);
        else
        terminalKey[TERM_TOGGLE_ACK] = (const char *) textSelect(LABEL_OFF);          
        
        terminalReDrawButton(TERM_TOGGLE_ACK, false);
        break;

      case TERM_BACK:  // back
        infoMenu.cur--;
        break;

      default:
        break;
    }
    getCharacterInfo((uint8_t *) &terminalBuf[BTT_lastTerminalIndex], &BTT_info);

    while ((terminalBuf[BTT_lastTerminalIndex]) && (BTT_lastTerminalIndex != terminalBufTail))
    {
      // Next Line
      if (BTT_cursorX + BTT_info.pixelWidth > CURSOR_END_X || (terminalBuf[BTT_lastTerminalIndex] == '\n' && BTT_cursorX != CURSOR_START_X))
      {
        BTT_cursorX = CURSOR_START_X;
        BTT_cursorY += BTT_info.pixelHeight;
      }

      if (terminalBuf[BTT_lastTerminalIndex] != '\n')
      {
        if (BTT_cursorY + BTT_info.pixelHeight > CURSOR_END_Y)  // Save the page pointer and scroll to a new screen
        {
          if (pageIndex != 0)
            break;

          pageTail = (pageTail + 1) % MAX_BUFF;
          // Save character buffer index to page buffer
          ptr[pageTail] = &terminalBuf[BTT_lastTerminalIndex];

          // Update the bottom of the page buffer to limit the page range
          if ((buf_full == 1) && (oldPageHead == pageHead))
            pageHead++;

          if (pageHead >= MAX_BUFF)
            pageHead = 0;

          oldPageHead = pageHead;
          BTT_cursorX = CURSOR_START_X;
          BTT_cursorY = CURSOR_START_Y;

          GUI_SetBkColor(TERM_BG_COLOR);
          GUI_ClearRect(CURSOR_START_X, CURSOR_START_Y, CURSOR_END_X, CURSOR_END_Y);

          sprintf(BTT_pageNum, "%d/%d", abs(((pageTail - pageHead) - pageIndex) + 1),
                  abs((pageTail - pageHead) + 1));

          terminalDrawPage(BTT_pageNum);
        }

        GUI_SetColor(TERM_FONT_COLOR);
        GUI_SetBkColor(TERM_BG_COLOR);
        GUI_DispOne(BTT_cursorX, BTT_cursorY, (uint8_t *) &terminalBuf[BTT_lastTerminalIndex]);
        BTT_cursorX += BTT_info.pixelWidth;
      }

      BTT_lastTerminalIndex += BTT_info.bytes;
      if (BTT_lastTerminalIndex >= TERMINAL_MAX_CHAR)
        BTT_lastTerminalIndex = 0;
    }

    //loopBackEnd();
}

void menuTerminal(void)
{
    terminalACK=1;
   
   terminalDrawMenu();
   menuSetFrontCallBack(menuCallBackTerminal);

    buf_full = 0;
    terminalBufTail = 0;
    terminalBuf[0] = 0;
    pageTail = 0;
    pageHead = 0;
    pageIndex = 0;
    oldPageIndex = 0;

  //  GUI_RestoreColorDefault();
}



#endif

