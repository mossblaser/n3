/**
 * Simple interface to a push-to-make button
 */

#ifndef N3_BTN_H
#define N3_BTN_H

#include <wirish/wirish.h>

#define BTN_MAX_JITTER_DURATION 20ul

/**
 * Assumes a simple push-to-make switch connected to ground and internally
 * pulled down in the uC.
 */
class N3_Btn {
	public:
		N3_Btn( uint8         pin
		      , unsigned long long_press_duration
		      );
		
		/**
		 * Attaches a do-nothing interrupt handler to the given pin on any change
		 * and enables an internal down-up resistor.
		 *
		 * Should the device be in sleep mode, the interrupt will re-awaken it
		 * which should awaken a main-loop allowing the calling of the update()
		 * method.
		 */
		void begin(void);
		
		/**
		 * Call either by polling or at least after any change of the pin's value.
		 */
		void update(void);
		
		
		/**
		 * Was the button pressed-then-released for a short period? Returns true if
		 * so and clears the flag.
		 */
		bool short_pressed(void) {bool f = short_pressed_flag; short_pressed_flag = false; return f;}
		
		
		/**
		 * Was the button long pressed? If so, return true and clear the flag. This
		 * function will also return true if the button is still being held.
		 */
		bool long_pressed(void) {bool f = long_pressed_flag; long_pressed_flag = false; return f;}
	
	protected:
		uint8 pin;
		
		unsigned long long_press_duration;
		
		// State of the pin when last observed
		bool last_state;
		
		// The value of millis() when the button was first pressed.
		unsigned long last_pressed;
		
		// Indicates the short-press has occurred which hasn't been read
		bool short_pressed_flag;
		
		// Indicates the long-press has occurred which hasn't been seen
		bool long_pressed_flag;
		
		// Indicates that a long-press has happened and flagged this button-press so
		// don't set the long_pressed_flag again (as it may have been read so we
		// shouldn't report it again).
		bool long_pressed_flag_occurred;
};

#endif
