// Not quite wiring...
#include <wirish/wirish.h>

#include "n3.h"

int main(void)
{
	
	
	while (true) {
		// Do nothing... forever!
	}
	
	return 0;
}

// Standard libmaple init() and main.
//
// The init() part makes sure your board gets set up correctly. It's
// best to leave that alone unless you know what you're doing. main()
// is the usual "call setup(), then loop() forever", but of course can
// be whatever you want.

__attribute__((constructor)) void
premain()
{
	init();
}
