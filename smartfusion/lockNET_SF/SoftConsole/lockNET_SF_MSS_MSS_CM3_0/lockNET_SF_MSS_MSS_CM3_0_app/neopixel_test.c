#include <stdio.h>
#include "neopixel.h"
#include "servo.h"

int np_test()
{
	NP_init();

	NP_set_pixel_c(0x88, 0xBB, 0xEE);
	NP_apply();

	int i;
	for (i = 0; i < 100000; ++i){
	}
	NP_set_pixel(0x0000FF00);
	NP_apply();

	for (i = 0; i < 100000; ++i){
	}
	NP_set_pixel(NP_COLOUR_BLUE);
	NP_apply();

	for (i = 0; i < 100000; ++i){
	}
	NP_set_pixel(NP_COLOUR_ORANGE);
	NP_apply();

	for (i = 0; i < 100000; ++i){
	}
	NP_set_pixel(NP_COLOUR_GREEN);
	NP_apply();

	for (i = 0; i < 100000; ++i){
	}
	NP_set_pixel(NP_COLOUR_RED);
	NP_apply();

	return 0;
}
