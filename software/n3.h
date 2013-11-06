/**
 * Definitions of key constants used by the N3 firmware.
 */

#ifndef N3_H
#define N3_H

#define N3_VERSION_STR "0.1"

// -----------------------------------------------------------------------------
// Software params
// -----------------------------------------------------------------------------

// How frequently should the UI be refreshed in the worst case? (msec)
#define N3_UI_UPDATE_PERIOD 500

// How long should the button be pressed to count as a long press (msec)
#define N3_BTN_LONG_PRESS_DURATION 500ul

// How long should the splash be shown for each window? (msec)
#define N3_SPLASH_DURATION 1000ul

// For windows with several pages to show, how long should each page be shown?
#define N3_PAGE_DURATION 3000ul

// -----------------------------------------------------------------------------
// Pins and device definitions
// -----------------------------------------------------------------------------

// Serial port used to communicate with the GPS receiver
#define N3_GPS_BAUDRATE 57600
#define n3_gps_serial Serial2

// LCD Display
#define N3_LCD_RS_PIN     15
#define N3_LCD_RW_PIN     16
#define N3_LCD_ENABLE_PIN 17
#define N3_LCD_D4_PIN     18
#define N3_LCD_D5_PIN     19
#define N3_LCD_D6_PIN     20
#define N3_LCD_D7_PIN     21

#define N3_LCD_COLS 8
#define N3_LCD_ROWS 2

// System voltage
#define N3_UC_VOLTAGE 3.3

// Battery voltage input
#define N3_BAT_V_PIN 3
#define N3_BAT_V_DIV_R1 50000.0
#define N3_BAT_V_DIV_R2 50000.0

#define N3_BAT_MIN_V 2.4
#define N3_BAT_MAX_V 2.8

// Button
#define N3_BTN_PIN 10

#endif
