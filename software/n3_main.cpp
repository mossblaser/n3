// Not quite wiring...
#include <wirish/wirish.h>

#include "n3.h"
#include "n3_globals.h"

#include "n3_ui.h"
#include "n3_ui_about.h"
#include "n3_ui_satellites.h"
#include "n3_ui_altitude.h"

N3_UI_About_Window          n3_ui_about_window;
N3_UI_Satellites_Window     n3_ui_satellites_window;

N3_UI_SEA_Altitude_Window   n3_ui_sea_altitude_window;
N3_UI_OSDN_Altitude_Window  n3_ui_osdn_altitude_window;
N3_UI_IOSDN_Altitude_Window n3_ui_iosdn_altitude_window;

N3_UI_Window *windows[] = {
	&n3_ui_osdn_altitude_window,
	&n3_ui_iosdn_altitude_window,
	&n3_ui_sea_altitude_window,
	
	&n3_ui_satellites_window,
	
	&n3_ui_about_window,
};
int num_windows = sizeof(windows)/sizeof(N3_UI_Window *);

N3_UI n3_ui(windows, num_windows);

int main(void)
{
	n3_btn.begin();
	n3_bat.begin();
	n3_gps.begin();
	n3_ui.begin();
	
	while (true) {
		n3_gps.update();
		n3_btn.update();
		n3_ui.update();
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
