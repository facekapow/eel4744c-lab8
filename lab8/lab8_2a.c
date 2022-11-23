/*
 * Lab 8, Section 2a
 * Name: Ariel Abreu
 * Class #: 11882
 * PI Name: Ben
 * Description: Continuously generate a sine wave on port A pin 2 using interrupts
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#include <stdbool.h>

#include "clock.h"
#include "sine.h"
#include "timer.h"

static volatile bool interrupt_triggered = false;

// if 0, interrupts are used. if 1, synchronous polling is used.
#define USE_POLLING 0

int main(void) {
	timer_t wave_timer;
	uint16_t sine_data_offset = 0;

	clock_init();

	// configure the pin to be an output
	PORTA.DIRSET = PIN2_bm;

	// configure the DAC to enable channel 0 (pin A2) and use the 2.5V reference from port B
	DACA.CTRLB = DAC_CHSEL_SINGLE_gc;
	DACA.CTRLC = DAC_REFSEL_AREFB_gc;
	DACA.CTRLA = DAC_CH0EN_bm | DAC_ENABLE_bm;

	// configure and start a timer
	timer_init(&TCC0, 1, PERIOD_998HZ, USE_POLLING ? 0 : timer_init_flag_enable_interrupts, &wave_timer);
	timer_start(&wave_timer);

#if !USE_POLLING
	// loop forever
	while (1) {
		// wait for the flag to be set
		while (!interrupt_triggered);

		// clear the flag
		interrupt_triggered = false;

		// start the DAC conversion
		DACA.CH0DATA = sine_data[sine_data_offset];

		// increment the offset
		++sine_data_offset;

		// check if we've reached the end of the data.
		// if so, wrap back around.
		if (sine_data_offset >= sizeof(sine_data) / sizeof(*sine_data)) {
			sine_data_offset = 0;
		}
	}
#else
	while (1) {
		// wait for the timer interrupt flag to be set
		while ((TCC0.INTFLAGS & TC0_OVFIF_bm) == 0);

		// clear the interrupt flag
		TCC0.INTFLAGS = TC0_OVFIF_bm;

		// start the DAC conversion
		DACA.CH0DATA = sine_data[sine_data_offset];

		// increment the offset
		++sine_data_offset;

		// check if we've reached the end of the data.
		// if so, wrap back around.
		if (sine_data_offset >= sizeof(sine_data) / sizeof(*sine_data)) {
			sine_data_offset = 0;
		}
	}
#endif

	return 0;
};

#if !USE_POLLING
ISR(TCC0_OVF_vect) {
	// explicitly clear the interrupt flag
	TCC0.INTFLAGS = TC0_OVFIF_bm;

	// set the flag
	interrupt_triggered = true;
};
#endif
