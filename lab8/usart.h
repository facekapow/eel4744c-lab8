/**
 * @file usart.h
 *
 * A small library to use USARTD0 to communicate with a device over USB.
 *
 * @author Ariel Abreu
 */


#ifndef _USART_H_
#define _USART_H_

#include <stdbool.h>

typedef enum usart_init_flags {
	/**
	 * If set, also enables RX on the USART.
	 * This also enables the receive-complete interrupt (RxC).
	 */
	usart_init_flag_enable_rx = 1 << 0,

	/**
	 * If set, CLK2X is set, doubling the clock speed of the USART.
	 */
	usart_init_flag_double_clock = 1 << 1,
} usart_init_flags_t;

typedef enum usart_parity {
	usart_parity_none = 0,
	usart_parity_odd = 1,
	usart_parity_even = 2,
} usart_parity_t;

/**
 * @brief Initializes USARTD0 for communication with a device over USB.
 *
 * By default, only TX is enabled, but RX can be enabled with the appropriate init flag.
 *
 * @param flags An optional set of flags to modify how the USART is initialized.
 */
void usart_init(int8_t bscale, uint16_t bsel, uint8_t data_bits, usart_parity_t parity, bool two_stop_bits, usart_init_flags_t flags);

/**
 * @brief Transmits the given 8-bit value over the USART.
 *
 * @param data The 8-bit value to transmit.
 */
void usart_write(uint8_t data);

/**
 * @brief Receives an 8-bit value from the USART.
 *
 * @param wait If `true`, the function will wait until a value is received before reading it. The only time you'd probably want to
 *             pass `false` here is when you already know the value has been received (e.g. in an ISR).
 */
uint8_t usart_read(bool wait);

#endif /* _USART_H_ */