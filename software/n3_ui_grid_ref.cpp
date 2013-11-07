#include <stdio.h>
#include <wirish/wirish.h>
#include <LiquidCrystal/LiquidCrystal.h>

#include "os_coord/os_coord.h"
#include "os_coord/os_coord_data.h"

#include "n3_ui_grid_ref.h"

#include "n3.h"
#include "n3_ui.h"
#include "n3_globals.h"
#include "n3_wgs84_to_os.h"

#define INVALID_LAT_LON -9876.5


bool
N3_UI_Grid_Ref_Window::is_valid(void)
{
	return n3_gps.is_fixed()
	       && get_grid_ref().code[0] != '\0';
}


void
N3_UI_Grid_Ref_Window::focus(void)
{
	n3_lcd.clear();
	
	// Make sure we render the thing!
	last_grid_ref.e = -1.0;
	last_grid_ref.n = -1.0;
}

void
N3_UI_Grid_Ref_Window::update(void)
{
	os_grid_ref_t grid_ref = get_grid_ref();
	
	if ( grid_ref.code[0] != last_grid_ref.code[0]
	   ||grid_ref.code[1] != last_grid_ref.code[1]
	   ||grid_ref.e       != last_grid_ref.e
	   ||grid_ref.n       != last_grid_ref.n
	   ) {
		
		char line[8];
		
		snprintf(line, 8, "%2s%05.0f", grid_ref.code, grid_ref.e);
		line[7] = '\0';
		n3_lcd.home();
		n3_lcd.print(line);
		
		snprintf(line, 8, "  %05.0f", grid_ref.n);
		line[7] = '\0';
		n3_lcd.setCursor(0,1);
		n3_lcd.print(line);
	}
	
	last_grid_ref = grid_ref;
}


/******************************************************************************
 * National Grid Location
 ******************************************************************************/


// TODO: Add Icon
static uint8 XXX_NULL_ICON[8] = {21,21,21,21, 1,1,1,1};


os_grid_ref_t
N3_UI_National_Grid_Grid_Ref_Window::get_grid_ref(void)
{
	return n3_wgs84_to_os( n3_gps.get_coordinates()
	                     , OS_TM_NATIONAL_GRID
	                     , OS_GR_NATIONAL_GRID
	                     );
}

uint8 *
N3_UI_National_Grid_Grid_Ref_Window::get_splash_icon(int char_num)
{
	return XXX_NULL_ICON;
}

static const char *NG_SPLASH_TOP = "G-Ref";
static const char *NG_SPLASH_BTM = "NatGr";

const char *
N3_UI_National_Grid_Grid_Ref_Window::get_splash_text(int line_no)
{
	switch (line_no) {
		default:
		case 0: return NG_SPLASH_TOP; break;
		case 1: return NG_SPLASH_BTM; break;
	}
}


/******************************************************************************
 * Irish National Grid Location
 ******************************************************************************/


os_grid_ref_t
N3_UI_Irish_National_Grid_Grid_Ref_Window::get_grid_ref(void)
{
	return n3_wgs84_to_os( n3_gps.get_coordinates()
	                     , OS_TM_IRISH_NATIONAL_GRID
	                     , OS_GR_IRISH_NATIONAL_GRID
	                     );
}

uint8 *
N3_UI_Irish_National_Grid_Grid_Ref_Window::get_splash_icon(int char_num)
{
	return XXX_NULL_ICON;
}

static const char *ING_SPLASH_TOP = "G-Ref";
static const char *ING_SPLASH_BTM = "IE NG";

const char *
N3_UI_Irish_National_Grid_Grid_Ref_Window::get_splash_text(int line_no)
{
	switch (line_no) {
		default:
		case 0: return ING_SPLASH_TOP; break;
		case 1: return ING_SPLASH_BTM; break;
	}
}

