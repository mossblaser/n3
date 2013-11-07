#include <wirish/wirish.h>

#include "n3.h"
#include "n3_sleep.h"

#define NO_WAKEUP  0ul
#define WAKEUP_NOW 1ul


bool
N3_Sleep::wakeup_due(void)
{
	bool wakeup = false;
	unsigned long now = millis();
	
	noInterrupts();
		for (int i = 0; i<N3_NUM_WAKEUPS; i++) {
			// If a wakeup is due
			if (wakeups[i] != NO_WAKEUP && wakeups[i] <= now) {
				wakeups[i] = NO_WAKEUP;
				wakeup = true;
			}
		}
	interrupts();
	
	return wakeup;
}


void
N3_Sleep::no_more(int wakeup_num)
{
	wakeups[wakeup_num] = WAKEUP_NOW;
}


void
N3_Sleep::no_longer_than(int wakeup_num, unsigned long delta)
{
	wakeups[wakeup_num] = millis() + delta;
}


void
N3_Sleep::sleep_now(void)
{
	while (!wakeup_due())
		asm("wfi");
}
