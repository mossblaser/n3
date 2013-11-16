/**
 * Log GPS data to an SD Card in GPX format.
 */

#ifndef N3_SD_LOG_H
#define N3_SD_LOG_H

#include <wirish/wirish.h>
#include <HardwareSPI.h>

#include "maple_sdfat/SdFat.h"
#include "maple_sdfat/FatStructs.h"
#include "maple_sdfat/Sd2Card.h"
#include "maple_sdfat/Sd2PinMap.h"
#include "maple_sdfat/SdFatmainpage.h"
#include "maple_sdfat/SdInfo.h"


// Filenames are 8.3 (followed by a null terminator)
#define N3_SD_LOG_MAX_FILENAME_LENGTH (8+1+3+1)


class N3_SD_Log {
	public:
		/**
		 * Initialise the logger. Takes the SPI port to use and an
		 * interval in msec between log entries.
		 */
		N3_SD_Log( HardwareSPI   spi
		         , unsigned long log_interval
		         );
		
		/**
		 * Set everything up ready to write to the SD card (including the
		 * HardwareSPI).
		 */
		void begin(void);
		
		/**
		 * Call periodically, causes the SD card to be written to.
		 */
		void update(void);
		
		/**
		 * Is the logger working? Reports false if writing to the SD card is failing
		 * for some reason.
		 */
		bool working(void);
	
	protected:
		/**
		 * Choose a filename for the file using today's date. Must only be used when a
		 * valid GPS time/date is available.
		 *
		 * Filenames of the form year-mon-day.gpx where day, month, year are two digit
		 * integers.
		 */
		void set_filename(void);
		
		/**
		 * Open the log file. If the file does not exist yet, create it and add the
		 * appropriate header. If the file does exist, make sure the cursor is
		 * placed at the end of an open <trkseg> section. Returns false on faliure.
		 */
		bool open_file(void);
		
		/**
		 * Close the currently open log file adding all required closing tags and
		 * then syncing the changes back to the SD card.
		 */
		bool close_file(void);
		
		// The SPI port used by the SD Card
		HardwareSPI spi;
		
		// msec between log entries.
		unsigned long log_interval;
		
		// When was the last log entry attempted?
		unsigned long last_entry_time;
		
		// Was the GPS fixed when the last entry was written? (Used to decide
		// whether to start a new "track" in the GPX file.
		bool last_gps_fixed;
		
		// The filename of the log file opened for writing. If of zero length then
		// the file is not open.
		char filename[N3_SD_LOG_MAX_FILENAME_LENGTH];
		
		// The card, volume, file system root and file we're working on (and whether
		// they're in a working state
		Sd2Card  card;   bool card_working;
		SdVolume volume; bool volume_working;
		SdFile   root;   bool root_working;
		SdFile   file;   bool file_working;
};

#endif

