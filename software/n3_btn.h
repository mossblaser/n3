/**
 * Simple interface to a push-to-make button
 */

#ifndef N3_BTN_H
#define N3_BTN_H

#include <wirish/wirish.h>

#define BTN_MAX_JITTER_DURATION 2

/**
 * Assumes a simple push-to-make switch connected to ground and internally
 * pulled up in the uC.
 */
class N3_Btn {
	public:
		/**
		 * Attaches a do-nothing interrupt handler to the given pin on any change
		 * and enables an internal pull-up resistor.
		 *
		 * Should the device be in sleep mode, the interrupt will re-awaken it
		 * which should awaken a main-loop allowing the calling of the update()
		 * method.
		 */
		N3_Btn( uint8 pin
		      );
		
		/**
		 * Call either by polling or at least after any change of the pin's value.
		 */
		void update(void);
		
		
		/**
		 * If the button has been pressed-and-released since the last call, returns
		 * the number of msec it was pressed for. Otherwise, returns 0.
		 */
		int get_press(void) {int d = last_duration; last_duration = 0; return d;}
	
	protected:
		uint8 pin;
		
		// State of the pin when last observed
		bool last_state;
		
		// The value of millis() when the button was first pressed.
		int last_pressed;
		
		// The number of millis between the button last being pressed and released
		// or 0 if not recently pressed.
		int last_duration;
};

#endif
