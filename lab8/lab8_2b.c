/*
 * Lab 8, Section 2b
 * Name: Ariel Abreu
 * Class #: 11882
 * PI Name: Ben
 * Description: Continuously generate a sine wave on port A pin 2 using events
 */

#include <avr/io.h>

#include "clock.h"
#include "sine.h"
#include "timer.h"

int main(void) {
	timer_t wave_timer;
	uint16_t sine_data_offset = 0;

	clock_init();

	// configure the pin to be an output
	PORTA.DIRSET = PIN2_bm;

	// configure the DAC to enable channel 0 (pin A2) and use the 2.5V reference from port B
	// (and also to use an event channel to trigger conversions)
	DACA.CTRLB = DAC_CHSEL_SINGLE_gc | DAC_CH0TRIG_bm;
	DACA.CTRLC = DAC_REFSEL_AREFB_gc;
	DACA.CTRLA = DAC_CH0EN_bm | DAC_ENABLE_bm;
	DACA.EVCTRL = DAC_EVSEL_0_gc;

	// configure and start a timer
	timer_init(&TCC0, 1, PERIOD_1567_98HZ, 0, &wave_timer);

	// configure event channel 0 to use the timer as an event source
	EVSYS.CH0MUX = EVSYS_CHMUX_TCC0_OVF_gc;

	timer_start(&wave_timer);

	while (1) {
		// wait for the data register to be emptied
		while ((DACA.STATUS & DAC_CH0DRE_bm) == 0);

		// update the DAC data
		DACA.CH0DATA = sine_data[sine_data_offset];

		// increment the offset
		++sine_data_offset;

		// check if we've reached the end of the data.
		// if so, wrap back around.
		if (sine_data_offset >= sizeof(sine_data) / sizeof(*sine_data)) {
			sine_data_offset = 0;
		}
	}

	return 0;
};
