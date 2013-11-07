#include <stdio.h>

#include <wirish/wirish.h>
#include <LiquidCrystal/LiquidCrystal.h>

#include "n3_ui_datetime.h"

#include "n3.h"
#include "n3_ui.h"
#include "n3_gps.h"
#include "n3_globals.h"

static const char MONTH_NAMES[12][4] = {
	"Jan",
	"Feb",
	"Mar",
	"Apr",
	"May",
	"Jun",
	"Jul",
	"Aug",
	"Sep",
	"Oct",
	"Nov",
	"Dec",
};


bool
N3_UI_Datetime_Window::is_valid(void)
{
	return n3_gps.is_fixed();
}


void
N3_UI_Datetime_Window::focus(void)
{
	n3_lcd.clear();
	last_yr = -1;
	last_mo = -1;
	last_dy = -1;
	last_hr = -1;
	last_mi = -1;
	last_se = -1;
}

void
N3_UI_Datetime_Window::update(void)
{
	int yr;
	int mo;
	int dy;
	int hr;
	int mi;
	int se;
	n3_gps.get_datetime(&yr, &mo, &dy, &hr, &mi, &se);
	
	if (   yr != last_yr
	    || mo != last_mo
	    || dy != last_dy
	    || hr != last_hr
	    || mi != last_mi
	    || se != last_se
	   ) {
		char line[8];
		
		snprintf(line, 8, "%02d:%02d   ", hr, mi);
		line[7] = '\0';
		n3_lcd.home();
		n3_lcd.print(line);
		
		snprintf(line, 8, "%02d %s     ", dy, MONTH_NAMES[mo-1]);
		line[7] = '\0';
		n3_lcd.setCursor(0,1);
		n3_lcd.print(line);
		
	}
	
	last_yr = yr;
	last_mo = mo;
	last_dy = dy;
	last_hr = hr;
	last_mi = mi;
	last_se = se;
}

#include "n3_icons/datetime_window_splash.h"

uint8 *
N3_UI_Datetime_Window::get_splash_icon(int char_num)
{
	return N3_ICON_DATETIME_WINDOW_SPLASH[char_num%3][char_num/3];
}

static const char *SPLASH_TOP = "GPS  ";
static const char *SPLASH_BTM = "Time ";

const char *
N3_UI_Datetime_Window::get_splash_text(int line_no)
{
	switch (line_no) {
		default:
		case 0: return SPLASH_TOP; break;
		case 1: return SPLASH_BTM; break;
	}
}

