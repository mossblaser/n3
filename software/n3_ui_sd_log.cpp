#include <stdio.h>

#include <wirish/wirish.h>
#include <LiquidCrystal/LiquidCrystal.h>

#include "n3_ui_sd_log.h"

#include "n3.h"
#include "n3_ui.h"
#include "n3_globals.h"


bool
N3_UI_SD_Log_Window::is_valid(void)
{
	return true;
}


void
N3_UI_SD_Log_Window::focus(void)
{
	n3_lcd.clear();
	
	// Set this incorreclty to force redrawing
	last_working = !n3_sd_log.working();
}

void
N3_UI_SD_Log_Window::update(void)
{
	bool working = n3_sd_log.working();
	
	if (working != last_working) {
		if (working) {
			n3_lcd.home();
			n3_lcd.print("Logging");
			n3_lcd.setCursor(0,1);
			n3_lcd.print("to card");
		} else {
			n3_lcd.home();
			//            |-----|
			n3_lcd.print("No SD  ");
			n3_lcd.setCursor(0,1);
			//            |-----|
			n3_lcd.print("Card   ");
		}
	}
	
	last_working = working;
}

#include "n3_icons/sd_log_window_splash.h"

uint8 *
N3_UI_SD_Log_Window::get_splash_icon(int char_num)
{
	return N3_ICON_SD_LOG_WINDOW_SPLASH[char_num%3][char_num/3];
}

static const char *SPLASH_TOP = "Route";
static const char *SPLASH_BTM = "Log'r";

const char *
N3_UI_SD_Log_Window::get_splash_text(int line_no)
{
	switch (line_no) {
		default:
		case 0: return SPLASH_TOP; break;
		case 1: return SPLASH_BTM; break;
	}
}

