/*
* State Machine for the door
*/
#include <stdint.h>
// State codifications
#define IDLE 						0x00;
#define CHECK_LIST 					0x01;
#define CONTACT_USERS_FOR_APPROVAL	0x02;
#define UNLOCK 						0x03;
#define BLOCK 						0x04;
#define LOCK 						0x05;
#define CONTACT_USERS_FOR_UPDATE 	0x06;
#define UPDATE_LISTS				0x07;

// TODO: Add also the fingerprint in the state machine
//		 MMIO reset
// 		 How proceed when the door is blocked? 
// 		 Might be convinient to add a timeout to return to idle
//		 Lists of ids stored in a global

volatile uint32_t *RESET = (uint32_t*) 0x40050004;

/* TODO functions
* int get_rf (void);
* int get_request_update (void);
* void check_list(int *is_whitelist, int *non_list, uint32_t *id);
* uint32_t get_nfcID(void);
* int get_users_response_for_approval(void);
* int get_decision(void);
* void check_door_is_closed; // waits until the door is closed again
* void unlock_door(void);
* void lock_door();
* void register_request(void);
* int get_users_response_for_update(void);
* int get_approval(void);
*
*
*
*
*/
void state_machine_door(void);


//void state_machine_door(void){
//
//	// Variable declarations
//	uint8_t state, next_state;
//	int rf;						// 1 if nfc detects a field, 0 if there is nothing detected
//	int update; 				// 1 if an w/b list is requested, 0 ow
//	int non_list;				// 1 if the NFC ID is not in the lists, 0 ow
//	int is_whitelist; 			// 1 if the ID is in the whitelist, 0 if it is in the blacklist
//	int access;					// 1 when the owner(s) give approval
//	uint32_t id;				// NFC ID
//	int response_update;		// 1 if users agreed to update the lists, 0 ow
//	int response_approval;		// 1 if users agreed to open door remotely, 0 ow
//
//	// Initializations
//	state =  IDLE;
//	next_state =  IDLE;
//	rf = get_rf();
//
//	while(1){
//
//		// Get next state
//		switch (state){
//			case IDLE:
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
//				if(non_list == 1){
//					next_state = CONTACT_USERS_FOR_APPROVAL;
//				}else{
//					if(whitelist == 1){
//						next_state = UNLOCK;
//					}else{
//						next_state = BLOCK;
//					}
//				}
//				break;
//
//			case CONTACT_USERS_FOR_APPROVAL:
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
//				next_state = LOCK;
//				break;
//
//			case LOCK:
//				next_state = IDLE;
//				break;
//
//			case CONTACT_USERS_FOR_UPDATE:
//				if(response_update == 1){
//					if(approval == 0)
//						next_state = UPDATE_LISTS;
//					else
//						next_state = IDLE;
//				}
//				// Might be essential to add a timeout
//				break;
//
//			case UPDATE_LISTS:
//				next_state = IDLE;
//				break;
//
//			case default:
//				next_state = IDLE;
//				break;
//
//		}
//
//		// State update
//		if(*RESET == 1){
//			state = IDLE;
//		}else{
//			state = next_state;
//		}
//
//		// Actions according to the state
//		switch (state){
//			case IDLE:
//				rf = get_rf();
//				break;
//
//			case CHECK_LIST:
//				id = get_nfcID();
//				checklist(&is_whitelist, &non_list, &id);
//				break;
//
//			case CONTACT_USERS_FOR_APPROVAL:
//				response_approval = get_users_response_for_approval();
//				access = get_decision();
//				break;
//
//			case BLOCK:
//
//				break;
//
//			case UNLOCK:
//				unlock_door();
//				check_door_is_closed();
//				break;
//
//			case LOCK:
//				lock_door();
//				break;
//
//			case CONTACT_USERS_FOR_UPDATE:
//				response_update = get_users_response_for_update();
//				approval = get_approval();
//				break;
//
//			case UPDATE_LISTS:
//				update_lists();
//				break;
//
//			case default:
//				next_state = IDLE;
//				break;
//
//
//		}
//
//	}
//
//
//}

















