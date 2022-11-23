/**
 * @file usart.c
 *
 * A small library to use USARTD0 to communicate with a device over USB.
 *
 * @author Ariel Abreu
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#include "usart.h"

enum usart_pins {
	usart_pin_tx = 3,
	usart_pin_rx = 2,

	usart_pin_tx_mask = 1 << usart_pin_tx,
	usart_pin_rx_mask = 1 << usart_pin_rx,
};

void usart_init(int8_t bscale, uint16_t bsel, uint8_t data_bits, usart_parity_t parity, bool two_stop_bits, usart_init_flags_t flags) {
	// initialize the TX pin (but write it to 1 first to make it idle)
	PORTD.OUTSET = usart_pin_tx_mask;
	PORTD.DIRSET = usart_pin_tx_mask;
	PORTD.PIN3CTRL = PORT_OPC_TOTEM_gc;

	// disable the USART to configure it
	USARTD0.CTRLB = (flags & usart_init_flag_double_clock) ? USART_CLK2X_bm : 0;

	// initialize the RX pin (if it was requested)
	if (flags & usart_init_flag_enable_rx) {
		PORTD.DIRCLR = usart_pin_rx_mask;
		PORTD.PIN2CTRL = PORT_OPC_PULLUP_gc;

		// also enable the RxC interrupt
		USARTD0.CTRLA = USART_RXCINTLVL_LO_gc;

		// clear the interrupt flag
		USARTD0.STATUS = USART_RXCIF_bm;
	}

	// configure the protocol
	USARTD0.CTRLC = USART_CMODE_ASYNCHRONOUS_gc;

	switch (data_bits) {
		case 5:
			USARTD0.CTRLC |= USART_CHSIZE_5BIT_gc;
			break;
		case 6:
			USARTD0.CTRLC |= USART_CHSIZE_6BIT_gc;
			break;
		case 7:
			USARTD0.CTRLC |= USART_CHSIZE_7BIT_gc;
			break;
		case 8:
			USARTD0.CTRLC |= USART_CHSIZE_8BIT_gc;
			break;
		case 9:
			USARTD0.CTRLC |= USART_CHSIZE_9BIT_gc;
			break;
	}

	switch (parity) {
		case usart_parity_none:
			USARTD0.CTRLC |= USART_PMODE_DISABLED_gc;
			break;
		case usart_parity_odd:
			USARTD0.CTRLC |= USART_PMODE_ODD_gc;
			break;
		case usart_parity_even:
			USARTD0.CTRLC |= USART_PMODE_EVEN_gc;
			break;
	}

	if (two_stop_bits) {
		USARTD0.CTRLC |= USART_SBMODE_bm;
	}

	USARTD0.BAUDCTRLA = bsel & 0xff;
	USARTD0.BAUDCTRLB = (((uint8_t)bscale << 4) & 0xf0) | ((bsel >> 8) & 0x0f);

	// enable the TX module
	USARTD0.CTRLB |= USART_TXEN_bm;

	// enable the receiver (and finish setting up interrupts), if requested
	if (flags & usart_init_flag_enable_rx) {
		USARTD0.CTRLB |= USART_RXEN_bm;

		// enable low-priority interrupts in the PMIC
		PMIC.CTRL |= PMIC_LOLVLEN_bm;

		// enable interrupts globally
		sei();
	}
};

void usart_write(uint8_t data) {
	// wait for the data register to be emptied
	while ((USARTD0.STATUS & USART_DREIF_bm) == 0);

	// write the data
	USARTD0.DATA = data;
};

uint8_t usart_read(bool wait) {
	// wait for something to be received (if requested)
	if (wait) {
		while ((USARTD0.STATUS & USART_RXCIF_bm) == 0);
	}

	// read the data
	return USARTD0.DATA;
};
