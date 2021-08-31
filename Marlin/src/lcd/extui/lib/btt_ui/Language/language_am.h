#ifndef _LANGUAGE_AM_H_
#define _LANGUAGE_AM_H_
#include "../../../../../inc/MarlinConfigPre.h"
#if ENABLED(BTT_UI_SPI)
    #define AM_LANGUAGE             "Հայերեն"
    #define AM_HEAT                 "Ջերմություն"
    #define AM_MOVE                 "Շարժել"
    #define AM_HOME                 "Սկզբնական"
    #define AM_PRINT                "Տպել"
    #define AM_EXTRUDE              "Հալել"
    #define AM_FAN                  "Հովացնել"
    #define AM_SETTINGS             "Կարգավորումներ"
    #define AM_SCREEN_SETTINGS      "Screen"
    #define AM_MACHINE_SETTINGS     "Machine"
    #define AM_FEATURE_SETTINGS     "Feature"
    #define AM_SILENT_ON            "Silent On"
    #define AM_SILENT_OFF           "Silent Off"
    #define AM_SHUT_DOWN            "Shut Down"
    #define AM_RGB_SETTINGS         "LED Color"
    #define AM_RGB_RED              "Red"
    #define AM_RGB_GREEN            "Green"
    #define AM_RGB_BLUE             "Blue"
    #define AM_RGB_WHITE            "White"
    #define AM_RGB_OFF              "LED Off"
    #define AM_GCODE                "Gcode"
    #define AM_CUSTOM               "Custom"
    #define AM_LEVELING             "Leveling"
    #define AM_POINT_1              "Point 1"
    #define AM_POINT_2              "Point 2"
    #define AM_POINT_3              "Point 3"
    #define AM_POINT_4              "Point 4"
    #define AM_ABL                  "ABL"
    #define AM_BLTOUCH              "Bltouch"
    #define AM_BLTOUCH_TEST         "Test"
    #define AM_BLTOUCH_DEPLOY       "Deploy"
    #define AM_BLTOUCH_STOW         "Stow"
    #define AM_BLTOUCH_REPEAT       "Repeat"
    #define AM_PROBE_OFFSET         "Offset"
    #define AM_EEPROM_SAVE          "Save"
    #define AM_INC                  "Ավելացնել"
    #define AM_DEC                  "Նվազեցնել"
    #define AM_NOZZLE               "Գլխիկ"
    #define AM_BED                  "Տախտակ"
    #define AM_STOP                 "Դադարեցնել"
    #define AM_BACK                 "Հետ"
    #define AM_PAGE_UP              "Վերեւ"
    #define AM_PAGE_DOWN            "Ներքեւ"
    #define AM_PAUSE                "Կանգնեցնել"
    #define AM_RESUME               "Շարունակել"
    #define AM_LOAD                 "Load"
    #define AM_UNLOAD               "Unload"
    #define AM_SLOW_SPEED           "Դանդաղ"
    #define AM_NORMAL_SPEED         "Նորմալ"
    #define AM_FAST_SPEED           "Արագ"
    #define AM_FAN_FULL_SPEED       "Ամբողջը"
    #define AM_FAN_HALF_SPEED       "Կիսատ"
    #define AM_ROTATE_UI            "Rotate UI"
    #define AM_TOUCHSCREEN_ADJUST   "TSC Adjust"
    #define AM_MORE                 "More"
    #define AM_SCREEN_INFO          "Info"
    #define AM_WHITE                "White"
    #define AM_BLACK                "Black"
    #define AM_BLUE                 "Blue"
    #define AM_RED                  "Red"
    #define AM_GREEN                "Green"
    #define AM_CYAN                 "Cyan"
    #define AM_YELLOW               "Yellow"
    #define AM_BROWN                "Brown"
    #define AM_GRAY                 "Gray"
    #define AM_DISCONNECT           "Անջատել"
    #define AM_BAUDRATE_115200      "115200"
    #define AM_BAUDRATE_250000      "250000"
    #define AM_RUNOUT_OFF           "Runout OFF"
    #define AM_RUNOUT_ON            "Runout ON"
    #define AM_SMART_RUNOUT_ON      "Smart ON"
    #define AM_PERCENTAGE           "Percentage"
    #define AM_BABYSTEP             "Դանդաղ"
    #define AM_PERCENTAGE_SPEED     "Արագություն"
    #define AM_PERCENTAGE_FLOW      "Flow"
    #define AM_VALUE_ZERO           "Zero"
    #define AM_1_DEGREE             "1℃"
    #define AM_5_DEGREE             "5℃"
    #define AM_10_DEGREE            "10℃"
    #define AM_X_INC                "X+"
    #define AM_Y_INC                "Y+"
    #define AM_Z_INC                "Z+"
    #define AM_X_DEC                "X-"
    #define AM_Y_DEC                "Y-"
    #define AM_Z_DEC                "Z-"
    #define AM_X_HOME               "X"
    #define AM_Y_HOME               "Y"
    #define AM_Z_HOME               "Z"
    #define AM_001_MM               "0.01mm"
    #define AM_01_MM                "0.1mm"
    #define AM_1_MM                 "1mm"
    #define AM_5_MM                 "5mm"
    #define AM_10_MM                "10mm"
    #define AM_100_MM               "100mm"
    #define AM_1_PERCENT            "1%"
    #define AM_5_PERCENT            "5%"
    #define AM_10_PERCENT           "10%"
    #define AM_READY                "Պատրաստ"
    #define AM_BUSY                 "Ծանրաբեռնված է"
    #define AM_UNCONNECTED          "Տպիչը կցված չէ"
    #define AM_DISCONNECT_INFO      "Now you can control the printer by your computer!"
    #define AM_LOADING              "Բեռնում է..."  
    #define AM_POWER_FAILED         "Տպելը չի ավարտվել, շարունակե՞լ"
    #define AM_CONTINUE             "Շարունակել"
    #define AM_CANNEL               "Չեղարկել"
    #define AM_ADJUST_TITLE         "Էկրանի կարգավորում"
    #define AM_ADJUST_INFO          "Please click on the red dot"
    #define AM_ADJUST_OK            "Adjustment Success"
    #define AM_ADJUST_FAILED        "Adjustment failed, Please Try Again"
    #define AM_WARNING              "Warning"
    #define AM_STOP_PRINT           "Stop printing?"
    #define AM_CONFIRM              "Confirm"
    #define AM_TFTSD                "TFT SD"
    #define AM_READ_TFTSD_ERROR     "TFTSD հիշողության սարքը չկա"
    #define AM_TFTSD_INSERTED       "Card inserted!"
    #define AM_TFTSD_REMOVED        "Card removed!"    
    #define AM_U_DISK               "U Disk"
    #define AM_READ_U_DISK_ERROR    "Read U Disk error!"
    #define AM_U_DISK_INSERTED      "U Disk inserted!"
    #define AM_U_DISK_REMOVED       "U Disk removed!"    
    #define AM_ONBOARDSD            "OnboardSD"
    #define AM_READ_ONBOARDSD_ERROR "OnboardSD հիշողության սարքը չկա"    
    #define AM_FILAMENT_RUNOUT      "Filament runout!"
    #define AM_PREHEAT              "Preheat"
    #define AM_PREHEAT_BOTH         "Both"
    #define AM_PREHEAT_PLA          "PLA"
    #define AM_PREHEAT_PETG         "PETG"
    #define AM_PREHEAT_ABS          "ABS"
    #define AM_PREHEAT_CUSTOM1      "CUSTOM1"
    #define AM_PREHEAT_CUSTOM2      "CUSTOM2"
    #define AM_IS_PAUSE             "Unable to extrude when printing, Pause printing?"
    #define AM_AUTO_SHUT_DOWN       "Auto Power"
    #define AM_MANUAL_SHUT_DOWN     "Manual"
    #define AM_UNIFIEDMOVE          "Movemeny"
    #define AM_UNIFIEDHEAT          "Heat&Fan"
    #define AM_COOLDOWN             "Cool Down"
    #define AM_EMERGENCYSTOP        "EM. STOP"
    #define AM_TOUCH_TO_EXIT        "Touch anywhere to exit"
    #define AM_MAINMENU             "Menu"
    #define AM_WAIT_TEMP_SHUT_DOWN  "Wait for the temperature of hotend to be lower than " STRINGIFY(AUTO_SHUT_DOWN_MAXTEMP) "℃" // Wait for the temperature of hotend to be lower than 50℃
    #define AM_FORCE_SHUT_DOWN      "Force"
    #define AM_SHUTTING_DOWN        "Shutting down..."
    #define AM_PRINTING_COMPLETED   "Printing completed!"
    #define AM_STATUS_INFO          "Info"
    #define AM_ERROR                "Error"
    #define AM_TIPS                 "Tips"
    #define AM_EEPROM_SAVE_SUCCESS  "EEPROM parameters saved successfully"
    #define AM_EEPROM_SAVE_FAILED   "EEPROM parameters saved failed"
    #define AM_SOFTENING            "Փափկեցնող ծախսվող նյութեր"
    #define AM_DEFAULT              "զրոյացնել"
    #define AM_IS_DEFAULT           "Գործարանի կարգավորումները վերականգնելու համար հաստատո՞ւմ եք:"
    #define AM_DEFAULT_OK           "Գործարանային կարգավորումները վերականգնվել են:"
    #define AM_LEVEL_TEMP           "Սպասեք, որ ջեռուցման գավազանը կայունանա մինչեւ 140 աստիճան"
    #define AM_PSTOP                "Սպասեք զրոյացման ավարտին.."
    #define AM_LEVEL_BED_TEMP       "Տաք մահճակալը կայունանում է մինչեւ 60 աստիճան"
    #define AM_LEVELING_OVEL        "Հավասարեցումն ավարտված է:"
    #define AM_LEVELING_ING         "Հավասարեցում.."
    #define AM_MOVEMENT             "շարժում"
    #define AM_OFF                  "OFF"
    #define AM_ON                   "ON"
    #define AM_COLDEXTRUD           "Արգելվում է սառը արտահոսքը (T0 <170 աստիճան)"
    #define AM_COLDEXTRUD170        "Warերմացեք"
    #define AM_CLEARNOZZ            "Մաքրեք վարդակը:."
    #define AM_MODE_SELECT          "Ռեժիմի ընտրություն"
    #define AM_POWERLOSS_ON         "PLR ON"
    #define AM_POWERLOSS_OFF        "PLR OFF"
    #define AM_MOTOR_UNLOCK         "Շարժիչի ապակողպում"
    #define AM_MESH_VALID           "Հավասարեցման թեստ"
    #define AM_WAIT_FOR_TEMP        "Խնդրում ենք սպասել ջերմաստիճանի բարձրացմանը մինչ նպատակային ջերմաստիճանը"
    #define AM_STOP_PLANE_TEST      "Wantանկանու՞մ եք դադարեցնել հավասարեցման թեստը?"
    #define AM_IS_BABYSTEP          "Համոզվա՞ծ եք խնայել ճշգրտված արժեքը?"
    #define AM_IS_LANGUAGE          "Are you sure to save the settings?"
    #define AM_HOT_HOMING_ON        "HOT HOME:ON"
    #define AM_HOT_HOMING_OFF       "HOT HOME:OFF"
    #define AM_HOT_HOMING_START     "HOT HOME:START!"
    #define AM_HOT_HOMING_Z_HOMING  "HOT HOME:Z HOMING!"   
    #define AM_HOT_HOMING_STOP      "HOT HOME:OK!"
    #define AM_LIST_MODE            "LIST"
    #define AM_ICON_MODE            "ICON"    
#endif
#endif