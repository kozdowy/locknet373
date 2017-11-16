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

void SERVO_close(void){
	(*SERVO) = (uint32_t) SERVO_CLOSED;
}

void SERVO_open(void){
	(*SERVO) = (uint32_t) SERVO_OPEN;
}

uint32_t SERVO_read_state(void){
	return (uint32_t) *SERVO;
}
