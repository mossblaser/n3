/**
 * Setup the board and its peripherals.
 */

#include <wirish/wirish.h>

#include "n3.h"
#include "n3_bat.h"


N3_Bat::N3_Bat( uint8 pin_
              , double r1
              , double r2
              , double a_ref_v
              )
: pin(pin_)
, a_in_to_v(((r2/(r1+r2))*a_ref_v) / 1024.0)
{
	// Nothing to do
}


void
N3_Bat::begin(void)
{
	pinMode(pin, INPUT_ANALOG);
}


double
N3_Bat::get_voltage(void)
{
	return ((double)analogRead(pin)) * a_in_to_v;
}
