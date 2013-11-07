#include <stdio.h>
#include <wirish/wirish.h>
#include <LiquidCrystal/LiquidCrystal.h>

#include "os_coord/os_coord.h"
#include "os_coord/os_coord_math.h"

#include "n3_ui_lat_lon.h"

#include "n3.h"
#include "n3_ui.h"
#include "n3_globals.h"

#define INVALID_LAT_LON -9876.5


bool
N3_UI_Lat_Lon_Window::is_valid(void)
{
	return n3_gps.is_fixed();
}


void
N3_UI_Lat_Lon_Window::focus(void)
{
	n3_lcd.clear();
	
	// Make sure we render the thing!
	last_lat_lon.lat = -999.0;
	last_lat_lon.lon = -999.0;
}

void
N3_UI_Lat_Lon_Window::update(void)
{
	os_lat_lon_t lat_lon = get_lat_lon();
	
	if ( lat_lon.lat != last_lat_lon.lat
	   ||lat_lon.lon != last_lat_lon.lon
	   ) {
		char line[8];
		
		snprintf(line, 8, "%1.5f", RAD_2_DEG(lat_lon.lat));
		line[7] = '\0';
		n3_lcd.home();
		n3_lcd.print(line);
		
		snprintf(line, 8, "%1.5f", RAD_2_DEG(lat_lon.lon));
		line[7] = '\0';
		n3_lcd.setCursor(0,1);
		n3_lcd.print(line);
	}
	
	last_lat_lon = lat_lon;
}


/******************************************************************************
 * WGS84 Coordinates
 ******************************************************************************/


os_lat_lon_t
N3_UI_WGS84_Lat_Lon_Window::get_lat_lon(void)
{
	return n3_gps.get_coordinates();
}


#include "n3_icons/wgs_lat_lon_window_splash.h"

uint8 *
N3_UI_WGS84_Lat_Lon_Window::get_splash_icon(int char_num)
{
	return N3_ICON_WGS_LAT_LON_WINDOW_SPLASH[char_num%3][char_num/3];
}

static const char *WGS84_SPLASH_TOP = "Lt/Ln";
static const char *WGS84_SPLASH_BTM = "WGS84";

const char *
N3_UI_WGS84_Lat_Lon_Window::get_splash_text(int line_no)
{
	switch (line_no) {
		default:
		case 0: return WGS84_SPLASH_TOP; break;
		case 1: return WGS84_SPLASH_BTM; break;
	}
}

