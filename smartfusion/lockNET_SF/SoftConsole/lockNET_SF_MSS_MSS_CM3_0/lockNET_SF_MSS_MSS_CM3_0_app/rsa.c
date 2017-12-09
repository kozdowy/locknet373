/*
 * rsa.c
 *
 *  Created on: Dec 7, 2017
 *      Author: eliubakk
 */
#include "rsa.h"
// FOR TESTING DELETE LATER

#define MESSAGE_U 0xDCBAABCD
#define MESSAGE_L 0xABCDDCBA
#define MSG_ENC_U 0x0f886e09
#define MSG_ENC_L 0x5e89cf95
#define PUB_EXP_U 0x00000000
#define PUB_EXP_L 0x00010001
#define PRI_EXP_U 0x044de026
#define PRI_EXP_L 0xcabdb311
#define MODULUS_U 0xeda515ef
#define MODULUS_L 0x24029417
#define RESIDUE_U 0x859cfcfb
#define RESIDUE_L 0x5a1f75d5

void RSA_test(void){
	//uint32_t message[] = {MESSAGE_U, MESSAGE_L};
	//uint32_t key[] = {PUB_EXP_U, PUB_EXP_L};
	uint32_t message[] = {MSG_ENC_U, MSG_ENC_L};
	uint32_t key[] = {PRI_EXP_U, PRI_EXP_L};
	uint32_t modulus[] = {MODULUS_U, MODULUS_L};
	uint32_t residue[] = {RESIDUE_U, RESIDUE_L};
	RSA_run(message, key, modulus, residue);
}

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
