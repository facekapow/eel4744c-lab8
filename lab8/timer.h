/**
 * @file timer.h
 *
 * A small library to configure and start timers.
 *
 * @author Ariel Abreu
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <avr/io.h>

typedef enum timer_init_flags {
	timer_init_flag_enable_interrupts = 1 << 0,
} timer_init_flags_t;

typedef struct timer {
	TC0_t* avr_timer;
	TC_CLKSEL_t prescaler;
} timer_t;

/**
 * @brief Initializes the given timer with the given settings, but does NOT start it.
 *
 * @param avr_timer The underlying AVR timer to use.
 * @param prescaler The prescaler to use.
 * @param period    The period to use.
 * @param flags     An optional set of flags to modify how the timer is initialized.
 * @param out_timer The timer structure being initialized.
 */
void timer_init(TC0_t* avr_timer, uint16_t prescaler, uint16_t period, timer_init_flags_t flags, timer_t* out_timer);

/**
 * @brief Resets the count of the given timer to 0.
 */
void timer_reset_count(timer_t* timer);

/**
 * @brief Stops the given timer.
 */
void timer_stop(timer_t* timer);

/**
 * @brief Starts the given timer.
 */
void timer_start(timer_t* timer);

/**
 * @brief Changes the period (and prescaler) of the given timer.
 *
 * @note This function will also stop the timer and reset the count.
 */
void timer_change_period(timer_t* timer, uint16_t prescaler, uint16_t period);

#endif /* TIMER_H_ */