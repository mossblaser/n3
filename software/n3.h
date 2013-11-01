/**
 * Definitions of key constants used by the N3 firmware.
 */

#ifndef N3_H
#define N3_H

// -----------------------------------------------------------------------------
// Software params
// -----------------------------------------------------------------------------

// How frequently should the UI be refreshed (msec)
#define N3_UI_UPDATE_PERIOD 500

// -----------------------------------------------------------------------------
// Pins and device definitions
// -----------------------------------------------------------------------------

// Serial port used to communicate with the GPS receiver
#define N3_GPS_BAUDRATE 57600
#define n3_gps_serial Serial1

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

// Button
#define N3_BTN_PIN 10

#endif
