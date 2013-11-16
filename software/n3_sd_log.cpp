#include <stdio.h>

#include <wirish/wirish.h>
#include <HardwareSPI.h>

#include "maple_sdfat/SdFat.h"
#include "maple_sdfat/FatStructs.h"
#include "maple_sdfat/Sd2Card.h"
#include "maple_sdfat/Sd2PinMap.h"
#include "maple_sdfat/SdFatmainpage.h"
#include "maple_sdfat/SdInfo.h"

#include "os_coord/os_coord.h"
#include "os_coord/os_coord_math.h"

#include "n3.h"
#include "n3_globals.h"
#include "n3_gps.h"
#include "n3_sd_log.h"

static const char FILE_HEADER[] =
	"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>\n"
	"\n"
	"<gpx\n"
	" xmlns=\"http://www.topografix.com/GPX/1/1\"\n"
	" version=\"1.1\"\n"
	" creator=\"N3 GPS v"N3_VERSION_STR"\">\n"
	"  <trk>\n"
	"    <trkseg>\n"
;

static const char FILE_DIVIDER[] =
	"    </trkseg>\n"
	"    <trkseg>\n"
;

static const char FILE_FOOTER[] =
	"    </trkseg>\n"
	"  </trk>\n"
	"</gpx>\n"
;
// The number of characters in the FILE_FOOTER (not including the terminating
// null) to allow easy truncating.
static const int FILE_FOOTER_LENGTH = sizeof(FILE_FOOTER)/sizeof(char) - 1;

// Fields:
//  * Latitude (float)
//  * Longitude (float)
//  * Elevation (float)
//  * Year (int)
//  * Month (int)
//  * Day (int)
//  * Hour (int)
//  * Min (int)
//  * Sec (int)
static const char FILE_ENTRY_FORMAT[] =
	"      <trkpt lat=\"%0.6f\" lon=\"%0.6f\"><ele>%0.2f</ele><time>%04d-%02d-%02dT%02d:%02d:%02dZ</time></trkpt>\n"
;

// An approximate (over estimate of the) maximum length of strings generated
// from FILE_ENTRY_FORMAT. Essentially just the length of the template with some
// additional space for the expansion of the fields.
static const int FILE_ENTRY_LENGTH = (sizeof(FILE_ENTRY_FORMAT)/sizeof(char)) + 6 + 6;


N3_SD_Log::N3_SD_Log( HardwareSPI   spi_
                    , unsigned long log_interval_
                    )
: spi(spi_)
, log_interval(log_interval_)
, last_entry_time(0)
, last_gps_fixed(false)
, card(spi, true) // Arg 2: Auto-setup the SPI interface
, card_working(false)
, volume_working(false)
, root_working(false)
, file_working(false)
{
	// Do nothing
}


void
N3_SD_Log::begin(void)
{
	// Run the at half speed since I don't wish to stress the tollerances of the
	// slightly iffy wiring...
	card_working = card.init(SPI_HALF_SPEED, -1);
	if (!card_working) return;
	
	// Open the first partition from MBR partition table
	volume_working = volume.init(&card, 1);
	if (!volume_working) return;
	
	// Open the root directory of the filesystem
	root_working = root.openRoot(&volume);
	if (!root_working) return;
	
	// If we've got this far, the file is probably going to work, assume it is for
	// now.
	file_working = true;
	
	// The file will be opened/closed upon every write by update()
	filename[0] = '\0';
}


bool
N3_SD_Log::working(void)
{
	return file_working;
}


void
N3_SD_Log::update(void)
{
	// Give up immediately if the SD card is not accessible (i.e. you'll need to
	// reset to try again)
	if (!file_working)
		return;
	
	if (millis() - last_entry_time > log_interval) {
		bool gps_fixed = n3_gps.is_fixed();
		
		// If the GPS is fixed, log our position
		if (gps_fixed) {
			file_working = open_file();
			if (file_working) {
				// Get the details for the receiver
				os_lat_lon_t coords    = n3_gps.get_coordinates();
				double       elevation = n3_gps.get_altitude();
				int year, month, day, hour, min, sec;
				n3_gps.get_datetime(&year, &month, &day, &hour, &min, &sec);
				
				// Format the log entry
				char entry[FILE_ENTRY_LENGTH];
				snprintf( entry, FILE_ENTRY_LENGTH, FILE_ENTRY_FORMAT
				        , RAD_2_DEG(coords.lat)
				        , RAD_2_DEG(coords.lon)
				        , elevation
				        , year, month, day
				        , hour, min,   sec
				        );
				entry[FILE_ENTRY_LENGTH-1] = '\0';
				file.write(entry);
				
				// Close the file until next time...
				file_working = close_file();
			}
		}
		
		
		last_gps_fixed = gps_fixed;
		
		// Reset the timer
		last_entry_time = millis();
	}
}


void
N3_SD_Log::set_filename(void)
{
	int year, month, day, hour, min, sec;
	n3_gps.get_datetime(&year, &month, &day, &hour, &min, &sec);
	// Two digit years (check out that future-proofing, man!)
	year %= 100;
	sprintf(filename, "%02d-%02d-%02d.gpx", year, month, day);
}


bool
N3_SD_Log::open_file(void)
{
	// Generate a filename if needed
	if (filename[0] == '\0') {
		set_filename();
	}
	
	// Try and open the file 
	if (file.open(&root, filename, O_RDWR)) {
		// File already exists, chop off the old footer
		if (!(file.truncate(file.fileSize() - FILE_FOOTER_LENGTH) && file.seekEnd())) {
			// Something went wrong
			file.close();
			return false;
		}
		
		// If GPS signal was previously lost and has just been reacquired add a
		// divider.
		if (last_gps_fixed == false) {
			file.write(FILE_DIVIDER);
		}
	} else if (file.open(&root, filename, O_RDWR | O_CREAT)) {
		// File does not exist (we just created it) so the header must be added and
		// then we're done!
		file.write(FILE_HEADER);
	} else {
		// The file could not be opened/created, give up!
		return false;
	}
	
	// All went well!
	return true;
}


bool
N3_SD_Log::close_file(void)
{
	// Add the footer to the file and close it
	file.write(FILE_FOOTER);
	file.close();
	return true;
}
