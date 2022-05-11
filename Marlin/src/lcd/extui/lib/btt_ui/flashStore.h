#ifndef _FLASHSTORE_H_
#define _FLASHSTORE_H_

#include "../../../../inc/MarlinConfigPre.h"
#if ENABLED(BTT_UI_SPI)

#define PARA_SIZE 256  //bytes
#define TSC_SIGN  0x20211021 // DO NOT MODIFY
#define PARA_SIGN 0x20211021 // If a new setting parameter is added, modify here and initialize the initial value in the "infoSettingsReset()" function

extern uint8_t  BTT_SaveData[PARA_SIZE]; 
bool readStoredPara(void);
void storePara(void);

#endif

#endif
