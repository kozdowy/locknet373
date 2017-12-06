/*
 * contact_switch.c
 *
 *  Created on: Nov 16, 2017
 *      Author: peces
 */
#include "contact_switch.h"

void CS_init(void){
	MSS_GPIO_init();
	MSS_GPIO_config( MSS_GPIO_1, MSS_GPIO_INPUT_MODE);
}

uint8_t CS_get_door_status(void){
	uint32_t inputs = MSS_GPIO_get_inputs();
	return (uint8_t) (inputs & MSS_GPIO_1_MASK) >> 1;
}
