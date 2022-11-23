/**
 * @file timer.c
 *
 * A small library to configure and start timers.
 *
 * @author Ariel Abreu
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#include "timer.h"

void timer_init(TC0_t* avr_timer, uint16_t prescaler, uint16_t period, timer_init_flags_t flags, timer_t* out_timer) {
	// disable the timer
	avr_timer->CTRLA = TC_CLKSEL_OFF_gc;

	// configure it for normal wavegen (but we won't be using that)
	avr_timer->CTRLB = TC_WGMODE_NORMAL_gc;

	// initialize the count to 0
	avr_timer->CNT = 0;

	// initialize the period
	avr_timer->PER = period;

	// save the timer information
	out_timer->avr_timer = avr_timer;
	switch (prescaler) {
		case 0:
			out_timer->prescaler = TC_CLKSEL_OFF_gc;
			break;
		case 1:
			out_timer->prescaler = TC_CLKSEL_DIV1_gc;
			break;
		case 2:
			out_timer->prescaler = TC_CLKSEL_DIV2_gc;
			break;
		case 4:
			out_timer->prescaler = TC_CLKSEL_DIV4_gc;
			break;
		case 8:
			out_timer->prescaler = TC_CLKSEL_DIV8_gc;
			break;
		case 64:
			out_timer->prescaler = TC_CLKSEL_DIV64_gc;
			break;
		case 256:
			out_timer->prescaler = TC_CLKSEL_DIV256_gc;
			break;
		case 1024:
			out_timer->prescaler = TC_CLKSEL_DIV1024_gc;
			break;
	}

	// optionally, enable interrupts
	if (flags & timer_init_flag_enable_interrupts) {
		avr_timer->INTCTRLA = TC_OVFINTLVL_LO_gc;

		// enable interrupts in the PMIC and globally
		PMIC.CTRL |= PMIC_LOLVLEN_bm;
		sei();
	}
};

void timer_reset_count(timer_t* timer) {
	timer->avr_timer->CNT = 0;
};

void timer_stop(timer_t* timer) {
	timer->avr_timer->CTRLA = TC_CLKSEL_OFF_gc;
};

void timer_start(timer_t* timer) {
	timer->avr_timer->CTRLA = timer->prescaler;
};

void timer_change_period(timer_t* timer, uint16_t prescaler, uint16_t period) {
	timer->avr_timer->CTRLA = TC_CLKSEL_OFF_gc;
	timer->avr_timer->CNT = 0;

	timer->avr_timer->PER = period;

	switch (prescaler) {
		case 0:
			timer->prescaler = TC_CLKSEL_OFF_gc;
			break;
		case 1:
			timer->prescaler = TC_CLKSEL_DIV1_gc;
			break;
		case 2:
			timer->prescaler = TC_CLKSEL_DIV2_gc;
			break;
		case 4:
			timer->prescaler = TC_CLKSEL_DIV4_gc;
			break;
		case 8:
			timer->prescaler = TC_CLKSEL_DIV8_gc;
			break;
		case 64:
			timer->prescaler = TC_CLKSEL_DIV64_gc;
			break;
		case 256:
			timer->prescaler = TC_CLKSEL_DIV256_gc;
			break;
		case 1024:
			timer->prescaler = TC_CLKSEL_DIV1024_gc;
			break;
	}
};
