#include <stdio.h>

#include <libmaple/dma.h>
#include <libmaple/usart.h>
#include <wirish/wirish.h>

#include "TinyGPS/TinyGPS.h"

#include "os_coord/os_coord.h"
#include "os_coord/os_coord_math.h"

#include "n3_gps.h"


// Convenient name for the DMA controller connected ot the USARTs
#define USART_DMA_DEV DMA1

// Alias for the channel/stream for USARTS on various STM32s
#if STM32_MCU_SERIES == STM32_SERIES_F1
// On STM32F1 microcontrollers (like what's on Maple and Maple Mini),
// dma tubes are channels.
#define USART_RX_DMA_TUBE DMA_CH6
#elif (STM32_MCU_SERIES == STM32_SERIES_F2 || \
       STM32_MCU_SERIES == STM32_SERIES_F4)
// On STM32F2 and STM32F4 microcontrollers (Maple 2 will have an F4),
// dma tubes are streams.
#define USART_RX_DMA_TUBE DMA_S5
#else
#error "unsupported stm32 series"
#endif


static void do_nothing_isr(void)
{
	// Do nothing (except for clearing the bits indicating the progress of the DMA!)
	dma_clear_isr_bits(USART_DMA_DEV, USART_RX_DMA_TUBE);
}


N3_GPS::N3_GPS( HardwareSerial *serial_
              , dma_request_src serial_dma_src_
              , uint32          baudrate_
              )
: serial(serial_)
, serial_dma_src(serial_dma_src_)
, baudrate(baudrate_)
{
	// Do nothing
}


void
N3_GPS::begin(void)
{
	serial->begin(baudrate);
	// Enable the USART's DMA signal for its receiver
	serial->c_dev()->regs->CR3 = USART_CR3_DMAR;
	
	// Set frequency of position calculation to 1Hz
	char set_fix_freq_cmd[] = "$PMTK300,1000,0,0,0,0*??\r\n";
	add_checksum(set_fix_freq_cmd);
	serial->print(set_fix_freq_cmd);
	
	// Send only RMC, GGA, GSA messages (and do so at the same rate as position
	// calculations)
	//
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
	
	// Set up DMA to dump serial data into an in-memory buffer and provide
	// interrupts every time half the buffer is filled just in-case nothing woke
	// the device up to allow the software to snatch the data from the buffer
	// first.
	
	// Start the DMA controller for the USART's events
	dma_init(USART_DMA_DEV);
	
	dma_tube_config tube_config;
	// We're reading from the serial port's data register
	tube_config.tube_src = &(serial->c_dev()->regs->DR);
	
	// Grab the 8-bit value in the register
	tube_config.tube_src_size = DMA_SIZE_8BITS;
	
	// Put stuff into the DMA buffer
	tube_config.tube_dst = dma_buffer;
	
	// ...which takes 8-bit values too
	tube_config.tube_dst_size = DMA_SIZE_8BITS;
	
	// Use the whole buffer
	tube_config.tube_nr_xfers = N3_GPS_DMA_BUFFER_SIZE;
	
	// Flags:
	// - DMA_CFG_DST_INC: Fill the buffer stepping forward after each value
	// - DMA_CFG_CIRC: The buffer is a circular buffer
	// - DMA_CFG_CMPLT_IE interrupt when the buffer is filled completely
	// - DMA_CFG_HALF_CMPLT_IE interrupt when the buffer is half full
	tube_config.tube_flags = DMA_CFG_DST_INC | DMA_CFG_CIRC | DMA_CFG_CMPLT_IE | DMA_CFG_HALF_CMPLT_IE;
	
	// Always null (microcontroller specific only values here)
	tube_config.target_data = NULL;
	
	// The DMA events to listen for are those from the USART.
	tube_config.tube_req_src = serial_dma_src;
	
	int status = dma_tube_cfg(USART_DMA_DEV, USART_RX_DMA_TUBE, &tube_config);
	ASSERT(status == DMA_TUBE_CFG_SUCCESS);
	
	// Attach the null ISR
	dma_attach_interrupt(USART_DMA_DEV, USART_RX_DMA_TUBE, do_nothing_isr);
	
	// Initialise the count of re
	dma_buffer_read_head = 0;
	
	// Set the ball rolling
	dma_enable(USART_DMA_DEV, USART_RX_DMA_TUBE);
}


int
N3_GPS::get_dma_buffer_write_head(void)
{
	return N3_GPS_DMA_BUFFER_SIZE 
	       - dma_tube_regs(USART_DMA_DEV, USART_RX_DMA_TUBE)->CNDTR;
}


void
N3_GPS::update(void)
{
	while (dma_buffer_read_head != get_dma_buffer_write_head()) {
		gps.encode(*(dma_buffer + dma_buffer_read_head));
		dma_buffer_read_head = (dma_buffer_read_head+1)%N3_GPS_DMA_BUFFER_SIZE;
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
