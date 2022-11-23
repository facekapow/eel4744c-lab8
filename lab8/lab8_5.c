/*
 * Lab 8, Section 5
 * Name: Ariel Abreu
 * Class #: 11882
 * PI Name: Ben
 * Description: A synthesizer using input from the computer (via USART), output to the speaker
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#include "clock.h"
#include "sine.h"
#include "usart.h"
#include "timer.h"

// 50ms
#define MINIMUM_TONE_PRESCALER 64
#define MINIMUM_TONE_PERIOD 25000

// 2Mbps baud rate
#define BSEL 0
#define BSCALE 0

static volatile bool did_receive_character = false;
static volatile char received_character;

static timer_t wave_timer;
static timer_t stop_wave_timer;

/**
 * @brief Plays the note with the given frequency on the speaker.
 */
static void play_note(uint16_t frequency_period);

int main(void) {
	bool using_triangle = false;

	clock_init();
	usart_init(BSCALE, BSEL, 8, usart_parity_none, false, usart_init_flag_enable_rx);
	timer_init(&TCC0, 1, PERIOD_1567_98HZ, 0, &wave_timer);
	timer_init(&TCD0, MINIMUM_TONE_PRESCALER, MINIMUM_TONE_PERIOD, timer_init_flag_enable_interrupts, &stop_wave_timer);

	// configure event channel 0 to use the timer as an event source
	EVSYS.CH0MUX = EVSYS_CHMUX_TCC0_OVF_gc;

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
	DACA.CTRLA = DAC_ENABLE_bm;
	DACA.CH1DATA = 0;

	// wait for keys
	while (true) {
		// wait until we receive a character
		while (!did_receive_character);

		// clear the flag
		did_receive_character = false;

		// copy the character
		char copy = received_character;

		switch (copy) {
			case 'E':
			case 'e':
				play_note(PERIOD_1046_50HZ);
				break;

			case '4':
			case '$':
				play_note(PERIOD_1108_73HZ);
				break;

			case 'R':
			case 'r':
				play_note(PERIOD_1174_66HZ);
				break;

			case '5':
			case '%':
				play_note(PERIOD_1244_51HZ);
				break;

			case 'T':
			case 't':
				play_note(PERIOD_1318_51HZ);
				break;

			case 'Y':
			case 'y':
				play_note(PERIOD_1396_91HZ);
				break;

			case '7':
			case '&':
				play_note(PERIOD_1479_98HZ);
				break;

			case 'U':
			case 'u':
				play_note(PERIOD_1567_98HZ);
				break;

			case '8':
			case '*':
				play_note(PERIOD_1661_22HZ);
				break;

			case 'I':
			case 'i':
				play_note(PERIOD_1760_00HZ);
				break;

			case '9':
			case '(':
				play_note(PERIOD_1864_66HZ);
				break;

			case 'O':
			case 'o':
				play_note(PERIOD_1975_53HZ);
				break;

			case 'P':
			case 'p':
				play_note(PERIOD_2093_00HZ);
				break;

			case 'S':
			case 's':
				// switch from sine to triangle or vice versa

				// first, disable the DMA channel
				DMA.CH0.CTRLA &= ~DMA_CH_ENABLE_bm;

				if (using_triangle) {
					// switch to sine
					DMA.CH0.SRCADDR0 = (uint8_t)(((uint32_t)&sine_data[0] >> 0) & 0xff);
					DMA.CH0.SRCADDR1 = (uint8_t)(((uint32_t)&sine_data[0] >> 8) & 0xff);
					DMA.CH0.SRCADDR2 = (uint8_t)(((uint32_t)&sine_data[0] >> 16) & 0xff);
					DMA.CH0.TRFCNT = sizeof(sine_data);
					using_triangle = false;
				} else {
					// switch to triangle
					DMA.CH0.SRCADDR0 = (uint8_t)(((uint32_t)&triangle_data[0] >> 0) & 0xff);
					DMA.CH0.SRCADDR1 = (uint8_t)(((uint32_t)&triangle_data[0] >> 8) & 0xff);
					DMA.CH0.SRCADDR2 = (uint8_t)(((uint32_t)&triangle_data[0] >> 16) & 0xff);
					DMA.CH0.TRFCNT = sizeof(triangle_data);
					using_triangle = true;
				}

				// now re-enable the DMA channel
				DMA.CH0.CTRLA |= DMA_CH_ENABLE_bm;
				break;
		}
	}

	return 0;
};

static void play_note(uint16_t frequency_period) {
	// update the waveform period to generate this note
	timer_change_period(&wave_timer, 1, frequency_period);

	// start the waveform timer to start generating audio waves
	timer_start(&wave_timer);

	// enable the DAC channel
	DACA.CTRLA |= DAC_CH1EN_bm;

	// reset and start the "stop wave" timer
	timer_reset_count(&stop_wave_timer);
	timer_start(&stop_wave_timer);
};

ISR(USARTD0_RXC_vect) {
	received_character = usart_read(false);
	did_receive_character = true;
};

ISR(TCD0_OVF_vect) {
	// stop the wave timer to stop generating audio waves
	timer_stop(&wave_timer);

	// disable the DAC channel
	DACA.CTRLA &= ~DAC_CH1EN_bm;

	// stop this timer (the "stop wave" timer)
	timer_stop(&stop_wave_timer);
};
