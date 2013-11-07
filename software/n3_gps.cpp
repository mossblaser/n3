#include <stdio.h>
#include <wirish/wirish.h>

#include "n3_gps.h"

#include "TinyGPS/TinyGPS.h"

#include "os_coord/os_coord.h"
#include "os_coord/os_coord_math.h"


N3_GPS::N3_GPS( HardwareSerial *serial_
              , uint32          baudrate_
              )
: serial(serial_)
, baudrate(baudrate_)
{
	// Do nothing
}


void
N3_GPS::begin(void)
{
	serial->begin(baudrate);
	
	// Set frequency of position calculation to 1Hz
	char set_fix_freq_cmd[] = "$PMTK300,1000,0,0,0,0*??\r\n";
	add_checksum(set_fix_freq_cmd);
	serial->print(set_fix_freq_cmd);
	
	// Send RMC, GGA, GSA messages only
	// Field: 0 NMEA_SEN_GLL -----------,
	// Field: 1 NMEA_SEN_RMC -----------+-,
	// Field: 2 NMEA_SEN_VTG -----------+-+-,
	// Field: 3 NMEA_SEN_GGA -----------+-+-+-,
	// Field: 4 NMEA_SEN_GSA -----------+-+-+---,
	// Field: 5 NMEA_SEN_GSV -----------+-+-+-+-+-,
	// Field: 6 NMEA_SEN_GRS -----------+-+-+-+-+-+-,
	// Field: 7 NMEA_SEN_GST -----------+-+-+-+-+-+-+-,
	// Field: 13 NMEA_SEN_MALM ---------+-+-+-+-+-+-+-+-----------,
	// Field: 14 NMEA_SEN_MEPH ---------+-+-+-+-+-+-+-+-----------+-,
	// Field: 15 NMEA_SEN_MDGP ---------+-+-+-+-+-+-+-+-----------+-+-,
	// Field: 16 NMEA_SEN_MDBG ---------+-+-+-+-+-+-+-+-----------+-+-+-,
	// Field: 17 NMEA_SEN_ZDA ----------+-+-+-+-+-+-+-+-----------+-+-+-+-,
	// Field: 18 NMEA_SEN_MCHN ---------+-+-+-+-+-+-+-+-----------+-+-+-+-+-,
	//                                  | | | | | | | |           | | | | | |
	char set_msg_freq_cmd[] = "$PMTK314,0,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0*??\r\n";
	add_checksum(set_msg_freq_cmd);
	serial->print(set_msg_freq_cmd);
}


void
N3_GPS::update(void)
{
	while (serial->available()) {
		gps.encode(serial->read());
	}
}


bool
N3_GPS::is_fixed(void)
{
	unsigned long fix_age;
	gps.get_position(NULL, NULL, &fix_age);
	
	return fix_age != TinyGPS::GPS_INVALID_AGE && fix_age < GPS_MAX_AGE;
}


n3_gps_fix_type_t
N3_GPS::fix_type(void)
{
	return (n3_gps_fix_type_t)gps.fix_mode2();
}


int
N3_GPS::get_num_satellites(void)
{
	return gps.satellites();
}


double
N3_GPS::get_altitude(void)
{
	return ((double)gps.altitude())/100.0;
}


os_lat_lon_t
N3_GPS::get_coordinates(void)
{
	long   llat, llon;
	gps.get_position(&llat, &llon);
	
	os_lat_lon_t coord;
	coord.lat = DEG_2_RAD(((double)llat)/1000000.0);
	coord.lon = DEG_2_RAD(((double)llon)/1000000.0);
	
	coord.eh = ((double)gps.ellipsoidal_height())/100.0;
	
	return coord;
}


double
N3_GPS::get_course(void)
{
	return ((double)gps.course())/100.0;
}


double
N3_GPS::get_speed(void)
{
	return (((double)gps.speed())/100.0) * _GPS_KMPH_PER_KNOT;
}


void
N3_GPS::get_datetime(int *yr_, int *mo_, int *dy_, int *hr_, int *mi_, int *se_)
{
	int  yr;
	byte mo;
	byte dy;
	byte hr;
	byte mi;
	byte se;
	
	gps.crack_datetime(&yr, &mo, &dy, &hr, &mi, &se);
	
	*yr_ = yr;
	*mo_ = mo;
	*dy_ = dy;
	*hr_ = hr;
	*mi_ = mi;
	*se_ = se;
}



void
N3_GPS::add_checksum(char *nema_string)
{
	int checksum = 0;
	
	// Skip the initial dollar
	nema_string++;
	
	while(*nema_string != '*')
		checksum ^= (int)(*nema_string++);
	
	// Skip the star and add the checksum
	snprintf(++nema_string, 5, "%02X\r\n", checksum);
}
