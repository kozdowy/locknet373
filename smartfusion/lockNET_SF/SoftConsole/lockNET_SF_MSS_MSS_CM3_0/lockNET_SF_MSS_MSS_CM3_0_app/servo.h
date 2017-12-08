/*
 * servo.h
 *
 *  Created on: Nov 14, 2017
 *      Author: kozdowy
 */

#ifndef SERVO_H_
#define SERVO_H_

#include <stdint.h>

#define SERVO_ADDR 0x40050004
#define SERVO_PERIOD 500000
#define SERVO_OPEN 22500
#define SERVO_CLOSED 37500

#define SERVO ((uint32_t *) SERVO_ADDR)

void SERVO_init(void);

void SERVO_close(void);

void SERVO_open(void);

uint32_t SERVO_read_state(void);

#endif /* SERVO_H_ */
