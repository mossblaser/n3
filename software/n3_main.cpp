/**
 * Main "world starts here" file for n3 (No-Nonsense Navigation).
 */

#include <wirish/wirish.h>

#include "n3.h"
#include "n3_globals.h"
#include "n3_sd_log.h"
#include "n3_sleep.h"

#include "n3_ui.h"
#include "n3_ui_about.h"
#include "n3_ui_satellites.h"
#include "n3_ui_altitude.h"
#include "n3_ui_grid_ref.h"
#include "n3_ui_lat_lon.h"
#include "n3_ui_datetime.h"
#include "n3_ui_no_fix.h"
#include "n3_ui_sd_log.h"

/******************************************************************************
 * Windows which form the user interface
 ******************************************************************************/

// A window which is displayed when no fix exists to inform the user that a fix
// is being saught.
N3_UI_No_Fix_Window n3_ui_no_fix_window;

// Windows which display position information (e.g. grid refs/coords)
N3_UI_National_Grid_Grid_Ref_Window       n3_ui_ng_grid_ref_window;
N3_UI_Irish_National_Grid_Grid_Ref_Window n3_ui_ing_grid_ref_window;
N3_UI_WGS84_Lat_Lon_Window                n3_ui_wgs84_lat_lon_window;

// Windows which show altitudes
N3_UI_SEA_Altitude_Window   n3_ui_sea_altitude_window;
N3_UI_OSDN_Altitude_Window  n3_ui_osdn_altitude_window;
N3_UI_IOSDN_Altitude_Window n3_ui_iosdn_altitude_window;

// GPS metadata windows
N3_UI_Datetime_Window   n3_ui_datetime_window;
N3_UI_Satellites_Window n3_ui_satellites_window;

// System windows
N3_UI_SD_Log_Window     n3_ui_sd_log_window;
N3_UI_About_Window      n3_ui_about_window;


N3_UI_Window *windows[] = {
	&n3_ui_no_fix_window,
	
	&n3_ui_ng_grid_ref_window,
	&n3_ui_ing_grid_ref_window,
	&n3_ui_wgs84_lat_lon_window,
	
	&n3_ui_osdn_altitude_window,
	&n3_ui_iosdn_altitude_window,
	&n3_ui_sea_altitude_window,
	
	&n3_ui_datetime_window,
	&n3_ui_satellites_window,
	
	&n3_ui_sd_log_window,
	&n3_ui_about_window,
};
int num_windows = sizeof(windows)/sizeof(N3_UI_Window *);

N3_UI n3_ui(windows, num_windows);



/******************************************************************************
 * The main-loop.
 ******************************************************************************/

int main(void)
{
	// Initialise all components which need it
	n3_btn.begin();
	n3_bat.begin();
	n3_gps.begin();
	n3_sd_log.begin();
	n3_ui.begin();
	
	// Main loop
	while (true) {
		// Scan for button presses
		n3_btn.update();
		
		// Process any incoming GPS data
		n3_gps.update();
		
		// Log the current positon (if due)
		n3_sd_log.update();
		
		// Redraw the UI
		n3_ui.update();
		
		// Make sure the main loop runs at least occasionally (e.g. to make sure
		// displayed battery infomation is up-to-date).
		n3_sleep.no_longer_than(N3_WAKEUP_REFRESH, N3_UI_UPDATE_PERIOD);
		
		// Go to sleep until an interrupt wakes us up again.
		// TODO: Since systick is enabled, we'll be woken every millisecond which is
		// a little wasteful. Maybe write a custom millis() call.
		n3_sleep.sleep_now();
	}
	
	// Shouldn't get here...
	return -1;
}

// Standard libmaple init()
//
// The init() part makes sure your board gets set up correctly. It's
// best to leave that alone unless you know what you're doing.

__attribute__((constructor)) void
premain()
{
	init();
}
