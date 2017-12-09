#include <stdio.h>
#include <inttypes.h>
#include "drivers/mss_i2c/mss_i2c.h"
#include <drivers/mss_gpio/mss_gpio.h>
#include <drivers/mss_uart/mss_uart.h>
#include "neopixel.h"
#include "servo.h"
#include "nfc.h"
#include "contact_switch.h"
#include "lora.h"
#include "lora_client_ex.h"
#include "rsa.h"
#include "unit_tests.h"
//#include "lora_server_ex.h"

uint8_t RSA_result_ready = 0;

__attribute__ ((interrupt)) void Fabric_IRQHandler( void ){
	RSA_result_ready = 1;
	NVIC_ClearPendingIRQ( Fabric_IRQn );
}

__attribute__ ((interrupt)) void GPIO9_IRQHandler( void ){
	MSS_GPIO_clear_irq(MSS_GPIO_9);
	LORA_handle_interrupt();
}

// Interrupts Handler
__attribute__ ((interrupt)) void GPIO1_IRQHandler( void )
{
	MSS_GPIO_clear_irq(MSS_GPIO_1);
	nfc_set_interrupt_handled(0);
}

void init_modules(void);
void state_machine_door(void);
void TEST_state_machine(void);

// Main program
int main()
{
	init_modules();
	int i;
	for(i = 0; i < 1000000; ++i);
	//TEST_nfc();
	//nfc_InListPassiveTarget();
	//while(nfc_is_interrupt_handled());
	//nfc_read_response();
	//while(nfc_is_interrupt_handled());
	//nfc_read_response();
	TEST_state_machine();
	return(0);
}

void init_modules(void){
	MSS_GPIO_init();
	RSA_init();
	NP_init();
	CS_init();
	SERVO_init();
	nfc_init();
}

void TEST_state_machine(void){
	uint8_t NFC_BLUE_ID[4] = {0x9B, 0x46, 0x0E, 0x89};
	uint8_t NFC_CARD_ID[4] = {0xA8, 0x56, 0x10, 0x00};
	nfc_InListPassiveTarget();

	while(1){
		if (CS_get_door_status() == CS_DOOR_OPEN){
			NP_set_pixel(NP_COLOUR_YELLOW);
			NP_apply();
			//SERVO_unlock();
		} else{
			if(!nfc_is_interrupt_handled()){
				nfc_read_response();
				if(!nfc_last_was_ack()){
					int i = 0;
					int correct_nfc = 1;
					for(i = 17; i > 13 && correct_nfc == 1; --i){
						if(nfc_get_response_buffer()[i] != NFC_BLUE_ID[i - 14]){
							correct_nfc = 0;
						}
					}
					if(correct_nfc == 1){
						SERVO_unlock();
						NP_set_pixel(NP_COLOUR_GREEN);
						NP_apply();
						for(i = 0; i < 10000000; ++i);
					}else{
						if(CS_get_door_status() == CS_DOOR_CLOSED){
							SERVO_lock();
						}
						NP_set_pixel(NP_COLOUR_RED);
						NP_apply();
						for(i = 0; i < 10000000; ++i);
					}
					nfc_InListPassiveTarget();
				}
			}
			if(CS_get_door_status() == CS_DOOR_CLOSED){
				SERVO_lock();
				NP_set_pixel(NP_COLOUR_BLUE);
				NP_apply();
			}
		}
	}
}

//void state_machine_door(void){
//
//	// Variable declarations
//	uint8_t state, next_state;
//	uint8_t rf;						// 1 if nfc detects a field, 0 if there is nothing detected
//	uint8_t update; 				// 1 if an w/b list is requested, 0 ow
//	uint8_t non_list;				// 1 if the NFC ID is not in the lists, 0 ow
//	uint8_t is_whitelist; 			// 1 if the ID is in the whitelist, 0 if it is in the blacklist
//	uint8_t access;					// 1 when the owner(s) give approval
//	uint32_t id;					// NFC ID
//	uint8_t response_update;		// 1 if users agreed to update the lists, 0 ow
//	uint8_t response_approval;		// 1 if users agreed to open door remotely, 0 ow
//
//	// Initializations
//	state =  IDLE;
//	next_state =  IDLE;
//	rf = get_rf();
//
//	while(1){
//		// Get next state
//		switch (state){
//			case IDLE :
//				rf = get_rf();
//				if(rf == 1){
//					next_state = CHECK_LIST;
//				}else{
//					update = get_request_update();
//					if(update == 1){
//						next_state = CONTACT_USERS_FOR_UPDATE;
//					}
//				}
//				break;
//
//
//			case CHECK_LIST:
//				id = get_nfcID();
//				check_list(&is_whitelist, &non_list, &id);
//				if(non_list == 1){
//					next_state = CONTACT_USERS_FOR_APPROVAL;
//				}else{
//					if(is_whitelist == 1){
//						next_state = UNLOCK;
//					}else{
//						next_state = BLOCK;
//					}
//				}
//				break;
//
//			case CONTACT_USERS_FOR_APPROVAL:
//				response_approval = get_users_response_for_approval();
//				access = get_decision();
//				if(response_approval == 1){
//					if(access == 0)
//						next_state = BLOCK;
//					else
//						next_state = UNLOCK;
//				}
//				// Might be essential to add a timeout
//				break;
//
//			case BLOCK:
//
//				break;
//
//			case UNLOCK:
//				SERVO_unlock();
//				next_state = LOCK;
//				break;
//
//			case LOCK:
//				if (CS_get_door_status() == DOOR_CLOSED){
//					SERVO_lock();
//				}
//				next_state = IDLE;
//				break;
//
//			case CONTACT_USERS_FOR_UPDATE:
//				response_update = get_users_response_for_update();
//				response_approval = get_approval();
//				if(response_update == 1){
//					if(response_approval == 0)
//						next_state = UPDATE_LISTS;
//					else
//						next_state = IDLE;
//				}
//				// Might be essential to add a timeout
//				break;
//
//			case UPDATE_LISTS:
//				update_lists();
//				next_state = IDLE;
//				break;
//
//			default:
//				next_state = IDLE;
//				break;
//
//		}
//
//		/*
//		// State update
//		if(*RESET == 1){
//			state = IDLE;
//		}else{
//			state = next_state;
//		}
//		*/
//		state = next_state;
//	}
//
//
//}
