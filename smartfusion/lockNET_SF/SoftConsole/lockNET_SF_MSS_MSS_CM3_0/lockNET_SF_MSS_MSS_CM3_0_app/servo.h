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
#define SERVO_UNLOCKED 23000
#define SERVO_LOCKED 45500 // 37500

#define SERVO ((uint32_t *) SERVO_ADDR)

void SERVO_init(void);

void SERVO_lock(void);

void SERVO_unlock(void);

uint32_t SERVO_read_state(void);

#endif /* SERVO_H_ */
