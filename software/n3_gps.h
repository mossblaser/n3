/**
 * A high-level interface to the GPS receiver.
 */

#ifndef N3_GPS_H
#define N3_GPS_H

#include "TinyGPS/TinyGPS.h"
#include "os_coord/os_coord.h"

/**
 * The maximum number of milliseconds ago the last GPS fix was read before it is
 * considered stale and thus invalid.
 */
#define GPS_MAX_AGE 10000


class N3_GPS {
	public:
		/**
		 * Set everything up. Sets the given serial port's baudrate.
		 */
		N3_GPS( HardwareSerial *serial
		      , uint32          baudrate
		      );
		
		void begin(void);
		
		
		/**
		 * Read and parse any new input from the serial port.
		 */
		void update(void);
		
		
		/**
		 * Have we got a GPS fix?
		 */
		bool is_fixed(void);
		
		
		/**
		 * How many satellites are being tracked?
		 */
		int get_num_satellites(void);
		
		
		/**
		 * Get the WGS84 coordinates currently reported by the GPS receiver.
		 */
		os_lat_lon_t get_coordinates(void);
		
		
		/**
		 * Get the GPS receiver's average-MSL corrected altitude.
		 */
		double get_altitude(void);
		
		
		/**
		 * Get the current course (in degrees).
		 */
		double get_course(void);
		
		
		/**
		 * Get the current speed (in km/h)
		 */
		double get_speed(void);
		
		
		/**
		 * Get the current GPS date/time.
		 */
		void get_datetime(int *yr, int *mo, int *dy, int *hr, int *mi, int *se);
	
	protected:
		HardwareSerial *serial;
		uint32 baudrate;
		TinyGPS gps;
};


#endif
