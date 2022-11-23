/*
 * Lab 8, Section 4
 * Name: Ariel Abreu
 * Class #: 11882
 * PI Name: Ben
 * Description: Continuously generate a sine wave for a G_6 note on the speaker
 */

#include <avr/io.h>

#include "clock.h"
#include "sine.h"
#include "timer.h"

int main(void) {
	timer_t wave_timer;

	clock_init();

	// configure the speaker pin to be an output
	PORTA.DIRSET = PIN3_bm;

	// configure the amplifier shutdown pin to be an output and set it high to prevent shutdown
	PORTC.DIRSET = PIN7_bm;
	PORTC.OUTSET = PIN7_bm;

	// configure the DMA controller
	//
	// first, reset it
	DMA.CTRL |= DMA_RESET_bm;

	// set REPCNT to 0 to repeat an unlimited number of times
	DMA.CH0.REPCNT = 0;
	DMA.CH0.CTRLA = DMA_CH_REPEAT_bm | DMA_CH_SINGLE_bm | DMA_CH_BURSTLEN_2BYTE_gc;
	DMA.CH0.ADDRCTRL = DMA_CH_SRCRELOAD_BLOCK_gc | DMA_CH_SRCDIR_INC_gc | DMA_CH_DESTRELOAD_BURST_gc | DMA_CH_DESTDIR_INC_gc;
	DMA.CH0.TRIGSRC = DMA_CH_TRIGSRC_EVSYS_CH0_gc;
	DMA.CH0.TRFCNT = sizeof(sine_data);
	DMA.CH0.SRCADDR0 = (uint8_t)(((uint32_t)&sine_data[0] >> 0) & 0xff);
	DMA.CH0.SRCADDR1 = (uint8_t)(((uint32_t)&sine_data[0] >> 8) & 0xff);
	DMA.CH0.SRCADDR2 = (uint8_t)(((uint32_t)&sine_data[0] >> 16) & 0xff);
	DMA.CH0.DESTADDR0 = (uint8_t)(((uint32_t)&DACA.CH1DATA >> 0) & 0xff);
	DMA.CH0.DESTADDR1 = (uint8_t)(((uint32_t)&DACA.CH1DATA >> 8) & 0xff);
	DMA.CH0.DESTADDR2 = (uint8_t)(((uint32_t)&DACA.CH1DATA >> 16) & 0xff);

	DMA.CH0.CTRLA |= DMA_CH_ENABLE_bm;

	DMA.CTRL |= DMA_ENABLE_bm;

	// configure the DAC to enable channel 1 (pin A3) and use the 2.5V reference from port B
	DACA.CTRLB = DAC_CHSEL_SINGLE1_gc;
	DACA.CTRLC = DAC_REFSEL_AREFB_gc;
	DACA.CTRLA = DAC_CH1EN_bm | DAC_ENABLE_bm;
	DACA.CH1DATA = 0;

	// configure and start a timer
	timer_init(&TCC0, 1, PERIOD_1567_98HZ, 0, &wave_timer);

	// configure event channel 0 to use the timer as an event source
	EVSYS.CH0MUX = EVSYS_CHMUX_TCC0_OVF_gc;

	timer_start(&wave_timer);

	// loop forever
	while (1);

	return 0;
};
