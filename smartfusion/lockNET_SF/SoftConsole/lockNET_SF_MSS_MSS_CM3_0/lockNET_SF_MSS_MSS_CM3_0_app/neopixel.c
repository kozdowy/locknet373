/*
 * neopixel.c
 *
 *  Created on: Nov 14, 2017
 *      Author: kozdowy
 */

#include "neopixel.h"

volatile uint32_t *NP_ADDRESS = (uint32_t*) 0x40050000;

void NP_init(void){
	// intentionally empty
}

void NP_set_pixel(uint32_t colour){
	*(NP_ADDRESS) = NP_CONTROL_SET | colour;
}

void NP_set_pixel_c(uint8_t red, uint8_t green, uint8_t blue){
	uint32_t set_colour = 0x00000000;
	set_colour |= ((uint32_t) green) << 16;
	set_colour |= ((uint32_t) red) << 8;
	set_colour |= (uint32_t) blue;
	set_colour |= NP_CONTROL_SET;
	*(NP_ADDRESS) = set_colour;
}

void NP_apply(void){
	*(NP_ADDRESS) = NP_CONTROL_APPLY;
}

void NP_clear(void){
	*(NP_ADDRESS) = NP_CONTROL_CLEAR;
}

uint32_t NP_get_pixel(void){
	return (uint32_t) *(NP_ADDRESS);
}
