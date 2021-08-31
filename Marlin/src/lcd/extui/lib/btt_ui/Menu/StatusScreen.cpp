#include "../TSC_Menu.h"

#include "../../../../../inc/MarlinConfigPre.h"
#include "../../../../../sd/cardreader.h"
#include "../../../../../feature/powerloss.h"

#if ENABLED(BTT_UI_SPI)
#define STATUS_START_Y (TITLE_END_Y +  0 * ICON_HEIGHT + 0 * SPACE_Y + SSICON_VAL_Y0)

#define TOOL_VAL_SEPARATOR_X     (START_X + 0 * ICON_WIDTH + 0 * SPACE_X + (ICON_WIDTH-BYTE_WIDTH)/2)
#define BED_VAL_SEPARATOR_X      (START_X + 1 * ICON_WIDTH + 1 * SPACE_X + (ICON_WIDTH-BYTE_WIDTH)/2)
#define FAN_VAL_SEPARATOR_X      (START_X + 2 * ICON_WIDTH + 2 * SPACE_X + (ICON_WIDTH-BYTE_WIDTH*4)/2 + BYTE_WIDTH*3)
#define FEEDRATE_VAL_SEPARATOR_X (START_X + 3 * ICON_WIDTH + 3 * SPACE_X + (ICON_WIDTH-BYTE_WIDTH*4)/2 + BYTE_WIDTH*3)

#define POS_START_Y     LCD_HEIGHT_PIXEL/2-3 
#define POSX_START_X    LCD_WIDTH_PIXEL/4+2*BYTE_WIDTH 
#define POSY_START_X    LCD_WIDTH_PIXEL/2-3*BYTE_WIDTH 
#define POSZ_START_X    LCD_WIDTH_PIXEL/2+6*BYTE_WIDTH  

//1 title, ITEM_PER_PAGE items (icon + label) 
MENUITEMS StatusItems = {
// title
LABEL_READY,
// icon                       label
 {{ICON_STATUSNOZZLE,         LABEL_BACKGROUND},
  {ICON_STATUSBED,            LABEL_BACKGROUND},
  {ICON_STATUSFAN,            LABEL_BACKGROUND},
  {ICON_STATUS_SPEED,         LABEL_BACKGROUND},
  {ICON_MAINMENU,             LABEL_MAINMENU},  
  {ICON_BACKGROUND,           LABEL_BACKGROUND}, //Reserved for gantry position to be added later
  {ICON_BACKGROUND,           LABEL_BACKGROUND}, //Reserved for gantry position to be added later
  {ICON_PRINT,                LABEL_PRINT},}
};

const ITEM ToolItems[3] = {
// icon                       label
  {ICON_STATUSNOZZLE,         LABEL_BACKGROUND},
  {ICON_STATUSBED,            LABEL_BACKGROUND},
  {ICON_STATUSFAN,            LABEL_BACKGROUND},
//{ICON_HEAT_STATUS,          LABEL_BACKGROUND},
//{ICON_BED_STATUS,           LABEL_BACKGROUND},
//{ICON_FAN_STATUS,           LABEL_BACKGROUND},
};
const ITEM SpeedItems[2] = {
// icon                       label
  {ICON_STATUS_SPEED,         LABEL_BACKGROUND},
  {ICON_STATUS_FLOW,          LABEL_BACKGROUND},
};

SCROLL     msgScroll;
static const uint8_t *msgbody;

const char* SpeedID[2] = {"Sp.", "Flow"};
// text position rectangles for Live icons 
//icon 0
const GUI_POINT pointID[4] = {
  {1*ICON_WIDTH+0*SPACE_X+START_X - BYTE_WIDTH/2,                   TITLE_END_Y + 0 * ICON_HEIGHT + 0 * SPACE_Y + SSICON_NAME_Y0},
  {2*ICON_WIDTH+1*SPACE_X+START_X - BYTE_WIDTH/2,                   TITLE_END_Y + 0 * ICON_HEIGHT + 0 * SPACE_Y + SSICON_NAME_Y0},
  {3*ICON_WIDTH+2*SPACE_X+START_X - BYTE_WIDTH/2,                   TITLE_END_Y + 0 * ICON_HEIGHT + 0 * SPACE_Y + SSICON_NAME_Y0},
  {4*ICON_WIDTH+3*SPACE_X+START_X - BYTE_WIDTH/2,                   TITLE_END_Y + 0 * ICON_HEIGHT + 0 * SPACE_Y + SSICON_NAME_Y0},
};
const GUI_RECT rectB[4] = {
  {START_X + 0 * ICON_WIDTH + 0 * SPACE_X,  TITLE_END_Y +  0 * ICON_HEIGHT + 0 * SPACE_Y + SSICON_VAL_Y0,
   START_X + 1 * ICON_WIDTH + 0 * SPACE_X,  TITLE_END_Y +  0 * ICON_HEIGHT + 0 * SPACE_Y + SSICON_VAL_Y0 + BYTE_HEIGHT},
  
  {START_X + 1 * ICON_WIDTH + 1 * SPACE_X,  TITLE_END_Y +  0 * ICON_HEIGHT + 0 * SPACE_Y + SSICON_VAL_Y0,
   START_X + 2 * ICON_WIDTH + 1 * SPACE_X,  TITLE_END_Y +  0 * ICON_HEIGHT + 0 * SPACE_Y + SSICON_VAL_Y0 + BYTE_HEIGHT},
  
  {START_X + 2 * ICON_WIDTH + 2 * SPACE_X,  TITLE_END_Y +  0 * ICON_HEIGHT + 0 * SPACE_Y + SSICON_VAL_Y0,
   START_X + 3 * ICON_WIDTH + 2 * SPACE_X,  TITLE_END_Y +  0 * ICON_HEIGHT + 0 * SPACE_Y + SSICON_VAL_Y0 + BYTE_HEIGHT},
  
  {START_X + 3 * ICON_WIDTH + 3 * SPACE_X,  TITLE_END_Y +  0 * ICON_HEIGHT + 0 * SPACE_Y + SSICON_VAL_Y0,
   START_X + 4 * ICON_WIDTH + 3 * SPACE_X,  TITLE_END_Y +  0 * ICON_HEIGHT + 0 * SPACE_Y + SSICON_VAL_Y0 + BYTE_HEIGHT},
};

//info rectangle          
const GUI_RECT RectInfo = {START_X + 1 * ICON_WIDTH + 1 * SPACE_X,  TITLE_END_Y +  1 * ICON_HEIGHT + 1 * SPACE_Y,
                           START_X + 3 * ICON_WIDTH + 2 * SPACE_X,  TITLE_END_Y +  2 * ICON_HEIGHT + 1 * SPACE_Y};

const  GUI_RECT msgRect ={START_X + 1 * ICON_WIDTH + 1 * SPACE_X + 2,   TITLE_END_Y +  1 * ICON_HEIGHT + 1 * SPACE_Y + STATUS_MSG_BODY_YOFFSET,
                          START_X + 3 * ICON_WIDTH + 2 * SPACE_X - 2,   TITLE_END_Y +  2 * ICON_HEIGHT + 1 * SPACE_Y - STATUS_MSG_BODY_BOTTOM};

const GUI_RECT RecGantry = {START_X,                        1*ICON_HEIGHT+0*SPACE_Y+TITLE_END_Y+STATUS_GANTRY_YOFFSET,
                            4*ICON_WIDTH+3*SPACE_X+START_X, 1*ICON_HEIGHT+1*SPACE_Y+TITLE_END_Y-STATUS_GANTRY_YOFFSET};

STATUS_MSG statusMsg;

void statusMsg_GetCurMsg(STATUS_MSG *msg)
{
  msg->curTool = ExtUI::getActiveTool();
  msg->actBed = ExtUI::getActualTemp_celsius(ExtUI::BED);
  msg->tagBed = ExtUI::getTargetTemp_celsius(ExtUI::BED),
  msg->actHotend = ExtUI::getActualTemp_celsius(msg->curTool),
  msg->tagHotend = ExtUI::getTargetTemp_celsius(msg->curTool);
  msg->fanPercent = ExtUI::getTargetFan_percent(ExtUI::FAN0);
  msg->feedRatePercent = ExtUI::getFeedrate_percent();
  // msg->x = ExtUI::getAxisPosition_mm(ExtUI::X),
  // msg->y = ExtUI::getAxisPosition_mm(ExtUI::Y),
  // msg->z = ExtUI::getAxisPosition_mm(ExtUI::Z);
} 


void Main_statusMsg_GetCurMsg(STATUS_MSG *msg)
{
  msg->curTool = ExtUI::getActiveTool();
  msg->actBed = ExtUI::getActualTemp_celsius(ExtUI::BED);
  msg->tagBed = ExtUI::getTargetTemp_celsius(ExtUI::BED),
  msg->actHotend = ExtUI::getActualTemp_celsius(msg->curTool),
  msg->tagHotend = ExtUI::getTargetTemp_celsius(msg->curTool);
  msg->fanPercent = ExtUI::getTargetFan_percent(ExtUI::FAN0);
  msg->feedRatePercent = ExtUI::getFeedrate_percent();
  msg->x = ExtUI::getAxisPosition_mm(ExtUI::X),
  msg->y = ExtUI::getAxisPosition_mm(ExtUI::Y),
  msg->z = ExtUI::getAxisPosition_mm(ExtUI::Z);
} 

void PrintstatusMsg_GetCurMsg(STATUS_MSG *msg)
{
  msg->curTool = ExtUI::getActiveTool();
  msg->actBed = ExtUI::getActualTemp_celsius(ExtUI::BED);
  msg->tagBed = ExtUI::getTargetTemp_celsius(ExtUI::BED),
  msg->actHotend = ExtUI::getActualTemp_celsius(msg->curTool);
  msg->tagHotend = ExtUI::getTargetTemp_celsius(msg->curTool);
  msg->fanPercent = ExtUI::getTargetFan_percent(ExtUI::FAN0);
  msg->feedRatePercent = ExtUI::getFeedrate_percent();
} 

void PrintstatusMsg_GetTANDBEDMsg(STATUS_MSG *msg)
{
  msg->actBed = ExtUI::getActualTemp_celsius(ExtUI::BED);
  msg->actHotend = ExtUI::getActualTemp_celsius(msg->curTool);
}
void PrintstatusMsg_GetT0Tag(STATUS_MSG *msg)
{
  msg->tagHotend = ExtUI::getTargetTemp_celsius(msg->curTool);
} 

void PrintstatusMsg_GetBEDTag(STATUS_MSG *msg)
{
  msg->tagBed = ExtUI::getTargetTemp_celsius(ExtUI::BED);
} 

static void redrawToolAct(void)
{
  GUI_DispDec(TOOL_VAL_SEPARATOR_X - 3*BYTE_WIDTH, STATUS_START_Y, (int16_t)statusMsg.actHotend, 3, RIGHT);
}
static void redrawToolTag(void)
{
  GUI_DispDec(TOOL_VAL_SEPARATOR_X + BYTE_WIDTH, STATUS_START_Y, (int16_t)statusMsg.tagHotend, 3, LEFT);
}

static void redrawBedAct(void)
{
  GUI_DispDec(BED_VAL_SEPARATOR_X - 3*BYTE_WIDTH, STATUS_START_Y, (int16_t)statusMsg.actBed, 3, RIGHT);
}
static void redrawBedTag(void)
{
  GUI_DispDec(BED_VAL_SEPARATOR_X + BYTE_WIDTH, STATUS_START_Y, (int16_t)statusMsg.tagBed, 3, LEFT);
}

static void redrawFan(void)
{
  GUI_DispDec(FAN_VAL_SEPARATOR_X - 3*BYTE_WIDTH, STATUS_START_Y, (int16_t)statusMsg.fanPercent, 3, RIGHT);
}

static void redrawFeedRate(void)
{
  GUI_DispDec(FEEDRATE_VAL_SEPARATOR_X - 3*BYTE_WIDTH, STATUS_START_Y, (int16_t)statusMsg.feedRatePercent, 3, RIGHT);
}

#define XYZ_SPACE   LCD_HEIGHT_PIXEL/2
#define X_SPACE     LCD_WIDTH_PIXEL

static void drawPosition(void)
{
  char tempstrX[20];
  char tempstrY[20];
  char tempstrZ[20];
  int tempx=0;
  int tempy=0;
  int tempz=0;  

  tempx=(abs)((statusMsg.x-(int)statusMsg.x)*100);
  sprintf(tempstrX,"%d.%d", (int)statusMsg.x,tempx);
  GUI_DispString(POSX_START_X+2*BYTE_WIDTH,POS_START_Y,(const uint8_t *)tempstrX);
  tempy=(abs)((statusMsg.y-(int)statusMsg.y)*100);
  sprintf(tempstrY,"%d.%d", (int)statusMsg.y,tempy);
  GUI_DispString(POSY_START_X+2*BYTE_WIDTH,POS_START_Y,(const uint8_t *)tempstrY);
  tempz=(abs)((statusMsg.z-(int)statusMsg.z)*100);
  sprintf(tempstrZ,"%d.%d", (int)statusMsg.z,tempz); 
  GUI_DispString(POSZ_START_X+2*BYTE_WIDTH,POS_START_Y,(const uint8_t *)tempstrZ); 
}

static void  redrawPosition(void)
{
  char tempstrX[20];
  char tempstrY[20];
  char tempstrZ[20];
  int tempx=0;
  int tempy=0;
  int tempz=0;  
  static float Sta_X=0;
  static float Sta_Y=0;
  static float Sta_Z=0;
  
  GUI_SetColor(GANTRYLBL_COLOR);
  if(Sta_X!=statusMsg.x)
  {
    Sta_X=statusMsg.x;
    tempx=(abs)((statusMsg.x-(int)statusMsg.x)*100);
    sprintf(tempstrX,"%d.%d", (int)statusMsg.x,tempx);
    GUI_ClearRect(POSX_START_X+2*BYTE_WIDTH,POS_START_Y,POSX_START_X+8*BYTE_WIDTH,POS_START_Y+BYTE_HEIGHT);
    GUI_DispString(POSX_START_X+2*BYTE_WIDTH,POS_START_Y,(const uint8_t *)tempstrX);
  }
  if(Sta_Y!=statusMsg.y)
  {
    Sta_Y=statusMsg.y;
    tempy=(abs)((statusMsg.y-(int)statusMsg.y)*100);
    sprintf(tempstrY,"%d.%d", (int)statusMsg.y,tempy);
    GUI_ClearRect(POSY_START_X+2*BYTE_WIDTH,POS_START_Y,POSY_START_X+8*BYTE_WIDTH,POS_START_Y+BYTE_HEIGHT);
    GUI_DispString(POSY_START_X+2*BYTE_WIDTH,POS_START_Y,(const uint8_t *)tempstrY);
  }
  if(Sta_Z!=statusMsg.z)
  {
    Sta_Z=statusMsg.z;
    tempz=(abs)((statusMsg.z-(int)statusMsg.z)*100);
    sprintf(tempstrZ,"%d.%d", (int)statusMsg.z,tempz); 
    GUI_ClearRect(POSZ_START_X+2*BYTE_WIDTH,POS_START_Y,POSZ_START_X+8*BYTE_WIDTH,POS_START_Y+BYTE_HEIGHT);
    GUI_DispString(POSZ_START_X+2*BYTE_WIDTH,POS_START_Y,(const uint8_t *)tempstrZ); 
  }

  GUI_RestoreColorDefault();
}

static void drawStatus(void)
{
  Main_statusMsg_GetCurMsg(&statusMsg);
  
  GUI_SetTextMode(GUI_TEXTMODE_TRANS);
  GUI_SetColor(HEADING_COLOR);
  GUI_DispStringRight(pointID[0].x, pointID[0].y, (uint8_t *)heatDisplayID[statusMsg.curTool+1]); // Ext label
  GUI_DispStringRight(pointID[1].x, pointID[1].y, (uint8_t *)heatDisplayID[0]); // Bed label
  GUI_DispStringRight(pointID[2].x, pointID[2].y, (uint8_t *)fanID[0]); // Fan label
  GUI_DispStringRight(pointID[3].x, pointID[3].y, (uint8_t *)SpeedID[0]); // Speed / Flow label
  GUI_SetColor(VAL_COLOR);
  GUI_SetBkColor(WHITE);
 // GUI_ClearPrect(&rectB[0]);
  GUI_DispString(TOOL_VAL_SEPARATOR_X, STATUS_START_Y, (uint8_t *)"/"); // Ext value
  redrawToolAct();
  redrawToolTag();
 // GUI_ClearPrect(&rectB[1]);
  GUI_DispString(BED_VAL_SEPARATOR_X, STATUS_START_Y, (uint8_t *)"/"); // Bed value
  redrawBedAct();
  redrawBedTag();
 // GUI_ClearPrect(&rectB[2]);
  GUI_DispString(FAN_VAL_SEPARATOR_X, STATUS_START_Y, (uint8_t *)"%"); // Fan speed percent
  redrawFan();
 // GUI_ClearPrect(&rectB[3]);
  GUI_DispString(FEEDRATE_VAL_SEPARATOR_X, STATUS_START_Y, (uint8_t *)"%"); // Feedrate percent
  redrawFeedRate();
  
  GUI_SetColor(GANTRYLBL_COLOR);
  GUI_DispString(POSX_START_X, POS_START_Y, (uint8_t *)"X:");
  GUI_DispString(POSY_START_X, POS_START_Y, (uint8_t *)"Y:");
  GUI_DispString(POSZ_START_X, POS_START_Y, (uint8_t *)"Z:");
  drawPosition();
  //redrawPosition();

  GUI_RestoreColorDefault();
}

void loopStatusCheck(void)
{
  STATUS_MSG tempMsg;
  Main_statusMsg_GetCurMsg(&tempMsg);

  // Position & actual temperature refresh per 1 sec
  static uint32_t nowTime_ms = 0;
  if (millis() - nowTime_ms > 1000) { // Refresh per 1 sec
    nowTime_ms = millis();
    // Refresh position
    if (statusMsg.x != tempMsg.x || statusMsg.y != tempMsg.y || statusMsg.z != tempMsg.z){
      statusMsg.x = tempMsg.x;
      statusMsg.y = tempMsg.y;
      statusMsg.z = tempMsg.z;
      redrawPosition();
    }

    GUI_SetColor(VAL_COLOR);
    GUI_SetBkColor(WHITE);
    // Refresh temperature
    //  if (tool != tempTool)
    if (statusMsg.actHotend != tempMsg.actHotend) {
      statusMsg.actHotend = tempMsg.actHotend;
      redrawToolAct();
    }
    
    if (statusMsg.actBed != tempMsg.actBed) {
      statusMsg.actBed = tempMsg.actBed;
      redrawBedAct();
    }
  }
  // Target temperature & fan speed & feedrate refresh in real time
  GUI_SetColor(VAL_COLOR);
  GUI_SetBkColor(WHITE);
  if (statusMsg.tagHotend != tempMsg.tagHotend) {
    statusMsg.tagHotend = tempMsg.tagHotend;
    redrawToolTag();
  }
  if (statusMsg.tagBed != tempMsg.tagBed) {
    statusMsg.tagBed = tempMsg.tagBed;
    redrawBedTag();
  }
  if (statusMsg.fanPercent != tempMsg.fanPercent) {
    statusMsg.fanPercent = tempMsg.fanPercent;
    redrawFan();
  }
  if (statusMsg.feedRatePercent != tempMsg.feedRatePercent) {
    statusMsg.feedRatePercent = tempMsg.feedRatePercent;
    redrawFeedRate();
  }

  GUI_RestoreColorDefault();
}


static bool msgNeedUpdate = true;
void statusScreen_setMsg(const uint8_t *title, const uint8_t *msg)
{
  msgbody = msg;
  msgNeedUpdate = true;
}

void redrawStatusMsg(void)
{
  GUI_SetBkColor(INFOMSG_BKCOLOR);
  GUI_ClearPrect(&msgRect);  
  Scroll_CreatePara(&msgScroll, (uint8_t *)msgbody, &msgRect);
  GUI_SetBkColor(BK_COLOR);
}

void drawStatusScreenMsg(void)
{
  STATUS_ReadDisplay(RectInfo.x0+2,RectInfo.y0);
  Scroll_CreatePara(&msgScroll, (uint8_t *)msgbody, &msgRect);
}

void scrollMsg(void){
  GUI_SetBkColor(INFOMSG_BKCOLOR);
  GUI_SetColor(INFOMSG_COLOR);
  Scroll_DispString(&msgScroll, CENTER);
  GUI_RestoreColorDefault();
}

void menuCallStatus(void)
{
  KEY_VALUES key_num = menuKeyGetValue();
   
  switch (key_num)
  {
    case KEY_ICON_0:
      heatSetCurHeaterIndex(statusMsg.curTool+1); // Tool, Nozzle
      infoMenu.menu[++infoMenu.cur] = menuHeat;
      break;
    case KEY_ICON_1:
      heatSetCurHeaterIndex(0); // Bed
      infoMenu.menu[++infoMenu.cur] = menuHeat;
      break;
    case KEY_ICON_2:
      infoMenu.menu[++infoMenu.cur] = menuFan;
      break;
    case KEY_ICON_3:
      infoMenu.menu[++infoMenu.cur] = menuSpeed;
      break;
    case KEY_ICON_4:
      infoMenu.menu[++infoMenu.cur] = menuMain;
      break;
    case KEY_ICON_7:
      infoMenu.menu[++infoMenu.cur]= menuPrintSelct;
      break;
    default:
    break;
  }
 
  if (msgNeedUpdate) {
    msgNeedUpdate = false;
    redrawStatusMsg();
  }
  
  scrollMsg();
  
  loopStatusCheck();  
  
}


void menuStatus()
{
  menuDrawPage(&StatusItems);
  drawStatus();
  drawStatusScreenMsg();
  msgNeedUpdate = false;
  queue.enqueue_one_now(PSTR("M420 S1"));
  queue.enqueue_one_now(PSTR("M420 Z30"));
  #ifdef FILAMENT_RUNOUT_SENSOR
  infoSettings.runout ? storeCmd(PSTR("M412 S1")) : storeCmd(PSTR("M412 S0"));
  #endif
  #ifdef POWER_LOSS_RECOVERY
  infoSettings.power_loss ? storeCmd(PSTR("M413 S1")) : storeCmd(PSTR("M413 S0"));
  #endif
  menuSetFrontCallBack(menuCallStatus);
}


void menuCallBackIsSettings(void)
{
  uint16_t key_num = KEY_GetValue(2, doubleBtnRect);
  switch(key_num)
  {
    case KEY_POPUP_CONFIRM:
     storePara();
     volumeReminderMessage(LABEL_EEPROM_SAVE_SUCCESS, STATUS_NORMAL);
     infoMenu.cur--;
     infoMenu.cur--;
    break;

    case KEY_POPUP_CANCEL:	
     infoMenu.cur--;
    break;		
  }
}

void menuIsSettings(void)
{
  popupDrawPage(bottomDoubleBtn, textSelect(LABEL_WARNING), textSelect(LABEL_IS_LANGUAGE), textSelect(LABEL_CONFIRM), textSelect(LABEL_CANNEL));
  menuSetFrontCallBack(menuCallBackIsSettings);
}

#endif

