/**
 * Utilities for deciding when to sleep. Provides a numer of "wakeup" slots
 * which allow a time to be set for when a wakeup should be performed for a
 * particular resource.
 */

#ifndef N3_SLEEP_H
#define N3_SLEEP_H

#include <wirish/wirish.h>

enum {
	// A wakeup which ensures the display is frequently refreshed
	N3_WAKEUP_REFRESH,
	
	// A wakeup due to the button wishing to check it is being long pressed
	N3_WAKEUP_BTN,
	
	// A wakeup due to the DMA interrupt firing on the GPS sending us some data
	N3_WAKEUP_GPS,
	
	// A wakeup due to some UI event which is due to happen (e.g. splash screen
	// expiring, page changing)
	N3_WAKEUP_UI,
	
	// The number of wakeups
	N3_NUM_WAKEUPS
};

class N3_Sleep {
	public:
		/**
		 * Is the system due to wake up?
		 */
		bool wakeup_due(void);
		
		/**
		 * Trigger a wake up ASAP. (i.e. don't go to sleep again!).
		 */
		void no_more(int wakeup_num);
		
		/**
		 * Make sure the system is awake after this delay.
		 */
		void no_longer_than(int wakeup_num, unsigned long delta);
		
		
		/**
		 * Go to sleep...
		 */
		void sleep_now(void);
		
	private:
		volatile unsigned long wakeups[N3_NUM_WAKEUPS];
};

#endif


