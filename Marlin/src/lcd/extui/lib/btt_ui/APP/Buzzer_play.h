#ifndef _BUZZER_PLAY_H
#define _BUZZER_PLAY_H

#include "../../../../../inc/MarlinConfig.h"
#if ENABLED(BTT_UI_SPI)

typedef enum
{
  sound_success,
  sound_error,
  sound_ok,
  sound_cancel,
  sound_notify,
  sound_keypress,
}SOUND;
void Buzzer_play(SOUND sound);

#endif
#endif