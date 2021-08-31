#include "../../../../../inc/MarlinConfigPre.h"
#if ENABLED(BTT_UI_SPI)
#include "../TSC_Menu.h"
MENUITEMS RGBItems = {
// title
LABEL_RGB_SETTINGS,
// icon                       label
 {{ICON_RGB_RED,              LABEL_RGB_RED},
  {ICON_RGB_GREEN,            LABEL_RGB_GREEN},
  {ICON_RGB_BLUE,             LABEL_RGB_BLUE},
  {ICON_RGB_WHITE,            LABEL_RGB_WHITE},
  {ICON_RGB_OFF,              LABEL_RGB_OFF},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACK,                 LABEL_BACK},}
};

void menuCallBackRGBSettings(void)
{
  KEY_VALUES key_num = menuKeyGetValue();
  switch(key_num)
  {
    case KEY_ICON_0:  //Red
      storeCmd("M150 R255 U0 B0 P255");
      break;
    
    case KEY_ICON_1:  //Green
      storeCmd("M150 R0 U255 B0 P255");
      break;
    
    case KEY_ICON_2:  //Blue
      storeCmd("M150 R0 U0 B255 P255");
      break;
    
    case KEY_ICON_3:  //White
      storeCmd("M150 R255 U255 B255 P255");
      break;
    
    case KEY_ICON_4:  //Turn Off
      storeCmd("M150 R0 U0 B0 P0");
      break;
    
    case KEY_ICON_7:
      infoMenu.cur--;
      break;
    
    default:
      break;
  }
}

void menuRGBSettings(void)
{
  menuDrawPage(&RGBItems);
  menuSetFrontCallBack(menuCallBackRGBSettings);
}


MENUITEMS machineSettingsItems = {
// title
LABEL_MACHINE_SETTINGS,
// icon                       label
 {{ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_MOTOR_UNLOCK,         LABEL_MOTOR_UNLOCK},
  #ifdef Z_HOT_HOMING
  {ICON_ABLB2,                LABEL_HOT_HOMING_ON},
  #else
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  #endif
  {ICON_SHUT_DOWN,            LABEL_SHUT_DOWN},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_RGB_SETTINGS,         LABEL_RGB_SETTINGS},
  {ICON_BACK,                 LABEL_BACK},}
};

#ifdef FIL_RUNOUT_PIN
  #define ITEM_RUNOUT_NUM 2
  const ITEM itemRunout[ITEM_RUNOUT_NUM] = {
  // icon                       label
    {ICON_RUNOUT_OFF,              LABEL_RUNOUT_OFF},
    {ICON_RUNOUT_ON,               LABEL_RUNOUT_ON},
  };
  const  uint8_t item_runout[ITEM_RUNOUT_NUM] = {FILAMENT_RUNOUT_OFF, FILAMENT_RUNOUT_ON};
  static uint8_t item_runout_i = 0;
#endif

#ifdef POWER_LOSS_RECOVERY
  #define POWER_LOSS_RECOVERY_NUM 2
  const ITEM itemPowerloss[POWER_LOSS_RECOVERY_NUM] = {
  // icon                       label
    {ICON_POWERLOSS_OFF,              LABEL_POWERLOSS_OFF},
    {ICON_POWERLOSS_ON,               LABEL_POWERLOSS_ON},
  };
  const  uint8_t item_powerloss[POWER_LOSS_RECOVERY_NUM] = {FILAMENT_RUNOUT_OFF, FILAMENT_RUNOUT_ON};
  static uint8_t item_powerloss_i = 0;
#endif

#ifdef Z_HOT_HOMING
#define ITEM_HOT_HOMING_NUM 2
const ITEM itemHotHoming[ITEM_HOT_HOMING_NUM] = {
// icon                       label
  {ICON_ABLB4,                LABEL_HOT_HOMING_OFF},
  {ICON_ABLR4,                LABEL_HOT_HOMING_ON},
};
const  uint8_t item_hothoming[ITEM_HOT_HOMING_NUM] = {HOT_HOMING_OFF, HOT_HOMING_ON};
static uint8_t item_hothoming_i = 0;
#endif

#define ITEM_PS_ON_NUM 2
const ITEM itemPowerOff[ITEM_PS_ON_NUM] = {
// icon                       label
  {ICON_PSOFF,         LABEL_MANUAL_SHUT_DOWN},
  {ICON_PSON,          LABEL_AUTO_SHUT_DOWN},
};
const  uint8_t item_power_off[ITEM_PS_ON_NUM] = {0, 1};
static uint8_t item_power_off_i = 0;

void menuCallBackMachineSettings(void)
{
  KEY_VALUES key_num = menuKeyGetValue();
  static bool lock=false;
  switch(key_num)
  {
   #ifdef FIL_RUNOUT_PIN
    case KEY_ICON_0:
      item_runout_i = (item_runout_i + 1) % ITEM_RUNOUT_NUM;                
      machineSettingsItems.items[key_num] = itemRunout[item_runout_i];
      menuDrawItem(&machineSettingsItems.items[key_num], key_num);
      infoSettings.runout = item_runout[item_runout_i];
      infoSettings.runout ? storeCmd(PSTR("M412 S1")) : storeCmd(PSTR("M412 S0"));
      lock=true;
      break;
   #endif
   #ifdef POWER_LOSS_RECOVERY
    case KEY_ICON_1:
      item_powerloss_i = (item_powerloss_i + 1) % POWER_LOSS_RECOVERY_NUM;                
      machineSettingsItems.items[key_num] = itemPowerloss[item_powerloss_i];
      menuDrawItem(&machineSettingsItems.items[key_num], key_num);
      infoSettings.power_loss = item_powerloss[item_powerloss_i];
      infoSettings.power_loss ? storeCmd(PSTR("M413 S1")) : storeCmd(PSTR("M413 S0"));
      lock=true;
      break;
   #endif
    case KEY_ICON_2:
       planner.finish_and_disable();
      break;
  #ifdef Z_HOT_HOMING
    case KEY_ICON_3:
      item_hothoming_i = (item_hothoming_i + 1) % ITEM_HOT_HOMING_NUM;                
      machineSettingsItems.items[key_num] = itemHotHoming[item_hothoming_i];
      menuDrawItem(&machineSettingsItems.items[key_num], key_num);
      infoSettings.hot_homing=item_hothoming_i;
      lock=true;
    break;
  #endif
    case KEY_ICON_4:
      storeCmd("M81");
      break;
    case KEY_ICON_5:
      item_power_off_i = (item_power_off_i + 1) % ITEM_PS_ON_NUM;                
      machineSettingsItems.items[key_num] = itemPowerOff[item_power_off_i];
      menuDrawItem(&machineSettingsItems.items[key_num], key_num);
      infoSettings.auto_off = item_power_off[item_power_off_i];
      lock=true;
      break;
    case KEY_ICON_6:
      infoMenu.menu[++infoMenu.cur] = menuRGBSettings;
      break;
    case KEY_ICON_7:
      if(lock)
      {
       infoMenu.menu[++infoMenu.cur]=menuIsSettings;
      }
      else
      {
       infoMenu.cur--;
      }
      lock=false;
      break;
    
    default:
      break;
  }
}

void menuMachineSettings(void)
{
  #ifdef FIL_RUNOUT_PIN
    for(uint8_t i=0; i<ITEM_RUNOUT_NUM; i++)
    {
      if(infoSettings.runout == item_runout[i])
      {
        item_runout_i = i;
        machineSettingsItems.items[KEY_ICON_0] = itemRunout[item_runout_i];
      }
    }
  #endif  
  #ifdef POWER_LOSS_RECOVERY
  for(uint8_t i=0; i<POWER_LOSS_RECOVERY_NUM; i++)
    {
      if(infoSettings.power_loss == item_powerloss[i])
      {
        item_powerloss_i = i;
        machineSettingsItems.items[KEY_ICON_1] = itemPowerloss[item_powerloss_i];
      }
    }
  #endif
  #ifdef Z_HOT_HOMING
    for(uint8_t i=0; i<ITEM_HOT_HOMING_NUM; i++)
    {
      if(infoSettings.hot_homing == item_hothoming[i])
      {
        item_hothoming_i = i;
        machineSettingsItems.items[KEY_ICON_3] = itemHotHoming[item_hothoming_i];
      }
    }
  #endif  
    for(uint8_t i=0; i<ITEM_PS_ON_NUM; i++)
    {
      if(infoSettings.auto_off == item_power_off[i])
      {
        item_power_off_i = i;
        machineSettingsItems.items[KEY_ICON_5] = itemPowerOff[item_power_off_i];
      }
    }

  menuDrawPage(&machineSettingsItems);
  menuSetFrontCallBack(menuCallBackMachineSettings);
}

#endif


