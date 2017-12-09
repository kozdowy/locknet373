/*
 * rsa.c
 *
 *  Created on: Dec 7, 2017
 *      Author: eliubakk
 */
#include "rsa.h"

void RSA_init(void){
	*RSA_BEGIN_ENCRYPT = 0x0;
}

void RSA_run(uint32_t* message, uint32_t *key, uint32_t *modulus, uint32_t *residue){
	*RSA_BEGIN_ENCRYPT = 0x0;

	*RSA_BIT_SWITCH = LOAD_UPPER;
	*RSA_MESSAGE = message[0];
	*RSA_EXPONENT = key[0];
	*RSA_MODULUS = modulus[0];
	*RSA_RESIDUE = residue[0];
	*RSA_BIT_SWITCH = LOAD_LOWER;
	*RSA_MESSAGE = message[1];
	*RSA_EXPONENT = key[1];
	*RSA_MODULUS = modulus[1];
	*RSA_RESIDUE = residue[1];

	*RSA_BEGIN_ENCRYPT = 0x1;

}

uint8_t RSA_read_result(uint32_t result[2]){
	uint8_t valid = *RSA_RESULT_VALID;
	if (!valid){
		return 1;
	}
	*RSA_BIT_SWITCH = LOAD_LOWER;
	result[1] = *RSA_RESULT;
	*RSA_BIT_SWITCH = LOAD_UPPER;
	result[0] = *RSA_RESULT;
	*RSA_BEGIN_ENCRYPT = 0x0;
	return 0;
}

uint8_t RSA_is_ready(void){
	return *RSA_RESULT_VALID;
}
