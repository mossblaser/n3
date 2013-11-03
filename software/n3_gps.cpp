#include "n3_gps.h"

#include "TinyGPS/TinyGPS.h"

#include "os_coord/os_coord.h"
#include "os_coord/os_coord_math.h"


N3_GPS::N3_GPS( HardwareSerial *serial_
              , uint32          baudrate
              )
{
	serial = serial_;
	serial->begin(baudrate);
}


void
N3_GPS::update(void)
{
	while (serial->available())
		gps.encode(serial->read());
}


bool
N3_GPS::is_fixed(void)
{
	unsigned long fix_age;
	gps.get_position(NULL, NULL, &fix_age);
	
	return fix_age != TinyGPS::GPS_INVALID_AGE && fix_age < GPS_MAX_AGE;
}


int
N3_GPS::get_num_satellites(void)
{
	return gps.satellites();
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
