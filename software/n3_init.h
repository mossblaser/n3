/**
 * Setup the board and its peripherals.
 */

#ifndef N3_INIT_H
#define N3_INIT_H

/**
 * Initialise everything! (Wrapper which calls all other init functions)
 */
void n3_init(void);

/**
 * Initialise the button (and its interrupt)
 */
void n3_init_btn(void);

/**
 * Initialise the battery monitoring pin
 */
void n3_init_bat(void);

/**
 * Initialise (and clear) the LCD
 */
void n3_init_lcd(void);

/**
 * Initialise the GPS receiver (and serial interrupts)
 */
void n3_init_gps(void);

#endif
