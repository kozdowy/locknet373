/*
 * contact_switch.h
 *
 *  Created on: Nov 16, 2017
 *      Author: kozdowy
 */

#ifndef CONTACT_SWITCH_H_
#define CONTACT_SWITCH_H_

#include <stdint.h>
#include <drivers/mss_gpio/mss_gpio.h>

#define CS_DOOR_OPEN 1
#define CS_DOOR_CLOSED 0

void CS_init(void);

uint8_t CS_get_door_status(void);

#endif /* CONTACT_SWITCH_H_ */
