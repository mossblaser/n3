/**
 * Definitions of global variables/resources in the system.
 */

#ifndef N3_GLOBALS_H
#define N3_GLOBALS_H

#include <LiquidCrystal/LiquidCrystal.h>
#include "n3_bat.h"
#include "n3_gps.h"
#include "n3_btn.h"
#include "n3_sleep.h"

extern LiquidCrystal n3_lcd;
extern N3_Bat        n3_bat;
extern N3_GPS        n3_gps;
extern N3_Btn        n3_btn;
extern N3_Sleep      n3_sleep;

#endif

