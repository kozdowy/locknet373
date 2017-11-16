/*
 * neopixel.h
 *
 *  Created on: Nov 14, 2017
 *      Author: kozdowy
 */

#ifndef NEOPIXEL_H_
#define NEOPIXEL_H_

#include <stdint.h>

#define NP_ADDR 0x40050000
#define NP_CONTROL_SET 0x01000000
#define NP_CONTROL_APPLY 0x02000000
#define NP_CONTROL_CLEAR 0x04000000

#define NP_COLOUR_BLUE 0x00208800
#define NP_COLOUR_ORANGE 0x0083A400
#define NP_COLOUR_GREEN 0x00901030
#define NP_COLOUR_RED 0x00209925

void NP_init(void);

void NP_set_pixel(uint32_t colour);

void NP_set_pixel_c(uint8_t red, uint8_t green, uint8_t blue);

void NP_apply(void);

void NP_clear(void);

uint32_t NP_get_pixel(void);

#endif /* NEOPIXEL_H_ */
