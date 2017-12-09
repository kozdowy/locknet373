/*
 * unit_tests.c
 *
 *  Created on: Dec 8, 2017
 *      Author: eliubakk
 */
#include "neopixel.h"
#include "servo.h"
#include "nfc.h"
#include "contact_switch.h"
#include "lora.h"
#include "lora_client_ex.h"
#include "rsa.h"
#include "unit_tests.h"
#include <assert.h>

//rsa testing
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

void TEST_rsa(void){
	RSA_init();
	uint32_t message[] = {MSG_ENC_U, MSG_ENC_L};
	uint32_t key[] = {PRI_EXP_U, PRI_EXP_L};
	uint32_t modulus[] = {MODULUS_U, MODULUS_L};
	uint32_t residue[] = {RESIDUE_U, RESIDUE_L};
	RSA_run(message, key, modulus, residue);
	while (!RSA_is_ready());
	uint32_t result[2];
	RSA_read_result(result);
	assert(result[0] == MESSAGE_U);
	assert(result[1] == MESSAGE_L);
}

void TEST_np_cs_np(void){
	NP_init();
	CS_init();
	uint32_t servo_state = 1;
	while (1){
		servo_state = SERVO_read_state();
		if (CS_get_door_status() == CS_DOOR_OPEN){
			NP_set_pixel_c(0x55, 0x00, 0x00);
			NP_apply();
			SERVO_unlock();
		} else{
			NP_set_pixel(NP_COLOUR_GREEN);
			NP_apply();
			SERVO_lock();
		}
	}
}

void TEST_nfc(void){
	nfc_init();
	int i = 0;
	for(i = 0; i < 1000000; ++i);
	//uint8_t receive_buf[20];
	nfc_GetFirmwareVersion();
	while(nfc_is_interrupt_handled());
	nfc_read_response();
	while(nfc_is_interrupt_handled());
	nfc_read_response();

	uint8_t correct_response[14] = {0x01, 0x00, 0x00, 0xFF, 0x06, 0xFA, 0xD5, 0x03, 0x32, 0x01, 0x06, 0x07, 0xE8, 0x00};

	for(i = 0; i < nfc_get_response_size(); ++i){
		assert(nfc_get_response_buffer()[i] == correct_response[i]);
	}
}

void TEST_lora(void){
	int init_res = LORA_init();
	uint8_t send_buf[] = {0x01, 0x03, 0x05, 0x07};
	LORA_write(RH_RF95_REG_0D_FIFO_ADDR_PTR, 0);
	LORA_burst_write(RH_RF95_REG_00_FIFO, send_buf, 4);
	int i;
	for (i = 0; i < 100000; ++i);
	LORA_write(RH_RF95_REG_0D_FIFO_ADDR_PTR, 0);
	uint8_t read_buf[4];
	LORA_burst_read(RH_RF95_REG_00_FIFO, read_buf, 4);
	LORA_client_ex_setup();
	//LORA_read_addr(LORA_RegFifoTxBaseAddr);
}
