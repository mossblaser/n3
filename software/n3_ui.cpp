#include <string.h>

#include "n3_ui.h"
#include "n3_globals.h"

#include "n3_icons/battery.h"
#include "n3_icons/gps.h"

N3_UI::N3_UI(N3_UI_Window **windows_, int num_windows_)
: windows(windows_)
, num_windows(num_windows_)
, cur_window(-1)
, last_bat_level(-1)
, last_gps_level(-1)
{
	// Do nothing (initialisation is done when cur_window is -ve on the first call
	// to update()).
}


void
N3_UI::begin(void)
{
	// Initialise everything if needed.
	// Set the LCD going...
	n3_lcd.begin(N3_LCD_COLS, N3_LCD_ROWS);
	n3_lcd.clear();
	
	// Select the first window to show
	next_window();
}


void
N3_UI::update(void)
{
	// Has the user short-pressed the button? (And thus wishes to advance the
	// window)
	if (n3_btn.short_pressed())
		next_window();
	
	// If the active window invalidates itself, we should start searching for
	// windows from the start of the list.
	// 
	// Motivation: This means that if GPS is lost (for example) causing GPS
	// windows to invalidate a "Searching for satellites" window at the start of
	// the list and only valid when there is no satellite reception will be shown
	// (rather than, say, the about screen which might appear next in the list).
	if (!(windows[cur_window]->is_valid())) {
		cur_window = -1;
		next_window();
	}
	
	if (!focused) {
		// Has the splash been shown long enough yet?
		if (millis() - switch_time >= N3_SPLASH_DURATION) {
			n3_lcd.clear();
			windows[cur_window]->focus();
			update_bat_gps_status(true);
			focused = true;
		}
	}
	
	if (focused) {
		// Show the window's contents
		windows[cur_window]->update();
		
		update_bat_gps_status(false);
	}
	
}

int
N3_UI::get_bat_level(void)
{
	int bat_level = (int)(((n3_bat.get_voltage() - N3_BAT_MIN_V)
	                       / (N3_BAT_MAX_V-N3_BAT_MIN_V)) * 5.0);
	bat_level = max(bat_level, 0);
	bat_level = min(bat_level, 5);
	
	return bat_level;
}


int
N3_UI::get_gps_level(void)
{
	if (n3_gps.get_num_satellites() == 0)
		return -1;
	else if (n3_gps.fix_type() == N3_GPS_NO_FIX)
		return 0;
	else if (n3_gps.fix_type() == N3_GPS_2D_FIX)
		return 1;
	else if (n3_gps.fix_type() == N3_GPS_3D_FIX)
		return 2;
	else if (n3_gps.fix_type() == N3_GPS_3D_FIX && n3_gps.get_hdop() < N3_GPS_EXCELLENT_HDOP)
		return 3;
	else
		return -1;
}


void
N3_UI::update_bat_gps_status(bool force)
{
	if (force) {
		last_bat_level = -999;
		last_gps_level = -999;
	}
	
	int bat_level = get_bat_level();
	int gps_level = get_gps_level();
	
	// Update display if required
	if (bat_level != last_bat_level) {
		switch (bat_level) {
			default:
			case 5: n3_lcd.createChar(N3_UI_BAT_ICON_CHAR, N3_ICON_BATTERY_5); break;
			case 4: n3_lcd.createChar(N3_UI_BAT_ICON_CHAR, N3_ICON_BATTERY_4); break;
			case 3: n3_lcd.createChar(N3_UI_BAT_ICON_CHAR, N3_ICON_BATTERY_3); break;
			case 2: n3_lcd.createChar(N3_UI_BAT_ICON_CHAR, N3_ICON_BATTERY_2); break;
			case 1: n3_lcd.createChar(N3_UI_BAT_ICON_CHAR, N3_ICON_BATTERY_1); break;
			case 0: n3_lcd.createChar(N3_UI_BAT_ICON_CHAR, N3_ICON_BATTERY_0); break;
		}
		n3_lcd.setCursor(N3_LCD_COLS-1, 0);
		n3_lcd.write(N3_UI_BAT_ICON_CHAR);
	}
	
	if (gps_level != last_gps_level) {
		switch (gps_level) {
			default:
			case -1: n3_lcd.createChar(N3_UI_GPS_ICON_CHAR, N3_ICON_GPS_X); break;
			case  0: n3_lcd.createChar(N3_UI_GPS_ICON_CHAR, N3_ICON_GPS_0); break;
			case  1: n3_lcd.createChar(N3_UI_GPS_ICON_CHAR, N3_ICON_GPS_1); break;
			case  2: n3_lcd.createChar(N3_UI_GPS_ICON_CHAR, N3_ICON_GPS_2); break;
			case  3: n3_lcd.createChar(N3_UI_GPS_ICON_CHAR, N3_ICON_GPS_3); break;
		}
		n3_lcd.setCursor(N3_LCD_COLS-1, 1);
		n3_lcd.write(N3_UI_GPS_ICON_CHAR);
	}
	
	last_bat_level = bat_level;
	last_gps_level = gps_level;
}


void
N3_UI::next_window(void)
{
	if (cur_window >= 0)
		windows[cur_window]->unfocus();
	
	// Find the next window which accepts focus
	while (true) {
		cur_window += 1;
		cur_window %= num_windows;
		
		if (windows[cur_window]->is_valid()) {
			// Clear and show the splash (if available) for this valid window
			n3_lcd.clear();
			
			if (strlen(windows[cur_window]->get_splash_text(0))) {
				// Display the icon
				for (int i = 0; i < 6; i++) {
					n3_lcd.createChar(i, windows[cur_window]->get_splash_icon(i));
					n3_lcd.setCursor(i%3, i/3);
					n3_lcd.write(i);
				}
				
				// Display the text
				for (int i = 0; i < 2; i++) {
					n3_lcd.setCursor(3, i);
					n3_lcd.print(windows[cur_window]->get_splash_text(i));
				}
				
				n3_sleep.no_longer_than(N3_WAKEUP_UI, N3_SPLASH_DURATION);
				switch_time = millis();
				focused     = false;
			} else {
				// No splash, show this window straight away
				switch_time = millis() + N3_SPLASH_DURATION;
				focused = false;
			}
			
			return;
		}
	}
}
