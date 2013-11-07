/**
 * A high-level interface to the GPS receiver.
 */

#ifndef N3_GPS_H
#define N3_GPS_H

#include <libmaple/dma.h>

#include "TinyGPS/TinyGPS.h"
#include "os_coord/os_coord.h"

/**
 * The maximum number of milliseconds ago the last GPS fix was read before it is
 * considered stale and thus invalid.
 */
#define GPS_MAX_AGE 10000

/**
 * Size of the circular buffer used to keep received DMA'd serial data. Should
 * be big enough to accept just a bit more than three messages
 */
#define N3_GPS_DMA_BUFFER_SIZE 512

typedef enum n3_gps_fix_type {
	N3_GPS_NO_FIX = 1,
	N3_GPS_2D_FIX = 2,
	N3_GPS_3D_FIX = 3,
} n3_gps_fix_type_t;


class N3_GPS {
	public:
		/**
		 * Set everything up. Sets the given serial port's baudrate.
		 */
		N3_GPS( HardwareSerial *serial
		      , dma_request_src serial_dma_src
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
		 * What type of fix do we have?
		 */
		n3_gps_fix_type_t fix_type(void);
		
		
		/**
		 * How many satellites are being tracked?
		 */
		int get_num_satellites(void);
		
		
		/**
		 * What is the current horizontal dilution of precision?
		 */
		double get_hdop(void);
		
		
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
		dma_request_src serial_dma_src;
		
		uint32 baudrate;
		TinyGPS gps;
		
		// A buffer into which serial data from the GPS will be DMA'd
		char dma_buffer[N3_GPS_DMA_BUFFER_SIZE];
		
		// An offset into the dma_buffer which indicates the next unread character.
		// If equal to the position to the DMA controller's 
		int  dma_buffer_read_head;
		
		/**
		 * Return the current offset into the dma_buffer the DMA controller is
		 * pointing at. This is the offset into the dma_buffer which will next be
		 * written to by the DMA controller.
		 */
		int get_dma_buffer_write_head(void);
		
		/**
		 * Given a nema sentence which starts with $ and ends with *xx\r\n\0 where "xx"
		 * means any two bytes, modifies the string to contain the checksum where xx
		 * were.
		 */
		void add_checksum(char *nema_string);
};


#endif
