#include "n3_btn.h"

#include <wirish/wirish.h>


void do_nothing_isr(void)
{
	// Do nothing...
}


N3_Btn::N3_Btn(uint8 pin_)
: pin(pin_)
, last_state(false)
, last_duration(0)
{
	// Initialise with pull-up and enable an interrupt
	pinMode(pin, INPUT);
	digitalWrite(pin, HIGH);
	attachInterrupt(pin, do_nothing_isr, CHANGE);
}


void
N3_Btn::update(void)
{
	// Invert the pin's value as it is being pulled-up
	bool new_state = !digitalRead(pin);
	
	if (!last_state && new_state) {
		// The button has just started being pressed
		last_pressed = millis();
	} else if (last_state && !new_state) {
		// The button has just been released
		int press_duration = millis() - last_pressed;
		if (press_duration > BTN_MAX_JITTER_DURATION) {
			last_duration = press_duration;
		}
	}
	
	last_state = new_state; 
}
