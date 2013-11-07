#include <wirish/wirish.h>
#include <LiquidCrystal/LiquidCrystal.h>

#include "n3_ui_satellites.h"

#include "n3.h"
#include "n3_ui.h"
#include "n3_gps.h"
#include "n3_globals.h"

void
N3_UI_Satellites_Window::focus(void)
{
	n3_lcd.clear();
	last_num_sats = -1;
}


void
N3_UI_Satellites_Window::update(void)
{
	int               num_sats = n3_gps.get_num_satellites();
	n3_gps_fix_type_t fix_type = n3_gps.fix_type();
	
	if (last_num_sats != num_sats || last_fix_type != fix_type) {
		n3_lcd.home();
		n3_lcd.print("       ");
		n3_lcd.home();
		//            |-----|
		n3_lcd.print(num_sats);
		n3_lcd.print(" sats");
		n3_lcd.setCursor(0,1);
		switch(fix_type) {
			default:
			case N3_GPS_NO_FIX:
			//            |-----|
			n3_lcd.print("No Fix "); break;
			case N3_GPS_2D_FIX:
			//            |-----|
			n3_lcd.print("2D Fix "); break;
			case N3_GPS_3D_FIX:
			//            |-----|
			n3_lcd.print("3D Fix "); break;
		}
	}
	
	last_num_sats = num_sats;
	last_fix_type = fix_type;
}


// TODO: Add Icon
static uint8 XXX_NULL_ICON[8] = {5,5,5,5, 5,5,5,5};

uint8 *
N3_UI_Satellites_Window::get_splash_icon(int char_num)
{
	return XXX_NULL_ICON;
}

static const char *SPLASH_TOP = "Sat  ";
static const char *SPLASH_BTM = "Info ";

const char *
N3_UI_Satellites_Window::get_splash_text(int line_no)
{
	switch (line_no) {
		default:
		case 0: return SPLASH_TOP; break;
		case 1: return SPLASH_BTM; break;
	}
}

