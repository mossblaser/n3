#include "n3_ui.h"
#include "n3_globals.h"

#include "n3_icons/battery.h"
#include "n3_icons/gps.h"

N3_UI::N3_UI(N3_UI_Window **windows_, int num_windows_)
: windows(windows_)
, num_windows(num_windows_)
, cur_window(-1)
{
	// Set the LCD going...
	n3_lcd.begin(N3_LCD_COLS, N3_LCD_ROWS);
	n3_lcd.clear();
	
	next_window();
}


void
N3_UI::update(void)
{
	// Has the user short-pressed the button? (And thus wishes to advance the
	// window)
	if (!(windows[cur_window]->is_valid())
	    && n3_btn.peek_press() > 0
	    && n3_btn.peek_press() < N3_BTN_LONG_PRESS_DURATION) {
		n3_btn.get_press();
		next_window();
	}
	
	if (!focused) {
		// Has the splash been shown long enough yet?
		if (millis() - switch_time > N3_SPLASH_DURATION) {
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


void
N3_UI::update_bat_gps_status(bool force)
{
	int last_bat_level = -1;
	int last_gps_level = -1;
	if (force) {
		last_bat_level = -1;
		last_gps_level = -1;
	}
	
	// Show the battery level as nominally between 0 and 5 (but if greater assumed
	// to be 5).
	int bat_level = (int)(((n3_bat.get_voltage() - N3_BAT_MIN_V)
	                       / (N3_BAT_MAX_V-N3_BAT_MIN_V)) * 5.0);
	
	int gps_level = n3_gps.get_num_satellites();
	
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
			case 5: n3_lcd.createChar(N3_UI_GPS_ICON_CHAR, N3_ICON_GPS_3); break;
			case 4: n3_lcd.createChar(N3_UI_GPS_ICON_CHAR, N3_ICON_GPS_2); break;
			case 3: n3_lcd.createChar(N3_UI_GPS_ICON_CHAR, N3_ICON_GPS_1); break;
			case 2: n3_lcd.createChar(N3_UI_GPS_ICON_CHAR, N3_ICON_GPS_0); break;
			case 1: n3_lcd.createChar(N3_UI_GPS_ICON_CHAR, N3_ICON_GPS_0); break;
			case 0: n3_lcd.createChar(N3_UI_GPS_ICON_CHAR, N3_ICON_GPS_X); break;
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
			// Clear and show the splash for this valid window
			n3_lcd.clear();
			
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
			
			switch_time = millis();
			focused     = false;
			
			return;
		}
	}
}
