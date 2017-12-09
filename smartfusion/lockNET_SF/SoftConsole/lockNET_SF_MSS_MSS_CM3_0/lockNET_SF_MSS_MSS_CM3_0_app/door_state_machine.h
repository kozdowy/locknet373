/*
 * door_state_machine.h
 *
 *  Created on: Dec 8, 2017
 *      Author: eliubakk
 */

#ifndef DOOR_STATE_MACHINE_H_
#define DOOR_STATE_MACHINE_H_
/*
* State Machine for the door
*/
#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>
#include <drivers/mss_i2c/mss_i2c.h>
#include <drivers/mss_gpio/mss_gpio.h>
#include <drivers/mss_uart/mss_uart.h>
#include "neopixel.h"
#include "servo.h"
#include "nfc.h"
#include "contact_switch.h"
#include "lora.h"
#include "lora_client_ex.h"
#include "rsa.h"

// State codifications
#define IDLE 						0x00
#define CHECK_LIST 					0x01
#define CONTACT_USERS_FOR_APPROVAL	0x02
#define UNLOCK 						0x03
#define BLOCK 						0x04
#define LOCK 						0x05
#define CONTACT_USERS_FOR_UPDATE 	0x06
#define UPDATE_LISTS				0x07

// TODO: Add also the fingerprint in the state machine
//		 MMIO reset
// 		 How proceed when the door is blocked?
// 		 Might be convinient to add a timeout to return to idle
//		 Lists of ids stored in a global

int get_rf (void);

int get_request_update (void);

void check_list(uint8_t *is_whitelist, uint8_t *non_list, uint32_t *id);

uint32_t get_nfcID(void);

int get_users_response_for_approval(void);

int get_decision(void);

void register_request(void);

uint8_t get_users_response_for_update(void);

uint8_t get_approval(void);

void update_lists(void);

void state_machine_door(void);

#endif /* DOOR_STATE_MACHINE_H_ */
