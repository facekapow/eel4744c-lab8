/*
 * Lab 8, Section 3
 * Name: Ariel Abreu
 * Class #: 11882
 * PI Name: Ben
 * Description: Continuously generate a sine wave on port A pin 2 using DMA
 */

#include <avr/io.h>

#include "clock.h"

static const uint16_t sine_data[] = {
	0x800,0x832,0x864,0x896,0x8c8,0x8fa,0x92c,0x95e,
	0x98f,0x9c0,0x9f1,0xa22,0xa52,0xa82,0xab1,0xae0,
	0xb0f,0xb3d,0xb6b,0xb98,0xbc5,0xbf1,0xc1c,0xc47,
	0xc71,0xc9a,0xcc3,0xceb,0xd12,0xd39,0xd5f,0xd83,
	0xda7,0xdca,0xded,0xe0e,0xe2e,0xe4e,0xe6c,0xe8a,
	0xea6,0xec1,0xedc,0xef5,0xf0d,0xf24,0xf3a,0xf4f,
	0xf63,0xf76,0xf87,0xf98,0xfa7,0xfb5,0xfc2,0xfcd,
	0xfd8,0xfe1,0xfe9,0xff0,0xff5,0xff9,0xffd,0xffe,
	0xfff,0xffe,0xffd,0xff9,0xff5,0xff0,0xfe9,0xfe1,
	0xfd8,0xfcd,0xfc2,0xfb5,0xfa7,0xf98,0xf87,0xf76,
	0xf63,0xf4f,0xf3a,0xf24,0xf0d,0xef5,0xedc,0xec1,
	0xea6,0xe8a,0xe6c,0xe4e,0xe2e,0xe0e,0xded,0xdca,
	0xda7,0xd83,0xd5f,0xd39,0xd12,0xceb,0xcc3,0xc9a,
	0xc71,0xc47,0xc1c,0xbf1,0xbc5,0xb98,0xb6b,0xb3d,
	0xb0f,0xae0,0xab1,0xa82,0xa52,0xa22,0x9f1,0x9c0,
	0x98f,0x95e,0x92c,0x8fa,0x8c8,0x896,0x864,0x832,
	0x800,0x7cd,0x79b,0x769,0x737,0x705,0x6d3,0x6a1,
	0x670,0x63f,0x60e,0x5dd,0x5ad,0x57d,0x54e,0x51f,
	0x4f0,0x4c2,0x494,0x467,0x43a,0x40e,0x3e3,0x3b8,
	0x38e,0x365,0x33c,0x314,0x2ed,0x2c6,0x2a0,0x27c,
	0x258,0x235,0x212,0x1f1,0x1d1,0x1b1,0x193,0x175,
	0x159,0x13e,0x123,0x10a,0x0f2,0x0db,0x0c5,0x0b0,
	0x09c,0x089,0x078,0x067,0x058,0x04a,0x03d,0x032,
	0x027,0x01e,0x016,0x00f,0x00a,0x006,0x002,0x001,
	0x000,0x001,0x002,0x006,0x00a,0x00f,0x016,0x01e,
	0x027,0x032,0x03d,0x04a,0x058,0x067,0x078,0x089,
	0x09c,0x0b0,0x0c5,0x0db,0x0f2,0x10a,0x123,0x13e,
	0x159,0x175,0x193,0x1b1,0x1d1,0x1f1,0x212,0x235,
	0x258,0x27c,0x2a0,0x2c6,0x2ed,0x314,0x33c,0x365,
	0x38e,0x3b8,0x3e3,0x40e,0x43a,0x467,0x494,0x4c2,
	0x4f0,0x51f,0x54e,0x57d,0x5ad,0x5dd,0x60e,0x63f,
	0x670,0x6a1,0x6d3,0x705,0x737,0x769,0x79b,0x7cd,
};

#define PERIOD_1567_98HZ 80

int main(void) {
	clock_init();

	// configure the pin to be an output
	PORTA.DIRSET = PIN2_bm;

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
	DMA.CH0.DESTADDR0 = (uint8_t)(((uint32_t)&DACA.CH0DATA >> 0) & 0xff);
	DMA.CH0.DESTADDR1 = (uint8_t)(((uint32_t)&DACA.CH0DATA >> 8) & 0xff);
	DMA.CH0.DESTADDR2 = (uint8_t)(((uint32_t)&DACA.CH0DATA >> 16) & 0xff);

	DMA.CH0.CTRLA |= DMA_CH_ENABLE_bm;

	DMA.CTRL |= DMA_ENABLE_bm;

	// configure the DAC to enable channel 0 (pin A2) and use the 2.5V reference from port B
	DACA.CTRLB = DAC_CHSEL_SINGLE_gc;
	DACA.CTRLC = DAC_REFSEL_AREFB_gc;
	DACA.CTRLA = DAC_CH0EN_bm | DAC_ENABLE_bm;
	DACA.CH0DATA = 0;

	// configure and start a timer
	TCC0.CTRLA = TC_CLKSEL_OFF_gc;
	TCC0.CTRLB = TC_WGMODE_NORMAL_gc;
	TCC0.CNT = 0;
	TCC0.PER = PERIOD_1567_98HZ;
	TCC0.CTRLA = TC_CLKSEL_DIV1_gc;

	// configure event channel 0 to use the timer as an event source
	EVSYS.CH0MUX = EVSYS_CHMUX_TCC0_OVF_gc;

	// loop forever
	while (1);

	return 0;
};
