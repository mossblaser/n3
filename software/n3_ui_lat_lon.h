/**
 * Display latitude/longitude.
 */

#ifndef N3_UI_LAT_LON_H
#define N3_UI_LAT_LON_H

#include "n3_ui.h"
#include "os_coord/os_coord.h"



/**
 * A pure virtual class providing a generic latitude/longitude viewer.
 */
class N3_UI_Lat_Lon_Window : public N3_UI_Window {
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
		
	protected:
		/**
		 * Return the current grid-reference
		 */
		virtual os_lat_lon_t get_lat_lon(void) = 0;
	
	private:
		os_lat_lon_t last_lat_lon;
};



class N3_UI_WGS84_Lat_Lon_Window : public N3_UI_Lat_Lon_Window {
	public:
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
		/**
		 * Return the current grid-reference
		 */
		virtual os_lat_lon_t get_lat_lon(void);
};

#endif



