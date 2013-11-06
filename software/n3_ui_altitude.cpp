#include <wirish/wirish.h>
#include <LiquidCrystal/LiquidCrystal.h>

#include "os_coord/os_coord.h"

#include "n3_ui_altitude.h"

#include "n3.h"
#include "n3_ui.h"
#include "n3_globals.h"

#define INVALID_ZEROED_ALTITUDE -9876.5

#define MINIMUM_REALISTIC_ALTITUDE_CM -99999

N3_UI_Altitude_Window::N3_UI_Altitude_Window(void)
: zeroed_altitude(INVALID_ZEROED_ALTITUDE)
{
	// Do nothing
}

void
N3_UI_Altitude_Window::focus(void)
{
	n3_lcd.clear();
	last_altitude = -9999.0;
}

void
N3_UI_Altitude_Window::update(void)
{
	double altitude = get_altitude();
	
	if (zeroed_altitude == INVALID_ZEROED_ALTITUDE || n3_btn.long_pressed()) {
		zeroed_altitude = altitude;
		
		// Force a redraw of the display
		last_altitude = -9999.9;
	}
	
	if (last_altitude != altitude) {
		// The current altitude
		n3_lcd.home();         n3_lcd.print("       ");
		n3_lcd.home();         n3_lcd.print(altitude);
		n3_lcd.setCursor(6,0); n3_lcd.print("m");
		
		// The distance from the zeroed height
		n3_lcd.setCursor(0,1);
		if (zeroed_altitude <= altitude)
			n3_lcd.print("+");
		n3_lcd.print(altitude - zeroed_altitude);
	}
	
	last_altitude = altitude;
}


/******************************************************************************
 * WGS84 Altitude
 ******************************************************************************/


// TODO: Add Icon
static uint8 XXX_NULL_ICON[8] = {21,21,21,21, 1,1,1,1};

bool
N3_UI_SEA_Altitude_Window::is_valid(void)
{
	return n3_gps.is_fixed() && n3_gps.get_altitude() <= MINIMUM_REALISTIC_ALTITUDE_CM;
}


double
N3_UI_SEA_Altitude_Window::get_altitude(void)
{
	return n3_gps.get_altitude();
}

uint8 *
N3_UI_SEA_Altitude_Window::get_splash_icon(int char_num)
{
	return XXX_NULL_ICON;
}

static const char *WGS84_SPLASH_TOP = "Alt. ";
static const char *WGS84_SPLASH_BTM = "SeaLv";

const char *
N3_UI_SEA_Altitude_Window::get_splash_text(int line_no)
{
	switch (line_no) {
		default:
		case 0: return WGS84_SPLASH_TOP; break;
		case 1: return WGS84_SPLASH_BTM; break;
	}
}



/******************************************************************************
 * OSDN Altitude
 ******************************************************************************/

#include "n3_wgs84_to_os.h"
#include "os_coord/os_coord_data.h"

bool
N3_UI_OSDN_Altitude_Window::is_valid(void)
{
	return n3_gps.is_fixed()
	       && n3_gps.get_altitude() <= MINIMUM_REALISTIC_ALTITUDE_CM
	       && n3_wgs84_to_os( n3_gps.get_coordinates()
	                        , OS_TM_NATIONAL_GRID
	                        , OS_GR_NATIONAL_GRID
	                        ).code[0] != '\0';
}


double
N3_UI_OSDN_Altitude_Window::get_altitude(void)
{
	os_grid_ref_t grid_ref = n3_wgs84_to_os( n3_gps.get_coordinates()
	                                       , OS_TM_NATIONAL_GRID
	                                       , OS_GR_NATIONAL_GRID
	                                       );
	return grid_ref.h;
}

uint8 *
N3_UI_OSDN_Altitude_Window::get_splash_icon(int char_num)
{
	return XXX_NULL_ICON;
}

static const char *OSDN_SPLASH_TOP = "Alt. ";
static const char *OSDN_SPLASH_BTM = "OSDN ";

const char *
N3_UI_OSDN_Altitude_Window::get_splash_text(int line_no)
{
	switch (line_no) {
		default:
		case 0: return OSDN_SPLASH_TOP; break;
		case 1: return OSDN_SPLASH_BTM; break;
	}
}



/******************************************************************************
 * Irish OSDN Altitude
 ******************************************************************************/

#include "n3_wgs84_to_os.h"
#include "os_coord/os_coord_data.h"

bool
N3_UI_IOSDN_Altitude_Window::is_valid(void)
{
	return n3_gps.is_fixed()
	       && n3_gps.get_altitude() <= MINIMUM_REALISTIC_ALTITUDE_CM
	       && n3_wgs84_to_os( n3_gps.get_coordinates()
	                        , OS_TM_IRISH_NATIONAL_GRID
	                        , OS_GR_IRISH_NATIONAL_GRID
	                        ).code[0] != '\0';
}


double
N3_UI_IOSDN_Altitude_Window::get_altitude(void)
{
	os_grid_ref_t grid_ref = n3_wgs84_to_os( n3_gps.get_coordinates()
	                                       , OS_TM_IRISH_NATIONAL_GRID
	                                       , OS_GR_IRISH_NATIONAL_GRID
	                                       );
	return grid_ref.h;
}

uint8 *
N3_UI_IOSDN_Altitude_Window::get_splash_icon(int char_num)
{
	return XXX_NULL_ICON;
}

static const char *IOSDN_SPLASH_TOP = "Alt. ";
static const char *IOSDN_SPLASH_BTM = "IOSDN";

const char *
N3_UI_IOSDN_Altitude_Window::get_splash_text(int line_no)
{
	switch (line_no) {
		default:
		case 0: return IOSDN_SPLASH_TOP; break;
		case 1: return IOSDN_SPLASH_BTM; break;
	}
}

