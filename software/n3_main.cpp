// Not quite wiring...
#include <wirish/wirish.h>

#include "n3.h"
#include "n3_globals.h"
#include "n3_sleep.h"

#include "n3_ui.h"
#include "n3_ui_about.h"
#include "n3_ui_satellites.h"
#include "n3_ui_altitude.h"
#include "n3_ui_grid_ref.h"
#include "n3_ui_lat_lon.h"
#include "n3_ui_datetime.h"

N3_UI_About_Window      n3_ui_about_window;
N3_UI_Satellites_Window n3_ui_satellites_window;
N3_UI_Datetime_Window   n3_ui_datetime_window;

N3_UI_SEA_Altitude_Window   n3_ui_sea_altitude_window;
N3_UI_OSDN_Altitude_Window  n3_ui_osdn_altitude_window;
N3_UI_IOSDN_Altitude_Window n3_ui_iosdn_altitude_window;

N3_UI_National_Grid_Grid_Ref_Window       n3_ui_ng_grid_ref_window;
N3_UI_Irish_National_Grid_Grid_Ref_Window n3_ui_ing_grid_ref_window;

N3_UI_WGS84_Lat_Lon_Window n3_ui_wgs84_lat_lon_window;

N3_UI_Window *windows[] = {
	&n3_ui_ng_grid_ref_window,
	&n3_ui_ing_grid_ref_window,
	
	&n3_ui_wgs84_lat_lon_window,
	
	&n3_ui_osdn_altitude_window,
	&n3_ui_iosdn_altitude_window,
	&n3_ui_sea_altitude_window,
	
	&n3_ui_datetime_window,
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
		
		// Go to sleep until an interrupt wakes us up again (since systick is
		// enabled, we'll be woken every ms anyway)
		n3_sleep.no_longer_than(N3_WAKEUP_REFRESH, N3_UI_UPDATE_PERIOD);
		n3_sleep.sleep_now();
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
