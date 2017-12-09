/*
 * rsa.h
 *
 *  Created on: Dec 7, 2017
 *      Author: kozdowy
 */

#ifndef RSA_H_
#define RSA_H_

#include <stdint.h>

#define RSA_BIT_SWITCH_ADDR 0x40050008
#define RSA_MESSAGE_ADDR 0x4005000C
#define RSA_MODULUS_ADDR 0x40050010
#define RSA_EXPONENT_ADDR 0x40050014
#define RSA_RESIDUE_ADDR 0x40050018
#define RSA_BEGIN_ENCRYPT_ADDR 0x4005001C
#define RSA_RESULT_ADDR 0x40050020
#define RSA_RESULT_VALID_ADDR 0x40050024

#define LOAD_UPPER 0x1
#define LOAD_LOWER 0x0

#define RSA_BIT_SWITCH ((uint32_t *) RSA_BIT_SWITCH_ADDR)
#define RSA_MESSAGE ((uint32_t *) RSA_MESSAGE_ADDR)
#define RSA_MODULUS ((uint32_t *) RSA_MODULUS_ADDR)
#define RSA_EXPONENT ((uint32_t *) RSA_EXPONENT_ADDR)
#define RSA_RESIDUE ((uint32_t *) RSA_RESIDUE_ADDR)
#define RSA_BEGIN_ENCRYPT ((uint32_t *) RSA_BEGIN_ENCRYPT_ADDR)
#define RSA_RESULT ((uint32_t *) RSA_RESULT_ADDR)
#define RSA_RESULT_VALID ((uint32_t *) RSA_RESULT_VALID_ADDR)

void RSA_init(void);

void RSA_run(uint32_t* message, uint32_t *key, uint32_t *modulus, uint32_t *residue);

uint8_t RSA_read_result(uint32_t *result);

uint8_t RSA_is_ready(void);
#endif /* RSA_H_ */
