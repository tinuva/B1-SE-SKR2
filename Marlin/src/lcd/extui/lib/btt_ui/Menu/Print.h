#ifndef _PRINT_H_
#define _PRINT_H_

#include "../../../../../inc/MarlinConfigPre.h"
#if ENABLED(BTT_UI_SPI)
enum
{
  LIST_MODE=0,
  ICON_MODE,
  FILE_MODE_COUNT  
};

void menuPrintFromSource(void);
void menuPrint(void);
#endif

#endif
