/**
 * Setup the board and its peripherals.
 */

#include <wirish/wirish.h>
#include <LiquidCrystal/LiquidCrystal.h>

#include "n3.h"
#include "n3_init.h"


void
n3_init(void)
{
	n3_init_btn();
	n3_init_bat();
	n3_init_lcd();
	n3_init_gps();
}


void
n3_init_btn(void)
{
	pinMode(N3_BTN_PIN, INPUT_PULLDOWN);
	
	// TODO: Setup Interrupt
}


void
n3_init_bat(void)
{
	pinMode(N3_BAT_V_PIN, INPUT_ANALOG);
}


// Concrete definition of the LiquidCrystal class defined (extern) in n3.h
LiquidCrystal n3_lcd( N3_LCD_RS_PIN
                    , N3_LCD_RW_PIN
                    , N3_LCD_ENABLE_PIN
                    , N3_LCD_D4_PIN
                    , N3_LCD_D5_PIN
                    , N3_LCD_D6_PIN
                    , N3_LCD_D7_PIN
                    );

void
n3_init_lcd(void)
{
	n3_lcd.begin(N3_LCD_COLS, N3_LCD_ROWS);
}

void
n3_init_gps(void)
{
	n3_gps_serial.begin(N3_GPS_BAUDRATE);
}
