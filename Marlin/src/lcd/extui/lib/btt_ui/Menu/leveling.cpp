#include "../../../../../inc/MarlinConfigPre.h"
#if ENABLED(BTT_UI_SPI)
#include "../TSC_Menu.h"


#ifdef AUTO_BED_LEVELING_BILINEAR

#include "../../../../../feature/bltouch.h"

#ifdef  FIX_MOUNTED_PROBE

//leveling progress
typedef enum
{
  LEVEL_STEP_0,
  LEVEL_STEP_1,
  LEVEL_STEP_2,
  LEVEL_STEP_3,
  LEVEL_STEP_4
}LEVEL_STEP;

#define LEVEL_CLEAR_PREHEAT_TEMP  180 //Preheating temperature
#define LEVEL_PREHEAT_TEMP        LEVELING_NOZZLE_TEMP //Preheating temperature
#define LEVEL_PREHEAT_BED         LEVELING_BED_TEMP  
#define LEVEL_CLEAR_LIN           3
#define LEVEL_EXTRUSION_WAIT_TIME 5000

MENUITEMS LevelingItems = {
// title
LABEL_ABL,
// icon                        label
{ {ICON_ABLB1,           LABEL_HEAT},
  {ICON_ABLB2,           LABEL_UNLOAD},
  {ICON_ABLB3,           LABEL_COOLDOWN},
  {ICON_ABLB4,           LABEL_LEVELING_ING},
  {ICON_BACKGROUND,      LABEL_BACKGROUND},
  {ICON_BACKGROUND,      LABEL_BACKGROUND},
  {ICON_BACKGROUND,      LABEL_BACKGROUND},
  {ICON_EM_STOP,         LABEL_EMERGENCYSTOP},}
};

#define ITEM_LEVELING_LEN_NUM 8
const ITEM itemLevelingLen[ITEM_LEVELING_LEN_NUM] = {
//icon          label
  {ICON_ABLB1,           LABEL_HEAT},
  {ICON_ABLB2,           LABEL_UNLOAD},
  {ICON_ABLB3,           LABEL_COOLDOWN},
  {ICON_ABLB4,           LABEL_LEVELING_ING},
  {ICON_ABLR1,           LABEL_HEAT},
  {ICON_ABLR2,           LABEL_UNLOAD},
  {ICON_ABLR3,           LABEL_COOLDOWN},
  {ICON_ABLR4,           LABEL_LEVELING_ING},
};

#define INFO_NORMAL_COLOR  WHITE
#define INFO_RUNING_COLOR  RED
#define INFO_RECT_COLOR    GBLUE
#define LEVEL_PROGRESS_INFO_SX     LCD_WIDTH_PIXEL/2+BYTE_WIDTH 
#define LEVEL_PROGRESS_INFO_SY     LCD_HEIGHT_PIXEL/2+BYTE_HEIGHT*2 
#define LEVEL_PROGRESS_INFO_EX     LEVEL_PROGRESS_INFO_SX+ICON_WIDTH+BYTE_WIDTH*2
#define LEVEL_PROGRESS_INFO_EY     LEVEL_PROGRESS_INFO_SY+ICON_HEIGHT+BYTE_HEIGHT*2
#define LEVEL_RPOGRESS_INFO_STEPX  LEVEL_PROGRESS_INFO_SX+BYTE_WIDTH     
#define LEVEL_RPOGRESS_INFO_STEPY  LEVEL_PROGRESS_INFO_SY+BYTE_HEIGHT

void DrawLevelProgressInfo()
{
 GUI_SetColor(INFO_RECT_COLOR);
 GUI_DrawRect(LEVEL_PROGRESS_INFO_SX,LEVEL_PROGRESS_INFO_SY,LEVEL_PROGRESS_INFO_EX,LEVEL_PROGRESS_INFO_EY);
 GUI_SetColor(INFO_RUNING_COLOR);
 GUI_DispString(LEVEL_RPOGRESS_INFO_STEPX , LEVEL_RPOGRESS_INFO_STEPY, (uint8_t *)">>T:180 B:60");
 GUI_SetColor(INFO_NORMAL_COLOR);
 GUI_DispString(LEVEL_RPOGRESS_INFO_STEPX , LEVEL_RPOGRESS_INFO_STEPY+BYTE_HEIGHT, (uint8_t *)"  E:-3.00");
 GUI_DispString(LEVEL_RPOGRESS_INFO_STEPX , LEVEL_RPOGRESS_INFO_STEPY+BYTE_HEIGHT*2, (uint8_t *)"  T:140 B:60");
 GUI_DispString(LEVEL_RPOGRESS_INFO_STEPX , LEVEL_RPOGRESS_INFO_STEPY+BYTE_HEIGHT*3, (uint8_t *)"  G28");
 GUI_DispString(LEVEL_RPOGRESS_INFO_STEPX , LEVEL_RPOGRESS_INFO_STEPY+BYTE_HEIGHT*4, (uint8_t *)"  G29");
 GUI_DispString(LEVEL_RPOGRESS_INFO_STEPX , LEVEL_RPOGRESS_INFO_STEPY+BYTE_HEIGHT*5, (uint8_t *)"  M500");
}
static uint16_t save_level_progress=LEVEL_STEP_0;
void reDrawLevelProgressInfo()
{
  switch(save_level_progress)
  {
    case LEVEL_STEP_2: 
      GUI_SetColor(INFO_RUNING_COLOR);
      GUI_DispString(LEVEL_RPOGRESS_INFO_STEPX , LEVEL_RPOGRESS_INFO_STEPY+BYTE_HEIGHT, (uint8_t *)">>E:-3.00");
      GUI_SetColor(INFO_NORMAL_COLOR);
      GUI_DispString(LEVEL_RPOGRESS_INFO_STEPX , LEVEL_RPOGRESS_INFO_STEPY, (uint8_t *)"  T:180 B:60");    
      break; 
    case LEVEL_STEP_3: 
      GUI_SetColor(INFO_RUNING_COLOR);
      GUI_DispString(LEVEL_RPOGRESS_INFO_STEPX , LEVEL_RPOGRESS_INFO_STEPY+BYTE_HEIGHT*2, (uint8_t *)">>T:140 B:60");
      GUI_SetColor(INFO_NORMAL_COLOR);
      GUI_DispString(LEVEL_RPOGRESS_INFO_STEPX , LEVEL_RPOGRESS_INFO_STEPY+BYTE_HEIGHT*1, (uint8_t *)"  E:-3.00");       
      break; 
    case LEVEL_STEP_4: 
      if(!G28_flag)
      {
        GUI_SetColor(INFO_RUNING_COLOR);
        GUI_DispString(LEVEL_RPOGRESS_INFO_STEPX , LEVEL_RPOGRESS_INFO_STEPY+BYTE_HEIGHT*3, (uint8_t *)">>G28");
        GUI_SetColor(INFO_NORMAL_COLOR);
        GUI_DispString(LEVEL_RPOGRESS_INFO_STEPX , LEVEL_RPOGRESS_INFO_STEPY+BYTE_HEIGHT*2, (uint8_t *)"  T:140 B:60");   
      }
      break; 
     default:break;    
  }
}

#define LEVEL_INFO_SX        BYTE_WIDTH 
#define LEVEL_INFO_SY        LCD_HEIGHT_PIXEL/2+BYTE_HEIGHT*2   
#define LEVEL_INFO_EX        LCD_WIDTH_PIXEL/2-BYTE_WIDTH*2
#define LEVEL_INFO_EY        LEVEL_PROGRESS_INFO_SY+ICON_HEIGHT+BYTE_HEIGHT*2
#define LEVEL_INFO_REMINDX   LEVEL_INFO_SX+BYTE_WIDTH
#define LEVEL_INFO_REMINDY   LEVEL_INFO_SY+BYTE_HEIGHT
#define LEVEL_INFO_STATUSSX  LEVEL_INFO_SX+BYTE_WIDTH
#define LEVEL_INFO_STATUSSY  LEVEL_INFO_SY+BYTE_HEIGHT*4+BYTE_HEIGHT/2
#define LEVEL_INFO_STATUSEX  LEVEL_INFO_EX-BYTE_WIDTH
#define LEVEL_INFO_STATUSEY  LEVEL_INFO_SY+BYTE_HEIGHT*4+BYTE_HEIGHT/2

static bool level_msgNeedUpdate = true;
static const uint8_t *level_msgbody;
SCROLL level_msgScroll;
const  GUI_RECT level_msgRect ={LEVEL_INFO_STATUSSX,LEVEL_INFO_STATUSSY,LEVEL_INFO_STATUSEX,LEVEL_INFO_STATUSEY};

void DrawLevelStatusInfo()
{
  GUI_SetColor(INFO_RECT_COLOR);
  GUI_DrawRect(LEVEL_INFO_SX,LEVEL_INFO_SY,LEVEL_INFO_EX,LEVEL_INFO_EY);
  GUI_SetColor(INFO_RUNING_COLOR);
  GUI_DispString(LEVEL_INFO_REMINDX , LEVEL_INFO_REMINDY, textSelect(LABEL_CLEARNOZZ));
  GUI_SetColor(INFO_NORMAL_COLOR);
  GUI_SetBkColor(BLACK);
  GUI_ClearPrect(&level_msgRect); 
  Scroll_CreatePara(&level_msgScroll, (uint8_t *)level_msgbody, &level_msgRect);
}

void level_setMsg(const uint8_t *title, const uint8_t *msg)
{
  level_msgbody = msg;
  level_msgNeedUpdate = true;
}
void level_scrollMsg(void){
  GUI_SetBkColor(BLACK);
  GUI_SetColor(INFO_NORMAL_COLOR);
  Scroll_DispString(&level_msgScroll, CENTER);
  //GUI_RestoreColorDefault();
}
void reDrawLevelStatusInfo()
{
  GUI_SetBkColor(BLACK);
  GUI_SetColor(INFO_NORMAL_COLOR);
  GUI_ClearPrect(&level_msgRect);  
  Scroll_CreatePara(&level_msgScroll, (uint8_t *)level_msgbody, &level_msgRect);
  //GUI_SetBkColor(WHITE);
}


static uint16_t level_progress=LEVEL_STEP_0;
void menuCallBackLeveling()
{
  static uint32_t Level_Extrusion_WaitTime = 0;
  
  KEY_VALUES key_num = menuKeyGetValue();
  switch(key_num)
  {
    case KEY_ICON_7:
     flashFirmware(1);
    break;
    default:break;
  }
  
  switch(level_progress)
  {
     case LEVEL_STEP_0: 
        thermalManager.setTargetHotend(LEVEL_CLEAR_PREHEAT_TEMP, 0);
        thermalManager.setTargetBed(LEVEL_PREHEAT_BED);
        level_progress=LEVEL_STEP_1;
     break; 
    case LEVEL_STEP_1:  //Wait for the nozzle to warm up to 180 degrees, and then start Ji
       if(ExtUI::getActualTemp_celsius(ExtUI::E0)>=(LEVEL_CLEAR_PREHEAT_TEMP-1))
       {
         //queue.enqueue_one_now("G91");
         ExtUI::setAxisPosition_mm(ExtUI::getAxisPosition_mm(ExtUI::E0) - LEVEL_CLEAR_LIN, ExtUI::E0, 60);
         //queue.enqueue_one_now("G90");
         level_progress=LEVEL_STEP_2;
         Level_Extrusion_WaitTime= millis();
         LevelingItems.items[0] = itemLevelingLen[0];
         LevelingItems.items[1] = itemLevelingLen[5];
         menuDrawItem(&LevelingItems.items[0], 0);
         menuDrawItem(&LevelingItems.items[1], 1);
       }
     break; 
    case LEVEL_STEP_2:  //Wait for a while after squeezing out
       if((millis()-Level_Extrusion_WaitTime)>=LEVEL_EXTRUSION_WAIT_TIME)
       { 
         thermalManager.setTargetHotend(LEVEL_PREHEAT_TEMP, 0);
         thermalManager.setTargetBed(LEVEL_PREHEAT_BED);
         level_progress=LEVEL_STEP_3;
         LevelingItems.items[1] = itemLevelingLen[1];
         LevelingItems.items[2] = itemLevelingLen[6];
         menuDrawItem(&LevelingItems.items[1], 1);
         menuDrawItem(&LevelingItems.items[2], 2);
       }
     break; 
    case LEVEL_STEP_3:
        if((ExtUI::getActualTemp_celsius(ExtUI::E0)>=(LEVEL_PREHEAT_TEMP-5))&&(ExtUI::getActualTemp_celsius(ExtUI::E0)<=(LEVEL_PREHEAT_TEMP+5)))
      {
        if((ExtUI::getActualTemp_celsius(ExtUI::BED)>=(LEVEL_PREHEAT_BED)-1)&&(ExtUI::getActualTemp_celsius(ExtUI::BED)<=(LEVEL_PREHEAT_BED+3)))
        {
          G28_flag=false;
          queue.enqueue_one_now("G28");
          queue.enqueue_one_now("G29");
          level_progress=LEVEL_STEP_4;
          LevelingItems.items[2] = itemLevelingLen[2];
          LevelingItems.items[3] = itemLevelingLen[7];
          menuDrawItem(&LevelingItems.items[2], 2);
          menuDrawItem(&LevelingItems.items[3], 3);
        }
      }
     break; 
    case LEVEL_STEP_4:
      if(G28_flag)
      {
        G28_flag=false;
        GUI_SetColor(GBLUE);
        GUI_DispString(LEVEL_RPOGRESS_INFO_STEPX , LEVEL_RPOGRESS_INFO_STEPY+BYTE_HEIGHT*4, (uint8_t *)">>G29");
        GUI_SetColor(WHITE);
        GUI_DispString(LEVEL_RPOGRESS_INFO_STEPX , LEVEL_RPOGRESS_INFO_STEPY+BYTE_HEIGHT*3, (uint8_t *)"  G28");    
      }
      if(G29_flag)
      {
        G29_flag=false;
        GUI_SetColor(GBLUE);
        GUI_DispString(LEVEL_RPOGRESS_INFO_STEPX , LEVEL_RPOGRESS_INFO_STEPY+BYTE_HEIGHT*5, (uint8_t *)">>M500");
        GUI_SetColor(WHITE);
        GUI_DispString(LEVEL_RPOGRESS_INFO_STEPX , LEVEL_RPOGRESS_INFO_STEPY+BYTE_HEIGHT*4, (uint8_t *)"  G29"); 
        queue.enqueue_one_now("M500");
        LevelingItems.items[3] = itemLevelingLen[3];
        infoMenu.cur--; 
        level_progress=LEVEL_STEP_0;
      }
      
     break; 
    default:break;    
  }

  if(save_level_progress!=level_progress)
  {
    save_level_progress=level_progress;
    reDrawLevelProgressInfo();
  }
  if(level_msgNeedUpdate)
  {
    level_msgNeedUpdate=false;
    reDrawLevelStatusInfo();
  }
    level_scrollMsg();
}

void menuLeveling()
{
  LevelingItems.items[0] = itemLevelingLen[4];
  menuDrawPage(&LevelingItems);
  DrawLevelStatusInfo();
  DrawLevelProgressInfo();
  menuSetFrontCallBack(menuCallBackLeveling);
}
#endif

#ifdef BLTOUCH
    
const MENUITEMS BltouchItems = {
// title
LABEL_BLTOUCH,
// icon                        label
{ {ICON_BLTOUCH_DEPLOY,        LABEL_BLTOUCH_DEPLOY},
  {ICON_BLTOUCH_STOW,          LABEL_BLTOUCH_STOW},
  {ICON_BLTOUCH_TEST,          LABEL_BLTOUCH_TEST},
  {ICON_BACKGROUND,            LABEL_BACKGROUND},
  {ICON_BACKGROUND,            LABEL_BACKGROUND},
  {ICON_BACKGROUND,            LABEL_BACKGROUND},
  {ICON_BACKGROUND,            LABEL_BACKGROUND},
  {ICON_BACK,                  LABEL_BACK},}
};

void menuCallBackAutoBltouch(void)
{
  KEY_VALUES key_num = menuKeyGetValue();
  switch(key_num)
  {
    case KEY_ICON_0:
      storeCmd("M280 P0 S10\n");
    break;
    case KEY_ICON_1:
      storeCmd("M280 P0 S90\n");
    break;
    case KEY_ICON_2:
      storeCmd("M280 P0 S120\n");
    break;

    case KEY_ICON_7:
    infoMenu.cur--; 
    break;
    default:break;
  }
}

void menuBltouch()
{
  menuDrawPage(&BltouchItems);
  menuSetFrontCallBack(menuCallBackAutoBltouch);
}
#endif



const MENUITEMS autoLevelingItems = {
// title
LABEL_ABL,
// icon                        label
 {{ICON_LEVELING,              LABEL_ABL},
  {ICON_X_HOME,                LABEL_X_HOME},
  {ICON_BACKGROUND,            LABEL_BACKGROUND},
 /*{ICON_MVALID,               LABEL_MESH_VALID},*/
  #ifdef BLTOUCH
   {ICON_BLTOUCH,              LABEL_BLTOUCH},
  #else
   {ICON_BACKGROUND,           LABEL_BACKGROUND},
  #endif
  {ICON_BACKGROUND,            LABEL_BACKGROUND},
  {ICON_BABYSTEP,              LABEL_BABYSTEP},
  {ICON_EM_STOP,               LABEL_EMERGENCYSTOP},
  {ICON_BACK,                  LABEL_BACK},}
};

void menuCallBackAutoBedLeveling(void)
{
  KEY_VALUES key_num = menuKeyGetValue();
  switch(key_num)
  {
    case KEY_ICON_0:
      #ifdef  FIX_MOUNTED_PROBE
        infoMenu.menu[++infoMenu.cur] = menuLeveling;
      #else
         storeCmd("G28");
         storeCmd("G29");
         storeCmd("M500");
      #endif
      break;
    case KEY_ICON_1:
      storeCmd("G28 X");
      break;
    #ifdef BLTOUCH
      case KEY_ICON_3:
      infoMenu.menu[++infoMenu.cur] = menuBltouch;
      break;    
    #endif
    // case KEY_ICON_2:
    //   mustStoreCmd("G28\n");
    //   infoMenu.menu[++infoMenu.cur] = menuMeshValid;
    //   break;
    case KEY_ICON_5:
      infoMenu.menu[++infoMenu.cur] = menuBabyStep;
      break;
    case KEY_ICON_6:
      flashFirmware(1);
      break;
    case KEY_ICON_7:
      infoMenu.cur--; 
      break;
    default:break;
  }
}

void menuAutoLeveling()
{
  menuDrawPage(&autoLevelingItems);
  menuSetFrontCallBack(menuCallBackAutoBedLeveling);
}

#endif
#endif
