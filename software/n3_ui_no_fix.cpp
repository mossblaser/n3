#include "n3_globals.h"
#include "n3_sleep.h"

#include "n3_ui_no_fix.h"

#define SPINNER_TICK_DELAY 100ul

bool
N3_UI_No_Fix_Window::is_valid(void)
{
	return !n3_gps.is_fixed();
}

#include "n3_icons/no_fix_window_graphic.h"
#include "n3_icons/no_fix_beam0_window_graphic.h"
#include "n3_icons/no_fix_beam1_window_graphic.h"
#include "n3_icons/no_fix_beam2_window_graphic.h"
#include "n3_icons/no_fix_beam3_window_graphic.h"

void
N3_UI_No_Fix_Window::focus(void)
{
	// Load the initial frame
	for (int i = 0; i < 6; i++) {
		// Load the buffer
		for (int y = 0; y < 8; y++)
			spinner_chars[i%3][i/3][y] = N3_ICON_NO_FIX_WINDOW_GRAPHIC[i%3][i/3][y];
		
		// Send to the display
		n3_lcd.createChar(i, spinner_chars[i%3][i/3]);
	}
	
	// Set the message & characters
	n3_lcd.setCursor(3,0); n3_lcd.print("I'm ");
	n3_lcd.setCursor(3,1); n3_lcd.print("Lost");
	n3_lcd.setCursor(0,0); n3_lcd.write(0);
	n3_lcd.setCursor(1,0); n3_lcd.write(1);
	n3_lcd.setCursor(2,0); n3_lcd.write(2);
	n3_lcd.setCursor(0,1); n3_lcd.write(3);
	n3_lcd.setCursor(1,1); n3_lcd.write(4);
	n3_lcd.setCursor(2,1); n3_lcd.write(5);
	
	beam_number = 0;
	
	
	// Make sure update does indeed update
	next_tick = 0;
}


void
N3_UI_No_Fix_Window::update(void)
{
	if (next_tick <= millis()) {
		// Load the beams on one at a time
		for (int i = 0; i < 6; i++) {
			// Xor the current beam
			for (int y = 0; y < 8; y++) {
				uint8 beam_line;
				// I'm sorry you had to see this...
				switch (beam_number) {
					default:
					case 0: beam_line = N3_ICON_NO_FIX_BEAM0_WINDOW_GRAPHIC[i%3][i/3][y]; break;
					case 1: beam_line = N3_ICON_NO_FIX_BEAM1_WINDOW_GRAPHIC[i%3][i/3][y]; break;
					case 2: beam_line = N3_ICON_NO_FIX_BEAM2_WINDOW_GRAPHIC[i%3][i/3][y]; break;
					case 3: beam_line = N3_ICON_NO_FIX_BEAM3_WINDOW_GRAPHIC[i%3][i/3][y]; break;
				}
				spinner_chars[i%3][i/3][y] ^= beam_line;
			}
			
			// Send to the display
			n3_lcd.createChar(i, spinner_chars[i%3][i/3]);
		}
		
		beam_number++;
		beam_number %= 4;
		
		// Set timer for next frame
		next_tick = millis() + SPINNER_TICK_DELAY;
		n3_sleep.no_longer_than(N3_WAKEUP_UI, SPINNER_TICK_DELAY);
	}
}



uint8 *
N3_UI_No_Fix_Window::get_splash_icon(int char_num)
{
	return NULL;
}


const char *
N3_UI_No_Fix_Window::get_splash_text(int line_no)
{
	const static char empty = '\0';
	return &empty;
}
