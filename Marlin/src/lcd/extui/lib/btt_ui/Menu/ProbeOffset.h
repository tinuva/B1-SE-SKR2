#ifndef _PROBEOFFSET_H_
#define _PROBEOFFSET_H_
#include "../../../../../inc/MarlinConfigPre.h"
#if ENABLED(BTT_UI_SPI)
#include "stdint.h"

void menuProbeOffset(void);
void setCurrentOffset(float offset);
#endif

#endif

