/*
 * servo.c
 *
 *  Created on: Nov 14, 2017
 *      Author: kozdowy
 */
#include "servo.h"

void SERVO_init(void){
	// intentionally blank
}

void SERVO_lock(void){
	(*SERVO) = (uint32_t) SERVO_LOCKED;
}

void SERVO_unlock(void){
	(*SERVO) = (uint32_t) SERVO_UNLOCKED;
}

uint32_t SERVO_read_state(void){
	return (uint32_t) *SERVO;
}
