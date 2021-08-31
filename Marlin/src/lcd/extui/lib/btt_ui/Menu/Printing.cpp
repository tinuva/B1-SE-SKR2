#include "../../../../../inc/MarlinConfigPre.h"
#if ENABLED(BTT_UI_SPI)
#include "../TSC_Menu.h"
#include "../../../../../feature/babystep.h"
#include "../../../../../module/settings.h"
#include "../../../../../module/probe.h"

MENUITEMS printingItems = {
//  title
LABEL_BACKGROUND,
// icon                       label
 {{ICON_PAUSE,                LABEL_PAUSE},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_STOP,                 LABEL_STOP},
  {ICON_NOZZLE,               LABEL_NOZZLE},
  {ICON_5_DEGREE,             LABEL_5_DEGREE},
  {ICON_DEC,                  LABEL_DEC},
  {ICON_INC,                  LABEL_INC},}
};

const ITEM itemIsPause[2] = {
// icon                       label
  {ICON_PAUSE,                LABEL_PAUSE},
  {ICON_RESUME,               LABEL_RESUME},
};

//
bool isPrinting(void)
{
  return ExtUI::isPrintingFromMedia() && !card.flag.abort_sd_printing;
}
bool ismenuStopMotor(void)
{
  if(infoMenu.menu[infoMenu.cur]==menuStopMotor)
  return true;
  else return false;
}
//
bool isPaused(void)
{
  return ExtUI::isPrintingFromMediaPaused();
}

uint8_t getPrintProgress(void)
{
  return card.percentDone();
}

uint32_t getPrintTime(void)
{
  return print_job_timer.duration();
}
//only return gcode file name except path
//for example:"SD:/test/123.gcode"
//only return "123.gcode"
uint8_t *getCurGcodeName(char *path)
{
  int i=strlen(path);
  for(; path[i]!='/'&& i>0; i--)
  {}
  return (uint8_t* )(&path[i+1]);
}

void resumeToPause(bool is_pause)
{
  if(infoMenu.menu[infoMenu.cur] != menuPrinting) return;
  printingItems.items[KEY_ICON_0] = itemIsPause[is_pause];
  menuDrawItem(&itemIsPause[is_pause],0);
}

bool setPrintPause(bool is_pause)
{
  static bool pauseLock = false;
  if(pauseLock)                      return false;
  if(!isPrinting())                  return false;
  if(isPaused() == is_pause)         return false;
  pauseLock = true;
  if (is_pause) {
    ExtUI::pausePrint();
  } else {
    ExtUI::setUserConfirmed();
    ExtUI::resumePrint();
  }
  pauseLock = false;
  return true;
}



const GUI_RECT progressRect = {1*SPACE_X_PER_ICON, 0*ICON_HEIGHT+0*SPACE_Y+TITLE_END_Y + ICON_HEIGHT/4,
                               3*SPACE_X_PER_ICON, 0*ICON_HEIGHT+0*SPACE_Y+TITLE_END_Y + ICON_HEIGHT*3/4};

#define BED_X  (progressRect.x1 - 11 * BYTE_WIDTH)
#define BED_Y  (progressRect.x1 - 19 * BYTE_WIDTH)
#define TEMP_X  progressRect.x0
#define TEMP_Y (progressRect.y1 - 4*BYTE_HEIGHT-4)
#define TIME_Y TEMP_Y+2*BYTE_HEIGHT
#define BABY_X BED_X-5*BYTE_WIDTH 
#define BABY_Y TEMP_Y+4*BYTE_HEIGHT 
#define Z_OFFER  (progressRect.x1 - 15 * BYTE_WIDTH)
#define SPEED_X  TEMP_X+2*BYTE_WIDTH
#define PERCENT_X   BED_X+4*BYTE_WIDTH
#define PERCENT_Y   TIME_Y   
#define FLOW_X    TEMP_X+2*BYTE_WIDTH
#define FAN_X     BED_X
#define SPEED_Y   BABY_Y
#define FLOW_Y  TEMP_Y+6*BYTE_HEIGHT   
#define POSITION_XY_Y (progressRect.y0 - 1 * BYTE_HEIGHT - 3)

static void redrawToolAct(void)
{
  GUI_PrintDispDec(progressRect.x0+BYTE_WIDTH*5, TEMP_Y, (int16_t)statusMsg.actHotend, 3, RIGHT);
}
static void redrawToolTag(void)
{
  GUI_PrintDispDec(progressRect.x0+BYTE_WIDTH*9, TEMP_Y, (int16_t)statusMsg.tagHotend,  3, LEFT);
}

static void redrawBedAct(void)
{
  GUI_PrintDispDec(BED_X + 4 * BYTE_WIDTH, TEMP_Y, (int16_t)statusMsg.actBed, 3, RIGHT);
}

static void redrawBedTag(void)
{
  GUI_PrintDispDec(BED_X + 8 * BYTE_WIDTH, TEMP_Y, (int16_t)statusMsg.tagBed,  3, LEFT);
}

// static void redrawPositionX(void)
// {
//   GUI_PrintDispFloat(progressRect.x0 + 2*BYTE_WIDTH, POSITION_XY_Y, statusMsg.x, 4, 2, LEFT);
// }
// static void redrawPositionY(void)
// {
//   GUI_PrintDispFloat(BED_Y + 2*BYTE_WIDTH, POSITION_XY_Y, statusMsg.y, 4, 2, LEFT);
// }
// static void redrawPositionZ(void)
// {
//    GUI_PrintDispFloat(BED_X + 2*BYTE_WIDTH, POSITION_XY_Y, statusMsg.z, 4, 2, LEFT);
// }

void reDrawTime(void)
{
  uint32_t printedTime = getPrintTime();
  uint8_t  hour = printedTime/3600%60,
           min = printedTime%3600/60;
           //sec = printedTime%60;
  GUI_SetNumMode(GUI_NUMMODE_ZERO);
  GUI_PrintDispDec(progressRect.x0 + 6 * BYTE_WIDTH, TIME_Y, hour, 2, RIGHT);
  GUI_PrintDispDec(progressRect.x0 + 9 * BYTE_WIDTH, TIME_Y, min, 2, RIGHT);
  //GUI_PrintDispDec(progressRect.x0 + 8 * BYTE_WIDTH, TIME_Y, sec, 2, RIGHT);
  GUI_SetNumMode(GUI_NUMMODE_SPACE);
}

void reDrawProgress(uint8_t progress)
{	  
  char buf[5];
  uint16_t progressX = map(progress, 0, 100, progressRect.x0, progressRect.x1);
  GUI_FillRectColor(progressRect.x0, progressRect.y0, progressX, progressRect.y1,BLUE);
  GUI_FillRectColor(progressX, progressRect.y0, progressRect.x1, progressRect.y1,GRAY);
  sprintf(buf, "%3d%%", progress);
  GUI_SetTextMode(GUI_TEXTMODE_TRANS);
  GUI_DispStringInPrect(&progressRect, (uint8_t *)buf);    
  GUI_SetTextMode(GUI_TEXTMODE_NORMAL);                     
}

static float print_baby_step_value = 0.0f;

static float getBabyStepPrintZAxisTotalMM(){

    return babystep.axis_total[BS_TOTAL_IND(Z_AXIS)] * planner.steps_to_mm[Z_AXIS];
}

static void setBabyStepPrintZAxisIncMM(float mm){
   float babysetvalue=getBabyStepPrintZAxisTotalMM()+mm;
  if((babysetvalue>=BABY_MIN_VALUE)&&(babysetvalue<=BABY_MAX_VALUE))
  {
    babystep.add_mm(Z_AXIS, mm);
  }
}

void reDrawBabystep()
{
   //probe.offset.z=babystep.axis_total[BS_TOTAL_IND(Z_AXIS)] * planner.steps_to_mm[Z_AXIS];
   GUI_DispFloat(BABY_X + 9*BYTE_WIDTH, BABY_Y, getBabyStepPrintZAxisTotalMM(), 3, 2, RIGHT);
}

void reDrawTitle(void)
{
  printingItems.title.address = (uint8_t *)card.longest_filename(); //getCurGcodeName(infoFile.title);
  menuDrawTitle(printingItems.title.address);
}

extern SCROLL   titleScroll;
extern GUI_RECT titleRect;

void reDrawSpeed();
void reDrawFlow();
void reDrawFan();
void reDrawrecent();

void printingDrawPage(void)
{
  printingItems.title.address = (uint8_t *)card.longest_filename(); //getCurGcodeName(infoFile.title);
  
  if (preview_file_Decode((char *)card.filename,KEY_ICON_3)) {
    printingItems.items[3].icon=ICON_PREVIEW;
  } else {
    printingItems.items[3].icon=ICON_STOP;
  }

  menuDrawPage(&printingItems);

  // nozzle temperature 
  MINIICON_ReadDisplay(TEMP_X,TEMP_Y-3,MINI_ICON_T0);
  GUI_DispString(TEMP_X+BYTE_WIDTH*8, TEMP_Y,(uint8_t* )"/");
  // hotbed temperature
  MINIICON_ReadDisplay(BED_X,TEMP_Y-3,MINI_ICON_BED);
  GUI_DispString(BED_X+BYTE_WIDTH*7, TEMP_Y, (uint8_t* )"/");
  // printed time
  MINIICON_ReadDisplay(TEMP_X,TIME_Y-3,MINI_ICON_T);
  GUI_DispString(TEMP_X+BYTE_WIDTH*8, TIME_Y-3, (uint8_t* )":");
  //Progress percentage
  MINIICON_ReadDisplay(BED_X,TIME_Y-3,MINI_ICON_PRO);
  MINIICON_ReadDisplay(BED_X,BABY_Y-3,MINI_ICON_BABYSTEP);
  MINIICON_ReadDisplay(SPEED_X-2*BYTE_WIDTH,SPEED_Y-3,MINI_ICON_SP);
  MINIICON_ReadDisplay(FLOW_X-2*BYTE_WIDTH,FLOW_Y-3,MINI_ICON_FL);
  MINIICON_ReadDisplay(BED_X,FLOW_Y-3,MINI_ICON_FAN);
  GUI_DispString(SPEED_X + 7*BYTE_WIDTH , SPEED_Y, (uint8_t *)"%");
  GUI_DispString(FLOW_X + 7*BYTE_WIDTH , FLOW_Y, (uint8_t *)"%");

  statusMsg_GetCurMsg(&statusMsg);
 // GUI_DispString(progressRect.x0, TEMP_Y, (uint8_t* )heatDisplayID[statusMsg.curTool + 1]);
  redrawToolAct();
  redrawBedAct();
  redrawToolTag();
  redrawBedTag();
  reDrawrecent();
  reDrawBabystep();
  reDrawTime();
  reDrawSpeed();
  reDrawFlow();
  reDrawFan();
}

#define ITEM_SCAN_NUM 6
#define ITEM_PRINT_DEGREE_NUM 4
#define ITEM_BABYSTEP_DEGREE_NUM 4

typedef enum
{
  NOZZLE=0,
  BED,
  BABYSTEP,
  SPEED,
  FLOW,
  FAN,
  EXTRUDE,
  ITEM_SCAN_TOOL,
}ITEM_SCAN;

typedef struct
{
  const ITEM *list;
  const float *ele;
  uint8_t cur;
  uint8_t totaled;
}PRINTELEMENTS;

static uint8_t lastProgress = 0;
static uint8_t printPaused = 0;
static uint32_t printedTime = 0;
const  uint8_t item_printdegree[ITEM_PRINT_DEGREE_NUM] = {1, 5, 10};
static uint8_t item_printdegree_i = 1;
static uint8_t printIndex = 0; // 0:T0 1: Bed, 2:babystep, 2: T1, etc
const float item_babystep_print[ITEM_PRINT_DEGREE_NUM] = {0.01f, 0.1f, 1};
static PRINTELEMENTS printelements;
const  uint8_t item_percentdegree[ITEM_PRINT_DEGREE_NUM] = {1, 5, 10};
static uint8_t item_percent_degree_i = 1;
static uint16_t lastPrintPercentage = 0;
static uint16_t lastPrintFlowage = 0;

static ExtUI::fan_t PrintcurIndex = ExtUI::FAN0;
static uint8_t fanPrintPercent = 0;
static uint8_t item_Fan_degree_i = 1;
const  uint8_t item_Fandegree[ITEM_PRINT_DEGREE_NUM] = {1, 5};

const ITEM printitemTool[] = {
// icon                       label
  {ICON_NOZZLE,               LABEL_NOZZLE},
  {ICON_BED,                  LABEL_BED},
  {ICON_BABYSTEP,             LABEL_BABYSTEP},
  {ICON_SPEEDPER,             LABEL_PERCENTAGE_SPEED},
  {ICON_FLOWPER,              LABEL_PERCENTAGE_FLOW},
  {ICON_FAN ,                 LABEL_FAN},
  {ICON_EXTRUDE,              LABEL_EXTRUDE},
};    

const ITEM itemHotDegree[ITEM_PRINT_DEGREE_NUM] = {
// icon                       label
  {ICON_1_DEGREE,             LABEL_1_DEGREE},
  {ICON_5_DEGREE,             LABEL_5_DEGREE},
  {ICON_10_DEGREE,            LABEL_10_DEGREE},
  {ICON_STOP,                 LABEL_STOP},
};
const ITEM itemBabyDegree[ITEM_PRINT_DEGREE_NUM] = {
// icon                       label
  {ICON_M001,               LABEL_001_MM},
  {ICON_01_MM,                LABEL_01_MM},
  {ICON_1_MM,                 LABEL_1_MM},
  {ICON_NORMAL_SPEED,         LABEL_VALUE_ZERO},
};
const ITEM itemPercentDegree[ITEM_PRINT_DEGREE_NUM] = {
// icon                       label
  {ICON_E_1_MM,               LABEL_1_PERCENT},
  {ICON_E_5_MM,               LABEL_5_PERCENT},
  {ICON_E_10_MM,              LABEL_10_PERCENT},
  {ICON_NORMAL_SPEED,         LABEL_NORMAL_SPEED},
};
const ITEM itemFanDegree[ITEM_PRINT_DEGREE_NUM] = {
// icon                       label
  {ICON_E_1_MM,               LABEL_1_PERCENT},
  {ICON_E_5_MM,               LABEL_5_PERCENT},
  {ICON_FAN_FULL_SPEED,       LABEL_FAN_FULL_SPEED},
  {ICON_STOP,                 LABEL_STOP},
};

void setPrintfTemp(int16_t tmp) {
  if (tmp < 0) return;
  if (printIndex==0) {    
    thermalManager.setTargetHotend(tmp, printIndex);
  } else {
    thermalManager.setTargetBed(tmp);
  }
}
int16_t getPrintTemp(void) {
  int16_t tarTemp = (printIndex==0) ? thermalManager.degTargetHotend(printIndex) : thermalManager.degTargetBed();
  // if (tarTemp == 0) {
  //   tarTemp = (printIndex==0) ? 180 :50;
  // }
  return tarTemp;
}

static void initPrintElements(uint8_t position)
{
  printelements.totaled = ITEM_BABYSTEP_DEGREE_NUM;
  printelements.list = itemBabyDegree;
  printelements.ele  = item_babystep_print;

  for(uint8_t i=0; i<printelements.totaled; i++)
  {
    if(memcmp(&printelements.list[i], &printingItems.items[position], sizeof(ITEM)) == 0)
    {
      printelements.cur = i;
      break;
    }
  }
}

uint16_t getPrintGercentageValue(uint8_t flowOrSpeed)
{
  if (flowOrSpeed == 0) return ExtUI::getFeedrate_percent();
  else return ExtUI::getFlowPercentage(ExtUI::getActiveTool());
}
void setPrintGercentageValue(uint8_t flowOrSpeed, uint16_t value)
{
  if (flowOrSpeed == 0) return  ExtUI::setFeedrate_percent(value);
  else return ExtUI::setFlow_percent(value, ExtUI::getActiveTool());
}
void  reDrawSpeed()
{
   GUI_PrintDispDec(SPEED_X + 4*BYTE_WIDTH, SPEED_Y,getPrintGercentageValue(0), 3, RIGHT);
}
void  reDrawFlow()
{
   GUI_PrintDispDec(FLOW_X + 4*BYTE_WIDTH, FLOW_Y,getPrintGercentageValue(1), 3, RIGHT);
}
void  reDrawFan()
{
  char Pacessbuf[5];
  sprintf(Pacessbuf, "%3d%%", fanPrintPercent);
  LCD_ShowString(PERCENT_X,FLOW_Y,16*(sizeof(Pacessbuf)),16,(uint8_t *)Pacessbuf);
}
void reDrawrecent()
{
  char Pacessbuf[5];
  sprintf(Pacessbuf, "%3d%%", lastProgress);
  LCD_ShowString(PERCENT_X,PERCENT_Y,16*(sizeof(Pacessbuf)),16,(uint8_t *)Pacessbuf);
}
void menuIsPause();


void menuCallBackPrinting(void)	
{
  STATUS_MSG tempMsg;
  statusMsg_GetCurMsg(&tempMsg);
  
  KEY_VALUES  key_num = PrintmenuKeyGetValue();
  static uint32_t nowTime_ms = 0;
 
  if (millis() - nowTime_ms > 1000) { // Refresh per 1 sec
    nowTime_ms = millis();
    
    if(print_baby_step_value != getBabyStepPrintZAxisTotalMM())
    {
      print_baby_step_value = getBabyStepPrintZAxisTotalMM();
      reDrawBabystep();
    }
    if(lastPrintPercentage != getPrintGercentageValue(0))
    {
      lastPrintPercentage = getPrintGercentageValue(0);
      reDrawSpeed();
    }
    if(lastPrintFlowage != getPrintGercentageValue(1))
    {
      lastPrintFlowage = getPrintGercentageValue(1);
      reDrawFlow();
    }
    if(fanPrintPercent != ExtUI::getTargetFan_percent(PrintcurIndex))
    {
      fanPrintPercent = ExtUI::getTargetFan_percent(PrintcurIndex);
      reDrawFan();
    }
    
    if (statusMsg.actHotend != tempMsg.actHotend) {
      statusMsg.actHotend = tempMsg.actHotend;
      redrawToolAct();
    }    
    if (statusMsg.actBed != tempMsg.actBed) {
      statusMsg.actBed = tempMsg.actBed;
      redrawBedAct();
    }
    if (lastProgress != getPrintProgress())
    {
      lastProgress = getPrintProgress();
      reDrawrecent();
    }
    if (printedTime != getPrintTime()%3600/60)
    {
      printedTime = getPrintTime()%3600/60;
      reDrawTime();
    }
  }

  if (statusMsg.tagHotend != tempMsg.tagHotend) {
    statusMsg.tagHotend = tempMsg.tagHotend;
    redrawToolTag();
  }
  if (statusMsg.tagBed != tempMsg.tagBed) {
    statusMsg.tagBed = tempMsg.tagBed;
    redrawBedTag();
  }

  if (printPaused != isPaused()) {
    printPaused = isPaused();
    resumeToPause(printPaused);
  }

  switch(key_num)
  {
    case KEY_ICON_0:
      setPrintPause(!isPaused());
      break;                                                                                                                                                                                                                                                  
    
    case KEY_ICON_3:
      if(isPrinting())				
        infoMenu.menu[++infoMenu.cur] = menuStopPrinting;	
      break;
      
    case KEY_ICON_4:
      
        printIndex = (printIndex+1) % ITEM_SCAN_NUM;
        printingItems.items[key_num] = printitemTool[printIndex];
        menuDrawItem(&printingItems.items[key_num], key_num);

        switch(printIndex)
         {
           case NOZZLE: 
           case BED: 
               item_printdegree_i = (item_printdegree_i) % ITEM_PRINT_DEGREE_NUM;
               printingItems.items[KEY_ICON_5] = itemHotDegree[item_printdegree_i];
               menuDrawItem(&printingItems.items[KEY_ICON_5], KEY_ICON_5);
           break;
           case BABYSTEP: 
              printelements.cur = (printelements.cur) % printelements.totaled;
              printingItems.items[KEY_ICON_5] = printelements.list[printelements.cur];
              menuDrawItem(&printingItems.items[KEY_ICON_5], KEY_ICON_5);
           break;
           case SPEED: 
           case FLOW: 
              item_percent_degree_i = (item_percent_degree_i) % ITEM_PRINT_DEGREE_NUM;
              printingItems.items[KEY_ICON_5] = itemPercentDegree[item_percent_degree_i];
              menuDrawItem(&printingItems.items[KEY_ICON_5], KEY_ICON_5);
           break;
           case FAN:
              item_Fan_degree_i = (item_Fan_degree_i) % ITEM_PRINT_DEGREE_NUM;
              printingItems.items[KEY_ICON_5] = itemFanDegree[item_Fan_degree_i];
              menuDrawItem(&printingItems.items[KEY_ICON_5], KEY_ICON_5);
           break; 
           
           default :break;
         }
      break;
    
    case KEY_ICON_5:
        switch(printIndex)
         {
           case NOZZLE: 
           case BED: 
               item_printdegree_i = (item_printdegree_i+1) % ITEM_PRINT_DEGREE_NUM;
               printingItems.items[key_num] = itemHotDegree[item_printdegree_i];
               menuDrawItem(&printingItems.items[key_num], key_num);
           break;
           case BABYSTEP: 
              printelements.cur = (printelements.cur + 1) % printelements.totaled;
              printingItems.items[key_num] = printelements.list[printelements.cur];
              menuDrawItem(&printingItems.items[key_num], key_num);
           break;
           case SPEED:
           case FLOW:
              item_percent_degree_i = (item_percent_degree_i+1) % ITEM_PRINT_DEGREE_NUM;
              printingItems.items[key_num] = itemPercentDegree[item_percent_degree_i];
              menuDrawItem(&printingItems.items[key_num], key_num);
           break;
           case FAN:
              item_Fan_degree_i = (item_Fan_degree_i+1) % ITEM_PRINT_DEGREE_NUM;
              printingItems.items[key_num] = itemFanDegree[item_Fan_degree_i];
              menuDrawItem(&printingItems.items[key_num], key_num);
           break;  
           case EXTRUDE:
              if (!isPaused()) // need paused before extrude
                infoMenu.menu[++infoMenu.cur] = menuIsPause;
              else
                infoMenu.menu[++infoMenu.cur] = menuExtrude; 
              break;  
           break;            
           default :break;
         }
      break;
    
    case KEY_ICON_6:

        switch(printIndex)
              {
                  case NOZZLE: 
                    setPrintfTemp((item_printdegree_i>=3)?0:(getPrintTemp() - item_printdegree[item_printdegree_i]));  
                    PrintstatusMsg_GetT0Tag(&tempMsg);
                    statusMsg.tagHotend = tempMsg.tagHotend;
                    redrawToolTag();
                  break;
                  case BED: 
                     setPrintfTemp((item_printdegree_i>=3)?0:(getPrintTemp() - item_printdegree[item_printdegree_i]));  
                     PrintstatusMsg_GetBEDTag(&tempMsg);
                     statusMsg.tagBed = tempMsg.tagBed;
                     redrawBedTag();
                  break;
                  case BABYSTEP: 
                     setBabyStepPrintZAxisIncMM((printelements.cur>=3)?(-print_baby_step_value):(-printelements.ele[printelements.cur]));
                  break;
                  case SPEED:
                      if(item_percent_degree_i>=3)
                      setPrintGercentageValue(0,100);   
                      else 
                      setPrintGercentageValue(0, getPrintGercentageValue(0) - item_percentdegree[item_percent_degree_i]);
                  break;
                  case FLOW:
                      if(item_percent_degree_i>=3)
                      setPrintGercentageValue(1,100);   
                      else 
                      setPrintGercentageValue(1, getPrintGercentageValue(1) - item_percentdegree[item_percent_degree_i]);
                  break;
                  case FAN:
                        if(item_Fan_degree_i==2)
                        {
                          ExtUI::setTargetFan_percent(100, PrintcurIndex);
                        }else  if(item_Fan_degree_i==3)
                        {
                          ExtUI::setTargetFan_percent(0, PrintcurIndex); 
                        }else
                        {
                          ExtUI::setTargetFan_percent(fanPrintPercent-item_Fandegree[item_percent_degree_i], PrintcurIndex);
                        }
                  break;
                  case EXTRUDE:
                    if (!isPaused()) // need paused before extrude
                      infoMenu.menu[++infoMenu.cur] = menuIsPause;
                    else
                      infoMenu.menu[++infoMenu.cur] = menuExtrude; 
                  break;  
                  default :break;
              }
            
      break;
    
    case KEY_ICON_7:

        switch(printIndex)
              {
                case NOZZLE: 
                case BED: 
                     setPrintfTemp((item_printdegree_i>=3)?0:(getPrintTemp() + item_printdegree[item_printdegree_i]));
                     PrintstatusMsg_GetT0Tag(&tempMsg);
                     statusMsg.tagHotend = tempMsg.tagHotend;
                     redrawToolTag();
                  break;
                case BABYSTEP: 
                     setBabyStepPrintZAxisIncMM((printelements.cur>=3)?(-print_baby_step_value):(printelements.ele[printelements.cur]));
                     PrintstatusMsg_GetBEDTag(&tempMsg);
                     statusMsg.tagBed = tempMsg.tagBed;
                     redrawBedTag();
                  break;
                case SPEED:
                      if(item_percent_degree_i>=3)
                      setPrintGercentageValue(0,100);   
                      else 
                      setPrintGercentageValue(0, getPrintGercentageValue(0) + item_percentdegree[item_percent_degree_i]);
                  break;
                case FLOW:
                     if(item_percent_degree_i>=3)
                      setPrintGercentageValue(1,100);   
                      else 
                      setPrintGercentageValue(1, getPrintGercentageValue(1) + item_percentdegree[item_percent_degree_i]);
                  break;
                case FAN:
                      if(item_Fan_degree_i==2)
                      {
                        ExtUI::setTargetFan_percent(100, PrintcurIndex);
                      }else  if(item_Fan_degree_i==3)
                      {
                        ExtUI::setTargetFan_percent(0, PrintcurIndex); 
                      }else
                      {
                        if (fanPrintPercent < 100)
                        ExtUI::setTargetFan_percent(fanPrintPercent+item_Fandegree[item_percent_degree_i], PrintcurIndex);
                      } 
                  break;  
                 case EXTRUDE:
                    if (!isPaused()) // need paused before extrude
                      infoMenu.menu[++infoMenu.cur] = menuIsPause;
                    else
                      infoMenu.menu[++infoMenu.cur] = menuExtrude; 
                  break;
                  default :break;
              }
     
      break;
    
    default :
          #ifdef LCD_ENCODER_SUPPORT
            if(LCD_encoderPosition)
            {    
              if(LCD_encoderPosition > 0)
              {
                switch(printIndex)
                {
                  case NOZZLE: 
                  case BED: 
                      setPrintfTemp((item_printdegree_i>=3)?0:(getPrintTemp() + item_printdegree[item_printdegree_i]));
                      PrintstatusMsg_GetT0Tag(&tempMsg);
                      statusMsg.tagHotend = tempMsg.tagHotend;
                      redrawToolTag();
                    break;
                  case BABYSTEP: 
                      setBabyStepPrintZAxisIncMM((printelements.cur>=3)?(-print_baby_step_value):(printelements.ele[printelements.cur]));
                      PrintstatusMsg_GetBEDTag(&tempMsg);
                      statusMsg.tagBed = tempMsg.tagBed;
                      redrawBedTag();
                    break;
                  case SPEED:
                        if(item_percent_degree_i>=3)
                        setPrintGercentageValue(0,100);   
                        else 
                        setPrintGercentageValue(0, getPrintGercentageValue(0) + item_percentdegree[item_percent_degree_i]);
                    break;
                  case FLOW:
                      if(item_percent_degree_i>=3)
                        setPrintGercentageValue(1,100);   
                        else 
                        setPrintGercentageValue(1, getPrintGercentageValue(1) + item_percentdegree[item_percent_degree_i]);
                    break;
                  case FAN:
                        if(item_Fan_degree_i==2)
                        {
                          ExtUI::setTargetFan_percent(100, PrintcurIndex);
                        }else  if(item_Fan_degree_i==3)
                        {
                          ExtUI::setTargetFan_percent(0, PrintcurIndex); 
                        }else
                        {
                          if (fanPrintPercent < 100)
                          ExtUI::setTargetFan_percent(fanPrintPercent+item_Fandegree[item_percent_degree_i], PrintcurIndex);
                        } 
                    break;  
                  case EXTRUDE:
                      // if (!isPaused()) // need paused before extrude
                      //   infoMenu.menu[++infoMenu.cur] = menuIsPause;
                      // else
                      //   infoMenu.menu[++infoMenu.cur] = menuExtrude; 
                    break;
                    default :break;
                }
              }
              if(LCD_encoderPosition < 0)
              {
                  switch(printIndex)
                {
                    case NOZZLE: 
                      setPrintfTemp((item_printdegree_i>=3)?0:(getPrintTemp() - item_printdegree[item_printdegree_i]));  
                      PrintstatusMsg_GetT0Tag(&tempMsg);
                      statusMsg.tagHotend = tempMsg.tagHotend;
                      redrawToolTag();
                    break;
                    case BED: 
                      setPrintfTemp((item_printdegree_i>=3)?0:(getPrintTemp() - item_printdegree[item_printdegree_i]));  
                      PrintstatusMsg_GetBEDTag(&tempMsg);
                      statusMsg.tagBed = tempMsg.tagBed;
                      redrawBedTag();
                    break;
                    case BABYSTEP: 
                      setBabyStepPrintZAxisIncMM((printelements.cur>=3)?(-print_baby_step_value):(-printelements.ele[printelements.cur]));
                    break;
                    case SPEED:
                        if(item_percent_degree_i>=3)
                        setPrintGercentageValue(0,100);   
                        else 
                        setPrintGercentageValue(0, getPrintGercentageValue(0) - item_percentdegree[item_percent_degree_i]);
                    break;
                    case FLOW:
                        if(item_percent_degree_i>=3)
                        setPrintGercentageValue(1,100);   
                        else 
                        setPrintGercentageValue(1, getPrintGercentageValue(1) - item_percentdegree[item_percent_degree_i]);
                    break;
                    case FAN:
                          if(item_Fan_degree_i==2)
                          {
                            ExtUI::setTargetFan_percent(100, PrintcurIndex);
                          }else  if(item_Fan_degree_i==3)
                          {
                            ExtUI::setTargetFan_percent(0, PrintcurIndex); 
                          }else
                          {
                            ExtUI::setTargetFan_percent(fanPrintPercent-item_Fandegree[item_percent_degree_i], PrintcurIndex);
                          }
                    break;
                    case EXTRUDE:
                      // if (!isPaused()) // need paused before extrude
                      //   infoMenu.menu[++infoMenu.cur] = menuIsPause;
                      // else
                      //   infoMenu.menu[++infoMenu.cur] = menuExtrude; 
                    break;  
                    default :break;
                }
              }
              LCD_encoderPosition = 0;
            }
          #endif
    break;
  }
}


void menuPrinting(void)
{
  printPaused = isPaused();
  printingItems.items[KEY_ICON_0] = itemIsPause[printPaused];
  initPrintElements(KEY_ICON_5);
  printingDrawPage();
  menuSetFrontCallBack(menuCallBackPrinting);
}

void menuCallStopMotor()
{

}
void menuStopMotor()
{
  GUI_Clear(BLACK);
  GUI_DispString(LCD_WIDTH_PIXEL/4+10*BYTE_WIDTH, LCD_HEIGHT_PIXEL/2-2*BYTE_HEIGHT, textSelect(LABEL_PSTOP));
  menuSetFrontCallBack(menuCallStopMotor);
}

void menuCallBackStopPrinting(void)
{
  uint16_t key_num = KEY_GetValue(2, doubleBtnRect);
  switch(key_num)
  {
    case KEY_POPUP_CONFIRM:
      VIOLENCE_flag=true;
      ExtUI::stopPrint();
     
      infoMenu.menu[++infoMenu.cur] = menuStopMotor; 
      
      break;

    case KEY_POPUP_CANCEL:
      infoMenu.cur--;
      break;		
  }
}

void menuStopPrinting(void)
{
  popupDrawPage(bottomDoubleBtn, textSelect(LABEL_WARNING), textSelect(LABEL_STOP_PRINT), textSelect(LABEL_CONFIRM), textSelect(LABEL_CANNEL));
  menuSetFrontCallBack(menuCallBackStopPrinting);
}

// Shut down menu, when the hotend temperature is higher than "AUTO_SHUT_DOWN_MAXTEMP"
// wait for cool down, in the meantime, you can shut down by force
void menuCallBackShutDown(void)
{
  static bool lock = false;
  bool shutDown = false;
  bool tempIsLower = true;
  uint16_t key_num = KEY_GetValue(2, doubleBtnRect);

  if (lock) return;
  lock = true;
  switch(key_num)
  {
    case KEY_POPUP_CONFIRM:
      shutDown = true;
      break;;

    case KEY_POPUP_CANCEL:
      infoMenu.cur--;
      break;		
  }
  for (ExtUI::extruder_t i = ExtUI::E0; i < EXTRUDERS; i = (ExtUI::extruder_t)(i + 1)) {
    if (ExtUI::getActualTemp_celsius(i) >= AUTO_SHUT_DOWN_MAXTEMP) {
      tempIsLower = false;
    }
  }
  if(tempIsLower || shutDown)
  {
    for (ExtUI::fan_t i = ExtUI::FAN0; i < FAN_COUNT; i = (ExtUI::fan_t)(i + 1))
      ExtUI::setTargetFan_percent(0, i);
    storeCmd(PSTR("M81"));
    infoMenu.cur--;
    popupReminder(textSelect(LABEL_SHUT_DOWN), textSelect(LABEL_SHUTTING_DOWN));
  }
  lock = false;
}

void menuShutDown(void)
{
  popupDrawPage(bottomDoubleBtn, textSelect(LABEL_SHUT_DOWN), textSelect(LABEL_WAIT_TEMP_SHUT_DOWN), textSelect(LABEL_FORCE_SHUT_DOWN), textSelect(LABEL_CANNEL)); 
  for (ExtUI::fan_t i = ExtUI::FAN0; i < FAN_COUNT; i = (ExtUI::fan_t)(i + 1))
    ExtUI::setTargetFan_percent(100, i);
  menuSetFrontCallBack(menuCallBackShutDown);
}

#endif

