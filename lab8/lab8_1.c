/*
 * Lab 8, Section 1
 * Name: Ariel Abreu
 * Class #: 11882
 * PI Name: Ben
 * Description: Continuously generate 2V with the DAC on port A pin 2.
 */

#include <avr/io.h>

#include "clock.h"


int main(void) {
	clock_init();

	// configure the pin to be an output
	PORTA.DIRSET = PIN2_bm;

	// configure the DAC to enable channel 0 (pin A2) and use the 2.5V reference from port B
	DACA.CTRLB = DAC_CHSEL_SINGLE_gc;
	DACA.CTRLC = DAC_REFSEL_AREFB_gc;
	DACA.CTRLA = DAC_CH0EN_bm | DAC_ENABLE_bm;

	while (1) {
		// wait until the data register is emptied
		while ((DACA.STATUS & DAC_CH0DRE_bm) == 0);

		// output = (ch_data / 0xfff) * vref
		//
		// ch_data = (output / vref) * 0xfff
		//         = (2 / 2.5) * 0xfff
		//         = 3276
		DACA.CH0DATA = 3276;
	}

	return 0;
};
