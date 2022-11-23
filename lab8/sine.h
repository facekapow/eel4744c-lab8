/**
 * @file sine.h
 *
 * A header containing an array of sine wave data points,
 * along with some period values for various frequencies
 * when running at 32MHz using a prescaler of 1.
 *
 * Additionally, despite the name, this file also contains
 * data points for a triangle wave.
 *
 * @author Ariel Abreu
 */

#ifndef SINE_H_
#define SINE_H_

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

static const uint16_t triangle_data[] = {
	0x020,0x040,0x060,0x080,0x0a0,0x0c0,0x0e0,0x100,
	0x120,0x140,0x160,0x180,0x1a0,0x1c0,0x1e0,0x200,
	0x220,0x240,0x260,0x280,0x2a0,0x2c0,0x2e0,0x300,
	0x320,0x340,0x360,0x380,0x3a0,0x3c0,0x3e0,0x400,
	0x420,0x440,0x460,0x480,0x4a0,0x4c0,0x4e0,0x500,
	0x520,0x540,0x560,0x580,0x5a0,0x5c0,0x5e0,0x600,
	0x620,0x640,0x660,0x680,0x6a0,0x6c0,0x6e0,0x700,
	0x720,0x740,0x760,0x780,0x7a0,0x7c0,0x7e0,0x800,
	0x81f,0x83f,0x85f,0x87f,0x89f,0x8bf,0x8df,0x8ff,
	0x91f,0x93f,0x95f,0x97f,0x99f,0x9bf,0x9df,0x9ff,
	0xa1f,0xa3f,0xa5f,0xa7f,0xa9f,0xabf,0xadf,0xaff,
	0xb1f,0xb3f,0xb5f,0xb7f,0xb9f,0xbbf,0xbdf,0xbff,
	0xc1f,0xc3f,0xc5f,0xc7f,0xc9f,0xcbf,0xcdf,0xcff,
	0xd1f,0xd3f,0xd5f,0xd7f,0xd9f,0xdbf,0xddf,0xdff,
	0xe1f,0xe3f,0xe5f,0xe7f,0xe9f,0xebf,0xedf,0xeff,
	0xf1f,0xf3f,0xf5f,0xf7f,0xf9f,0xfbf,0xfdf,0xfff,
	0xfdf,0xfbf,0xf9f,0xf7f,0xf5f,0xf3f,0xf1f,0xeff,
	0xedf,0xebf,0xe9f,0xe7f,0xe5f,0xe3f,0xe1f,0xdff,
	0xddf,0xdbf,0xd9f,0xd7f,0xd5f,0xd3f,0xd1f,0xcff,
	0xcdf,0xcbf,0xc9f,0xc7f,0xc5f,0xc3f,0xc1f,0xbff,
	0xbdf,0xbbf,0xb9f,0xb7f,0xb5f,0xb3f,0xb1f,0xaff,
	0xadf,0xabf,0xa9f,0xa7f,0xa5f,0xa3f,0xa1f,0x9ff,
	0x9df,0x9bf,0x99f,0x97f,0x95f,0x93f,0x91f,0x8ff,
	0x8df,0x8bf,0x89f,0x87f,0x85f,0x83f,0x81f,0x800,
	0x7e0,0x7c0,0x7a0,0x780,0x760,0x740,0x720,0x700,
	0x6e0,0x6c0,0x6a0,0x680,0x660,0x640,0x620,0x600,
	0x5e0,0x5c0,0x5a0,0x580,0x560,0x540,0x520,0x500,
	0x4e0,0x4c0,0x4a0,0x480,0x460,0x440,0x420,0x400,
	0x3e0,0x3c0,0x3a0,0x380,0x360,0x340,0x320,0x300,
	0x2e0,0x2c0,0x2a0,0x280,0x260,0x240,0x220,0x200,
	0x1e0,0x1c0,0x1a0,0x180,0x160,0x140,0x120,0x100,
	0x0e0,0x0c0,0x0a0,0x080,0x060,0x040,0x020,0x000,
};

#define PERIOD_698HZ 182
#define PERIOD_798HZ 159
#define PERIOD_898HZ 141
#define PERIOD_998HZ 127

// C_6
#define PERIOD_1046_50HZ 119
// C#_6 / Db_6
#define PERIOD_1108_73HZ 113
// D_6
#define PERIOD_1174_66HZ 106
// D#_6 / Eb_6
#define PERIOD_1244_51HZ 100
// E_6
#define PERIOD_1318_51HZ 95
// F_6
#define PERIOD_1396_91HZ 89
// F#_6 / Gb_6
#define PERIOD_1479_98HZ 84
// G_6
#define PERIOD_1567_98HZ 80
// G#_6 / Ab_6
#define PERIOD_1661_22HZ 75
// A_6
#define PERIOD_1760_00HZ 71
// A#_6 / Bb_6
#define PERIOD_1864_66HZ 67
// B_6
#define PERIOD_1975_53HZ 63
// C_7
#define PERIOD_2093_00HZ 60

#endif /* SINE_H_ */