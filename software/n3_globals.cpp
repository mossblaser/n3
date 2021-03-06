#include <wirish/wirish.h>
#include <LiquidCrystal/LiquidCrystal.h>

#include "n3.h"
#include "n3_globals.h"
#include "n3_bat.h"
#include "n3_gps.h"
#include "n3_btn.h"
#include "n3_sd_log.h"
#include "n3_sleep.h"


N3_Sleep n3_sleep;

// Concrete definition of the battery monitor
N3_Bat n3_bat( N3_BAT_V_PIN
             , N3_BAT_V_DIV_R1
             , N3_BAT_V_DIV_R2
             , N3_UC_VOLTAGE
             );


// Concrete definition of the LiquidCrystal
LiquidCrystal n3_lcd( N3_LCD_RS_PIN
                    , N3_LCD_RW_PIN
                    , N3_LCD_ENABLE_PIN
                    , N3_LCD_D4_PIN
                    , N3_LCD_D5_PIN
                    , N3_LCD_D6_PIN
                    , N3_LCD_D7_PIN
                    );


N3_GPS n3_gps( &n3_gps_serial
             , N3_GPS_SERIAL_DMA_SRC
             , N3_GPS_BAUDRATE
             );


N3_Btn n3_btn( N3_BTN_PIN
             , N3_BTN_LONG_PRESS_DURATION
             );

N3_SD_Log n3_sd_log( N3_SD_CARD_SPI
                   , N3_LOG_INTERVAL
                   );
