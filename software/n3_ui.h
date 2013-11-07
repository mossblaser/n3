/**
 * The user-interface manager and the base-class for UI views.
 */

#ifndef N3_UI_H
#define N3_UI_H

#include "n3.h"

#include <wirish/wirish.h>


#define N3_UI_BAT_ICON_CHAR 6
#define N3_UI_GPS_ICON_CHAR 7


class N3_UI_Window {
	public:
		/**
		 * Indicates whether the window contains valid data. If false, it is only
		 * legal to call is_valid() and unfocus().
		 */
		virtual bool is_valid(void) = 0;
		
		/**
		 * Attempt to give this window focus.
		 */
		virtual void focus(void) = 0;
		
		/**
		 * Update the screen. Called periodically while focused.
		 */
		virtual void update(void) = 0;
		
		/**
		 * The window is nolonger focused, update will nolonger be called.
		 */
		virtual void unfocus(void) = 0;
		
		
		/**
		 * Returns a pointer to an array of 8 bytes containing the bitmap to display
		 * on the left of the splash screen like so for a given character position:
		 *
		 *   012
		 *   345
		 */
		virtual uint8 *get_splash_icon(int char_num) = 0;
		
		
		/**
		 * Return the text to display on the splash screen. Must return a string for
		 * the given line number.
		 */
		virtual const char *get_splash_text(int line_no) = 0;
};


/**
 * The main UI management class.
 */
class N3_UI {
	public:
		N3_UI(N3_UI_Window **windows, int num_windows);
		
		void begin(void);
		
		void update(void);
	
	protected:
		/**
		 * Cycle around to the next available window.
		 */
		void next_window(void);
		
		/**
		 * Returns an indication of battery level where 0 is low and 5 is high.
		 */
		int get_bat_level(void);
		
		/**
		 * Returns an indication of GPS reception quality ranging from -1 to 3 where
		 * -1 is no satellites, 0 is no fix, 1 is a 2D fix, 2 is a 3D fix and 3 is a
		 * good 3D fix.
		 */
		int get_gps_level(void);
		
		/**
		 * Draw the battery and GPS status icons to the LCD. If force is not false,
		 * only draw if changed.
		 */
		void update_bat_gps_status(bool force);
		
		N3_UI_Window **windows;
		int            num_windows;
		int            cur_window;
		
		// Time at which the window was switched to and the splash displayed
		unsigned long switch_time;
		
		// True only once the splash has been displayed for a reasonable period.
		bool focused;
		
		int last_bat_level;
		int last_gps_level;
};

#endif


