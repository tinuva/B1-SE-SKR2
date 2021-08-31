#include "../../../../../inc/MarlinConfigPre.h"
#if ENABLED(BTT_UI_SPI)
#include "BabyStep.h"
#include "../TSC_Menu.h"
#include "../../../../../feature/babystep.h"
#include "../../../../../module/settings.h"
#include "../../../../../module/probe.h"

const char testGcode[] = "M82\n\
G28\n\
G1 Z15.0 F6000\n\
G92 E0\n\
G1 F200 E3\n\
G92 E0\n\
M83\n\
G1 F1500 E-6.5\n\
M107\n\
G0 F3600 X13.582 Y39.342 Z0.3\n\
G1 F1500 E6.5\n\
G1 F1800 X13.582 Y47.04 E0.38405\n\
G1 X13.582 Y65 E0.89603\n\
G1 X18.535 Y72.698 E0.45668\n\
G1 X30.091 Y90.658 E1.06548\n\
G1 X36.166 Y75.262 E0.82574\n\
G1 X50.338 Y39.342 E1.92649\n\
G1 X60.244 Y54.738 E0.91337\n\
G1 X83.356 Y90.658 E2.13097\n\
G1 X89.431 Y75.262 E0.82574\n\
G1 X103.604 Y39.342 E1.92651\n\
G1 X113.51 Y54.738 E0.91337\n\
G1 X136.622 Y90.658 E2.13097\n\
G1 X142.697 Y75.262 E0.82574\n\
G1 X156.869 Y39.342 E1.92649\n\
G1 X166.775 Y54.738 E0.91337\n\
G1 X189.887 Y90.658 E2.13097\n\
G1 X195.962 Y75.262 E0.82574\n\
G1 X210.135 Y39.342 E1.92651\n\
G1 X220.041 Y54.738 E0.91337\n\
G1 X243.153 Y90.658 E2.13097\n\
G1 X249.228 Y75.262 E0.82574\n\
G1 X263.401 Y39.342 E1.92651\n\
G1 X273.307 Y54.738 E0.91337\n\
G1 X296.418 Y90.658 E2.13094\n\
G1 X296.418 Y108.659 E0.89807\n\
G1 X296.418 Y150.658 E2.09534\n\
G1 X286.512 Y135.262 E0.91337\n\
G1 X263.401 Y99.342 E2.13094\n\
G1 X257.326 Y114.738 E0.82574\n\
G1 X243.153 Y150.658 E1.92651\n\
G1 X233.247 Y135.262 E0.91337\n\
G1 X210.135 Y99.342 E2.13097\n\
G1 X204.06 Y114.738 E0.82574\n\
G1 X189.887 Y150.658 E1.92651\n\
G1 X179.981 Y135.262 E0.91337\n\
G1 X156.869 Y99.342 E2.13097\n\
G1 X150.794 Y114.738 E0.82574\n\
G1 X136.622 Y150.658 E1.92649\n\
G1 X126.716 Y135.262 E0.91337\n\
G1 X103.604 Y99.342 E2.13097\n\
G1 X97.529 Y114.738 E0.82574\n\
G1 X83.356 Y150.658 E1.92651\n\
G1 X73.45 Y135.262 E0.91337\n\
G1 X50.338 Y99.342 E2.13097\n\
G1 X44.263 Y114.738 E0.82574\n\
G1 X30.091 Y150.658 E1.92649\n\
G1 X25.138 Y142.96 E0.45668\n\
G1 X13.582 Y125 E1.06548\n\
G1 X13.582 Y143.001 E0.89807\n\
G1 X13.582 Y185 E2.09534\n\
G1 X18.535 Y192.698 E0.45668\n\
G1 X30.091 Y210.658 E1.06548\n\
G1 X36.166 Y195.262 E0.82574\n\
G1 X50.338 Y159.342 E1.92649\n\
G1 X60.244 Y174.738 E0.91337\n\
G1 X83.356 Y210.658 E2.13097\n\
G1 X89.431 Y195.262 E0.82574\n\
G1 X103.604 Y159.342 E1.92651\n\
G1 X113.51 Y174.738 E0.91337\n\
G1 X136.622 Y210.658 E2.13097\n\
G1 X142.697 Y195.262 E0.82574\n\
G1 X156.869 Y159.342 E1.92649\n\
G1 X166.775 Y174.738 E0.91337\n\
G1 X189.887 Y210.658 E2.13097\n\
G1 X195.962 Y195.262 E0.82574\n\
G1 X210.135 Y159.342 E1.92651\n\
G1 X220.041 Y174.738 E0.91337\n\
G1 X243.153 Y210.658 E2.13097\n\
G1 X249.228 Y195.262 E0.82574\n\
G1 X263.401 Y159.342 E1.92651\n\
G1 X273.307 Y174.738 E0.91337\n\
G1 X296.418 Y210.658 E2.13094\n\
G1 X296.418 Y228.659 E0.89807\n\
G1 X296.418 Y270.658 E2.09534\n\
G1 X286.512 Y255.262 E0.91337\n\
G1 X263.401 Y219.342 E2.13094\n\
G1 X257.326 Y234.738 E0.82574\n\
G1 X243.153 Y270.658 E1.92651\n\
G1 X233.247 Y255.262 E0.91337\n\
G1 X210.135 Y219.342 E2.13097\n\
G1 X204.06 Y234.738 E0.82574\n\
G1 X189.887 Y270.658 E1.92651\n\
G1 X179.981 Y255.262 E0.91337\n\
G1 X156.869 Y219.342 E2.13097\n\
G1 X150.794 Y234.738 E0.82574\n\
G1 X136.622 Y270.658 E1.92649\n\
G1 X126.716 Y255.262 E0.91337\n\
G1 X103.604 Y219.342 E2.13097\n\
G1 X97.529 Y234.738 E0.82574\n\
G1 X83.356 Y270.658 E1.92651\n\
G1 X73.45 Y255.262 E0.91337\n\
G1 X50.338 Y219.342 E2.13097\n\
G1 X44.263 Y234.738 E0.82574\n\
G1 X30.091 Y270.658 E1.92649\n\
G1 X25.138 Y262.96 E0.45668\n\
G1 X13.582 Y245 E1.06548\n\
G1 F1500 E-6.5\n\
G0 F600 X13.582 Y245 Z0.5\n\
G0 F3600 X13.582 Y39.342\n\
M106 S255\n\
G1 F1500 E6.5\n\
G1 F1800 X13.582 Y59.869 E0.68273\n\
G1 X13.582 Y65 E0.17066\n\
G1 X26.79 Y85.527 E0.81185\n\
G1 X30.091 Y90.658 E0.20292\n\
G1 X46.289 Y49.604 E1.4679\n\
G1 X50.338 Y39.342 E0.36692\n\
G1 X76.753 Y80.396 E1.62369\n\
G1 X83.356 Y90.658 E0.40587\n\
G1 X99.555 Y49.604 E1.46791\n\
G1 X103.604 Y39.342 E0.36692\n\
G1 X130.019 Y80.396 E1.62369\n\
G1 X136.622 Y90.658 E0.40587\n\
G1 X152.82 Y49.604 E1.4679\n\
G1 X156.869 Y39.342 E0.36692\n\
G1 X183.284 Y80.396 E1.62369\n\
G1 X189.887 Y90.658 E0.40587\n\
G1 X206.086 Y49.604 E1.46791\n\
G1 X210.135 Y39.342 E0.36692\n\
G1 X236.55 Y80.396 E1.62369\n\
G1 X243.153 Y90.658 E0.40587\n\
G1 X259.352 Y49.604 E1.46791\n\
G1 X263.401 Y39.342 E0.36692\n\
G1 X289.815 Y80.396 E1.62367\n\
G1 X296.418 Y90.658 E0.40587\n\
G1 X296.418 Y138.659 E1.59652\n\
G1 X296.418 Y150.658 E0.39909\n\
G1 X270.004 Y109.604 E1.62367\n\
G1 X263.401 Y99.342 E0.40587\n\
G1 X247.202 Y140.396 E1.46791\n\
G1 X243.153 Y150.658 E0.36692\n\
G1 X216.738 Y109.604 E1.62369\n\
G1 X210.135 Y99.342 E0.40587\n\
G1 X193.936 Y140.396 E1.46791\n\
G1 X189.887 Y150.658 E0.36692\n\
G1 X163.472 Y109.604 E1.62369\n\
G1 X156.869 Y99.342 E0.40587\n\
G1 X140.671 Y140.396 E1.4679\n\
G1 X136.622 Y150.658 E0.36692\n\
G1 X110.207 Y109.604 E1.62369\n\
G1 X103.604 Y99.342 E0.40587\n\
G1 X87.405 Y140.396 E1.46791\n\
G1 X83.356 Y150.658 E0.36692\n\
G1 X56.941 Y109.604 E1.62369\n\
G1 X50.338 Y99.342 E0.40587\n\
G1 X34.14 Y140.396 E1.4679\n\
G1 X30.091 Y150.658 E0.36692\n\
G1 X16.883 Y130.131 E0.81185\n\
G1 X13.582 Y125 E0.20292\n\
G1 X13.582 Y173.001 E1.59652\n\
G1 X13.582 Y185 E0.39909\n\
G1 X26.79 Y205.527 E0.81185\n\
G1 X30.091 Y210.658 E0.20292\n\
G1 X46.289 Y169.604 E1.4679\n\
G1 X50.338 Y159.342 E0.36692\n\
G1 X76.753 Y200.396 E1.62369\n\
G1 X83.356 Y210.658 E0.40587\n\
G1 X99.555 Y169.604 E1.46791\n\
G1 X103.604 Y159.342 E0.36692\n\
G1 X130.019 Y200.396 E1.62369\n\
G1 X136.622 Y210.658 E0.40587\n\
G1 X152.82 Y169.604 E1.4679\n\
G1 X156.869 Y159.342 E0.36692\n\
G1 X183.284 Y200.396 E1.62369\n\
G1 X189.887 Y210.658 E0.40587\n\
G1 X206.086 Y169.604 E1.46791\n\
G1 X210.135 Y159.342 E0.36692\n\
G1 X236.55 Y200.396 E1.62369\n\
G1 X243.153 Y210.658 E0.40587\n\
G1 X259.352 Y169.604 E1.46791\n\
G1 X263.401 Y159.342 E0.36692\n\
G1 X289.815 Y200.396 E1.62367\n\
G1 X296.418 Y210.658 E0.40587\n\
G1 X296.418 Y258.659 E1.59652\n\
G1 X296.418 Y270.658 E0.39909\n\
G1 X270.004 Y229.604 E1.62367\n\
G1 X263.401 Y219.342 E0.40587\n\
G1 X247.202 Y260.396 E1.46791\n\
G1 X243.153 Y270.658 E0.36692\n\
G1 X216.738 Y229.604 E1.62369\n\
G1 X210.135 Y219.342 E0.40587\n\
G1 X193.936 Y260.396 E1.46791\n\
G1 X189.887 Y270.658 E0.36692\n\
G1 X163.472 Y229.604 E1.62369\n\
G1 X156.869 Y219.342 E0.40587\n\
G1 X140.671 Y260.396 E1.4679\n\
G1 X136.622 Y270.658 E0.36692\n\
G1 X110.207 Y229.604 E1.62369\n\
G1 X103.604 Y219.342 E0.40587\n\
G1 X87.405 Y260.396 E1.46791\n\
G1 X83.356 Y270.658 E0.36692\n\
G1 X56.941 Y229.604 E1.62369\n\
G1 X50.338 Y219.342 E0.40587\n\
G1 X34.14 Y260.396 E1.4679\n\
G1 X30.091 Y270.658 E0.36692\n\
G1 X16.883 Y250.131 E0.81185\n\
G1 X13.582 Y245 E0.20292\n\
G1 F1500 E-6.5\n\
M140 S0\n\
M82\n\
M107\n\
M104 S0\n\
M140 S0\n\
G92 E1\n\
G1 E-1 F300\n\
G28 X0 Y0\n\
M84\n\
M82\n\
M104 S0\n\
";
static uint32_t cur = 0;

void loopTest(void)
{
  bool    sd_comment_mode = false;
  bool    sd_comment_space = true;
  char    sd_char;
  uint8_t sd_count = 0;

  if (!pTest) return;
  //if (heatHasWaiting() || infoCmd.count)  return;

  for(; cur < COUNT(testGcode); )
  {
    sd_char = testGcode[cur];
    cur++;

    //Gcode
    if (sd_char == '\n' )         //'\n' is end flag for per command
    {
      sd_comment_mode = false;   //for new command
      sd_comment_space= true;
      if(sd_count!=0)
      {
        infoCmd.queue[infoCmd.index_w].gcode[sd_count++] = '\n';
        infoCmd.queue[infoCmd.index_w].gcode[sd_count] = 0; //terminate string
        infoCmd.queue[infoCmd.index_w].src = SERIAL_PORT;
        sd_count = 0; //clear buffer
        infoCmd.index_w = (infoCmd.index_w + 1) % CMD_MAX_LIST;
        infoCmd.count++;
        break;
      }
    }
    else if (sd_count >= CMD_MAX_CHAR - 2) {  }   //when the command length beyond the maximum, ignore the following bytes
    else
    {
      if (sd_char == ';')             //';' is comment out flag
        sd_comment_mode = true;
      else
      {
        if(sd_comment_space && (sd_char== 'G'||sd_char == 'M'||sd_char == 'T'))  //ignore ' ' space bytes
          sd_comment_space = false;
        if (!sd_comment_mode && !sd_comment_space && sd_char != '\r')  //normal gcode
          infoCmd.queue[infoCmd.index_w].gcode[sd_count++] = sd_char;
      }
    }
  }
  sendQueueCmd();
  if (cur == COUNT(testGcode)) {
    cur = 0;
    pTest = 0;
  }
}

static float getBabyStepZAxisTotalMM(){

  return babystep.axis_total[BS_TOTAL_IND(Z_AXIS)] * planner.steps_to_mm[Z_AXIS];
}

static void setBabyStepZAxisIncMM(float mm){

  float babysetvalue=getBabyStepZAxisTotalMM()+mm;
  if((babysetvalue>=BABY_MIN_VALUE)&&(babysetvalue<=BABY_MAX_VALUE))
  {
   babystep.add_mm(Z_AXIS, mm);
  }

}
//static float point_value = 0.0f;
void menuCallBackIsBabyStep(void)
{
  uint16_t key_num = KEY_GetValue(2, doubleBtnRect);
  switch(key_num)
  {
    case KEY_POPUP_CONFIRM:
      storeCmd("M500");

     volumeReminderMessage(LABEL_EEPROM_SAVE_SUCCESS, STATUS_NORMAL);
     _delay_ms(100);
     infoMenu.cur--;
    break;

    case KEY_POPUP_CANCEL:	
     infoMenu.cur--;
      break;		
  }
}

void menuIsSaveBabyStep(void)
{
  popupDrawPage(bottomDoubleBtn, textSelect(LABEL_WARNING), textSelect(LABEL_IS_BABYSTEP), textSelect(LABEL_CONFIRM), textSelect(LABEL_CANNEL));
  menuSetFrontCallBack(menuCallBackIsBabyStep);
}

//1 title, ITEM_PER_PAGE items(icon+label) 
MENUITEMS babyStepItems = {
//title
  LABEL_BABYSTEP,
//icon                        label
 {{ICON_DEC,                  LABEL_DEC},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_INC,                  LABEL_INC},
  {ICON_EEPROM_SAVE,          LABEL_EEPROM_SAVE},
  {ICON_01_MM,                LABEL_01_MM},
  {ICON_NORMAL_SPEED,         LABEL_VALUE_ZERO},
  {ICON_BACK,                 LABEL_BACK},}
};

typedef struct
{
  const ITEM *list;
  const float *ele;
  uint8_t cur;
  uint8_t totaled;
}ELEMENTS;

#define ITEM_BABYSTEP_UNIT_NUM 3
const ITEM itemBabyStepUnit[ITEM_BABYSTEP_UNIT_NUM] = {
// icon                       label
  {ICON_M001,               LABEL_001_MM},
  {ICON_01_MM,                LABEL_01_MM},
  {ICON_1_MM,                 LABEL_1_MM},
};
const float item_babystep_unit[ITEM_BABYSTEP_UNIT_NUM] = {0.01f, 0.1f, 1};

static ELEMENTS elementsUnit;

static void initElements(uint8_t position)
{
  elementsUnit.totaled = ITEM_BABYSTEP_UNIT_NUM;
  elementsUnit.list = itemBabyStepUnit;
  elementsUnit.ele  = item_babystep_unit;

  for(uint8_t i=0; i<elementsUnit.totaled; i++)
  {
    if(memcmp(&elementsUnit.list[i], &babyStepItems.items[position], sizeof(ITEM)) == 0)
    {
      elementsUnit.cur = i;
      break;
    }
  }
}

extern bool pTest;




void showBabyStep(void)
{
  GUI_DispFloat(CENTER_X - 5*BYTE_WIDTH/2, CENTER_Y, getBabyStepZAxisTotalMM(), 3, 2, RIGHT);
}
void babyStepReDraw(void)
{
  GUI_DispFloat(CENTER_X - 5*BYTE_WIDTH/2, CENTER_Y, getBabyStepZAxisTotalMM(), 3, 2, RIGHT);
}


void menuCallBackBabyStep(void)
{
  static float baby_step_value = 0.0f;
  KEY_VALUES key_num = menuKeyGetValue();
  switch(key_num)
  {
    case KEY_ICON_0:
      setBabyStepZAxisIncMM(-elementsUnit.ele[elementsUnit.cur]);
      break;
    case KEY_ICON_3:
      setBabyStepZAxisIncMM(elementsUnit.ele[elementsUnit.cur]);
      break;
    case KEY_ICON_4:
      infoMenu.menu[++infoMenu.cur] = menuIsSaveBabyStep;
      break;
    case KEY_ICON_5:
      elementsUnit.cur = (elementsUnit.cur + 1) % elementsUnit.totaled;
      babyStepItems.items[key_num] = elementsUnit.list[elementsUnit.cur];
      menuDrawItem(&babyStepItems.items[key_num], key_num);
      break;
    case KEY_ICON_6:
      setBabyStepZAxisIncMM(-baby_step_value);
      break;
    case KEY_ICON_7:
         infoMenu.cur--;
      break;
    default :
        #ifdef LCD_ENCODER_SUPPORT
        if(LCD_encoderPosition)
        {    
          if(LCD_encoderPosition > 0)
            setBabyStepZAxisIncMM(elementsUnit.ele[elementsUnit.cur]);
          if(LCD_encoderPosition < 0)
            setBabyStepZAxisIncMM(-elementsUnit.ele[elementsUnit.cur]);
          LCD_encoderPosition = 0;
        }
        #endif
      break;
  }
  if(baby_step_value != getBabyStepZAxisTotalMM())
  {
    baby_step_value = getBabyStepZAxisTotalMM();
    babyStepReDraw();
  }
}

void menuBabyStep()
{
  initElements(KEY_ICON_5);
  menuDrawPage(&babyStepItems);
  showBabyStep();
  menuSetFrontCallBack(menuCallBackBabyStep);
}
#endif



