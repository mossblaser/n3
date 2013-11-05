/**
 * Battery Level Monitoring
 */

#ifndef N3_BAT_H
#define N3_BAT_H


/**
 * Battery level reader.
 *
 * Assumes a circuit like the following:
 *
 *      ,------,
 *      | +    |
 *    --'--    Z R1
 *     ===     Z
 *      |      +----- Analog Pin
 *    --'--    Z
 *     ===     Z R2
 *      | -    |
 *      `------'
 *
 * Where the following is known:
 * * Voltage reference used by the analog input
 * * Resistances of the two resistors
 */
class N3_Bat {
	public:
		/**
		 * Constructor.
		 *
		 * @param pin An analog pin through which to read a potential divider on the
		 *            battery's +ve output.
		 * @param r1 The value of the resistor between the bat+ and the analog pin
		 * @param r2 The value of the resistor between the bat- and the analog pin
		 * @param a_ref_v Analog pin's reference voltage
		 */
		N3_Bat( uint8 pin
		      , double r1
		      , double r2
		      , double a_ref_v
		      );
		
		/**
		 * Set up the analog pin.
		 */
		void begin(void);
		
		double get_voltage(void);
		
	protected:
		uint8  pin;
		
		// Multiply an analog input (0-1) by this value to convert to voltage
		double a_in_to_v;
};


#endif

