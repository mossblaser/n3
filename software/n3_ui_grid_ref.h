/**
 * Display OS-style grid references. What N3 was made to do!
 */

#ifndef N3_UI_GRID_REF_H
#define N3_UI_GRID_REF_H

#include "n3_ui.h"
#include "os_coord/os_coord.h"



/**
 * A pure virtual class providing a generic grid reference viewer.
 */
class N3_UI_Grid_Ref_Window : public N3_UI_Window {
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
		virtual os_grid_ref_t get_grid_ref(void) = 0;
	
	private:
		os_grid_ref_t last_grid_ref;
};



class N3_UI_National_Grid_Grid_Ref_Window : public N3_UI_Grid_Ref_Window {
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
		virtual os_grid_ref_t get_grid_ref(void);
};


class N3_UI_Irish_National_Grid_Grid_Ref_Window : public N3_UI_Grid_Ref_Window {
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
		virtual os_grid_ref_t get_grid_ref(void);
};

#endif



