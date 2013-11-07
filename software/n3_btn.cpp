#include "n3.h"
#include "n3_globals.h"
#include "n3_sleep.h"
#include "n3_btn.h"

#include <wirish/wirish.h>


static void do_nothing_isr(void)
{
	n3_sleep.no_more(N3_WAKEUP_BTN);
}


N3_Btn::N3_Btn(uint8 pin_, unsigned long long_press_duration_)
: pin(pin_)
, long_press_duration(long_press_duration_)
, last_state(false)
, last_pressed(0ul)
, short_pressed_flag(false)
, long_pressed_flag(false)
, long_pressed_flag_occurred(false)
{
	// Do nothing, initialise in the first call to update()
}


void
N3_Btn::begin(void)
{
	// Initialise with pull-down and enable an interrupt
	pinMode(pin, INPUT_PULLDOWN);
	attachInterrupt(pin, do_nothing_isr, CHANGE);
}


void
N3_Btn::update(void)
{
	// Invert the pin's value as it is being pulled-up
	bool new_state = digitalRead(pin);
	
	if (!last_state && new_state) {
		// The button has just started being pressed
		last_pressed = millis();
		long_pressed_flag_occurred = false;
		n3_sleep.no_longer_than(N3_WAKEUP_BTN, N3_BTN_LONG_PRESS_DURATION);
	} else if (last_state && !new_state) {
		// The button has just been released
		unsigned long press_duration = millis() - last_pressed;
		if (press_duration > BTN_MAX_JITTER_DURATION && press_duration < long_press_duration) {
			// Indicate that a short press has occurred (long presses are reported
			// further down)
			short_pressed_flag = true;
		}
	} else if (new_state) {
		// The button is being pressed, has it been a long press yet?
		unsigned long press_duration = millis() - last_pressed;
		if (press_duration >= long_press_duration && !long_pressed_flag_occurred) {
			long_pressed_flag = true;
			long_pressed_flag_occurred = true;
		}
	}
	
	last_state = new_state; 
}
