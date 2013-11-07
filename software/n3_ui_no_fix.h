/**
 * A screen which simply alerts the user to the fact that no GPS fix is
 * available.
 */

#ifndef N3_UI_NO_FIX_H
#define N3_UI_NO_FIX_H

#include "n3_ui.h"



class N3_UI_No_Fix_Window : public N3_UI_Window {
	public:
		/**
		 * Indicates whether the window contains valid data. If false, it is only
		 * legal to call is_valid() and unfocus().
		 */
		virtual bool is_valid(void);
		
		/**
		 * Attempt to give this window focus.
		 */
		virtual void focus(void);
		
		/**
		 * Update the screen. Called periodically while focused.
		 */
		virtual void update(void);
		
		/**
		 * The window is nolonger focused, update will nolonger be called.
		 */
		virtual void unfocus(void) { /* Nothing to do. */ }
		
		
		/**
		 * Returns a pointer to an array of 8 bytes containing the bitmap to display
		 * on the left of the splash screen like so for a given character position:
		 *
		 *   012
		 *   345
		 */
		virtual uint8 *get_splash_icon(int char_num);
		
		
		/**
		 * Return the text to display on the splash screen. Must return a string for
		 * the given line number.
		 */
		virtual const char *get_splash_text(int line_no);
	
	protected:
		// The time at which the spinner will next be refreshed
		unsigned long next_tick;
		
		// The beam number to be added next
		int beam_number;
		
		// A buffer to generate the spinner characters in
		uint8  spinner_chars[3][2][8];
};

#endif

