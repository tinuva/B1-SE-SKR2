#include "../TSC_Menu.h"
#include "../../../../../inc/MarlinConfig.h"

#if ENABLED(BTT_UI_SPI)

#ifndef _INTERFACECMD_H_
#define _INTERFACECMD_H_


#define CMD_MAX_LIST 20
#define CMD_MAX_CHAR 48

typedef struct
{
  char gcode[CMD_MAX_CHAR];
  uint8_t src;   // 0: TouchScreen Cmd, 1: Serial Port 2 rx Cmd, 2: Serial Port 3 rx Cmd
}GCODE;

typedef struct
{
  GCODE   queue[CMD_MAX_LIST];
  uint8_t index_r; // Ring buffer read position
  uint8_t index_w; // Ring buffer write position
  uint8_t count;   // Count of commands in the queue
}GCODE_QUEUE;

extern GCODE_QUEUE infoCmd;

void sendQueueCmd(void);

#endif
#endif
