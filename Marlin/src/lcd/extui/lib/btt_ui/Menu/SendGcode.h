#ifndef _SENDGCODE_H_
#define _SENDGCODE_H_

#include "../../../../../inc/MarlinConfigPre.h"
#if ENABLED(BTT_UI_SPI)

#include "menu.h"
#define KEY_FONT_COLOR     WHITE
#define KEY_BG_COLOR       BLACK
#define KEY_BORDER_COLOR   0x2174
#define KEY_BORDER_COLOR_2 DARKGRAY

// keyboard layouts
#define LAYOUT_1_COL_NUM 6
#define LAYOUT_1_ROW_NUM 6

#define LAYOUT_2_COL_NUM 7
#define LAYOUT_2_ROW_NUM 6

#define LAYOUT_3_COL_NUM 10
#define LAYOUT_3_ROW_NUM 8

#if LCD_WIDTH < 480  // number of columns and rows is based on LCD display resolution
  #define GKEY_COL_NUM LAYOUT_1_COL_NUM
  #define GKEY_ROW_NUM LAYOUT_1_ROW_NUM
  #define HAS_ABC_KEY
#elif LCD_WIDTH < 800
  #define GKEY_COL_NUM LAYOUT_2_COL_NUM
  #define GKEY_ROW_NUM LAYOUT_2_ROW_NUM
  #define HAS_ABC_KEY
#else
  #define GKEY_COL_NUM LAYOUT_3_COL_NUM
  #define GKEY_ROW_NUM LAYOUT_3_ROW_NUM
#endif
#define HAS_ABC_KEY

#define GCODE_IDLE_TOUCH 0XFFFF
typedef enum
{
  TERMINAL_GCODE,
  TERMINAL_ACK,
} TERMINAL_SRC;

#define ITEM_TOGGLE_NUM         2
// Labels list for ON/OFF settings
// const LABEL itemToggle[ITEM_TOGGLE_NUM] =
// {
//   LABEL_OFF,
//   LABEL_ON
// };

typedef enum
{
  GKEY_0 = 0,
  GKEY_1,
  GKEY_2,
  GKEY_3,
  GKEY_4,
  GKEY_5,
  GKEY_6,
  GKEY_7,
  GKEY_8,
  GKEY_9,
  GKEY_10,
  GKEY_11,
  GKEY_12,
  GKEY_13,
  GKEY_14,
  GKEY_15,
  GKEY_16,
  GKEY_17,
  GKEY_18,
  GKEY_19,
  GKEY_20,
  GKEY_21,
  GKEY_22,
  GKEY_23,
  #if GKEY_COL_NUM > LAYOUT_1_COL_NUM
    GKEY_24,
    GKEY_25,
    GKEY_26,
    GKEY_27,
  #endif
  #if GKEY_COL_NUM > LAYOUT_2_COL_NUM
    GKEY_28,
    GKEY_29,
    GKEY_30,
    GKEY_31,
    GKEY_32,
    GKEY_33,
    GKEY_34,
    GKEY_35,
    GKEY_36,
    GKEY_37,
    GKEY_38,
    GKEY_39,
    GKEY_40,
    GKEY_41,
    GKEY_42,
    GKEY_43,
    GKEY_44,
    GKEY_45,
    GKEY_46,
    GKEY_47,
    GKEY_48,
    GKEY_49,
    GKEY_50,
    GKEY_51,
    GKEY_52,
    GKEY_53,
    GKEY_54,
    GKEY_55,
    GKEY_56,
    GKEY_57,
    GKEY_58,
    GKEY_59,
  #endif

  #if defined(HAS_ABC_KEY)
    GKEY_ABC_123,
  #endif

  GKEY_SPACE,
  GKEY_DEL,
  GKEY_BACK,
  GKEY_SEND,
  GKEY_KEY_NUM,  // number of keys
  GKEY_IDLE = GCODE_IDLE_TOUCH,
} GKEY_VALUES;


#define ITEM_DEGREE_NUM         3
#define ITEM_SPEED_NUM          3
#define ITEM_PERCENT_STEPS_NUM  3
//#define ITEM_MOVE_LEN_NUM       5
#define ITEM_FINE_MOVE_LEN_NUM  3
#define ITEM_EXT_LEN_NUM        5
#define ITEM_TOGGLE_NUM         2

void menuSendGcode(void);
void menuTerminal(void);
void sendGcodeTerminalCache(char *stream, TERMINAL_SRC src);
#endif
#endif
