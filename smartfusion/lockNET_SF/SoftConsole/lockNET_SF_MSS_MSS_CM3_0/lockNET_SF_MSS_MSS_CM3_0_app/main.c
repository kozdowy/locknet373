#include <stdio.h>
#include <inttypes.h>
#include "drivers/mss_i2c/mss_i2c.h"
#include <drivers/mss_gpio/mss_gpio.h>
#include <drivers/mss_uart/mss_uart.h>
#include "lock.h"
#include "neopixel.h"
#include "servo.h"
#include "nfc.h"
#include "contact_switch.h"
#include "lora.h"
#include "lora_client_ex.h"
#include "rsa.h"
#include "unit_tests.h"
#include "keys.h"
//#include "lora_server_ex.h"

// CHANGE FOR OTHER LOCK


#define ACCESS_GRANTED 0xA6
#define ACCESS_DENIED 0xAD

const uint32_t RSA_PUB[2] = {0, RSA_PUBLIC_KEY};
const uint32_t RSA_MOD[2] = {RSA_MODULUS_U, RSA_MODULUS_L};
const uint32_t RSA_PRI[2] = {RSA_PRIVATE_KEY_U, RSA_PRIVATE_KEY_L};
const uint32_t RSA_RES[2] = {RSA_RESIDUE_U, RSA_RESIDUE_L};
const uint32_t RSA_SERVER_MOD[2] = {RSA_SERVER_MODULUS_U, RSA_SERVER_MODULUS_L};
const uint32_t RSA_SERVER_RES[2] = {RSA_SERVER_RESIDUE_U, RSA_SERVER_RESIDUE_L};

//uint8_t RSA_result_ready = 0;
int LOCK_timer = 0;

/*
__attribute__ ((interrupt)) void Fabric_IRQHandler( void ){
	RSA_result_ready = 1;
	NVIC_ClearPendingIRQ( Fabric_IRQn );
}
*/

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

int main(void);
void init_modules(void);
void state_machine_door(void);
void TEST_state_machine(void);
void handle_lora_response(uint8_t* lora_buf);
void verify_nfc_list(uint8_t* nfc_response);
void unlock_door(void);
uint8_t lock_door(void);

// Main program
int main(void)
{
	MSS_GPIO_init();
	/*
	if (LORA_init() == 1){
		  printf("init failed\r\n");
		  return;
	  }

	  while (1){
		  LORA_client_ex_loop();
	  }
	int i;
	*/
	//LORA_client_ex_setup();
	init_modules();
	/*
	MSS_GPIO_init();
	for(i = 0; i < 10000000; ++i);
	RSA_init();
	for(i = 0; i < 10000000; ++i);
	NP_init();
	for(i = 0; i < 10000000; ++i);

	for(i = 0; i < 10000000; ++i);
	LORA_init();
	CS_init();
	SERVO_init();
	for(i = 0; i < 10000000; ++i);
	nfc_init();

	for(i = 0; i < 10000000; ++i);
	*/
	//TEST_nfc();
	//nfc_InListPassiveTarget();
	//while(nfc_is_interrupt_handled());
	//nfc_read_response();
	//while(nfc_is_interrupt_handled());
	//nfc_read_response();
	//while(1){LORA_client_ex_loop();}
	TEST_state_machine();
	return(0);
}

void init_modules(void){
		MSS_GPIO_init();
		RSA_init();
		NP_init();
		LORA_init();
		CS_init();
		SERVO_init();
		nfc_init();
}

void TEST_state_machine(void){
	//uint8_t NFC_BLUE_ID[4] = {0x9B, 0x46, 0x0E, 0x89};
	//uint8_t NFC_CARD_ID[4] = {0xA8, 0x56, 0x10, 0x00};
	nfc_InListPassiveTarget();
  uint8_t lora_buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t lora_len = sizeof(lora_buf);
  uint32_t gpio9 = (MSS_GPIO_get_inputs() & MSS_GPIO_9_MASK) >> 9;

	while(1){
		gpio9 = (MSS_GPIO_get_inputs() & MSS_GPIO_9_MASK) >> 9;
		if (CS_get_door_status() == CS_DOOR_OPEN){
			NP_set_pixel(NP_COLOUR_YELLOW);
			NP_apply();
		} else{

      if (LORA_wait_available_timeout(1000)){
        if (LORA_recv(lora_buf, &lora_len)){
          handle_lora_response(lora_buf);
        }
      }

			if(!nfc_is_interrupt_handled()){
				nfc_read_response();
				if(!nfc_last_was_ack()){
					uint8_t* nfc_response = nfc_get_response_buffer();
					verify_nfc_list(nfc_response);
					nfc_InListPassiveTarget();
				}
			}
			if(CS_get_door_status() == CS_DOOR_CLOSED && !LOCK_timer){
				SERVO_lock();
				NP_set_pixel(NP_COLOUR_BLUE);
				NP_apply();
			}
		}
    if (LOCK_timer > 0){
      LOCK_timer--;
    }
	}
}

void handle_lora_response(uint8_t* lora_buf){
  uint32_t resp_msg[2];
  uint32_t decr_msg[2];
  resp_msg[0] = (lora_buf[0] << 24) | (lora_buf[1] << 16) |
    (lora_buf[2] << 8) | (lora_buf[3]);
  resp_msg[1] = (lora_buf[4] << 24) | (lora_buf[5] << 16) |
    (lora_buf[6] << 8) | (lora_buf[7]);

  RSA_run(resp_msg, RSA_PRI, RSA_MOD, RSA_RES);
  while (!RSA_is_ready());
  RSA_read_result(decr_msg);

  uint8_t access = decr_msg[1] & 0xFF;
  if (access == ACCESS_GRANTED){
    unlock_door();
  } else if (access == ACCESS_DENIED){
    lock_door();
  } else {
	  NP_set_pixel_c(0xFF, 0xFF, 0xFF);
  }
}

void unlock_door(void){
  SERVO_unlock();
  NP_set_pixel(NP_COLOUR_GREEN);
  NP_apply();
  LOCK_timer = 10;
}

uint8_t lock_door(void){
  uint8_t resp = 0;
  if(CS_get_door_status() == CS_DOOR_CLOSED){
    resp = 1;
    SERVO_lock();
  }
  NP_set_pixel(NP_COLOUR_RED);
  NP_apply();
  LOCK_timer = 10;
  return resp;
}

void verify_nfc_list(uint8_t* nfc_response){
	int i;
  uint8_t nfc_id[4];
  for (i = 17; i >= 14; --i){
    nfc_id[i - 14] = nfc_response[i];
  }
  uint32_t nfc_msg[2];
  uint32_t enc_msg[2];
  nfc_msg[0] = 0;
  nfc_msg[1] = (nfc_id[0] << 24) | (nfc_id[1] << 16) |
    (nfc_id[2] << 8) | (nfc_id[3]);
  RSA_run(nfc_msg, RSA_PUB, RSA_SERVER_MOD, RSA_SERVER_RES);
  while (!RSA_is_ready());
  RSA_read_result(enc_msg);
  uint8_t enc_msg_b[8];
  for (i = 0; i < 8; ++i){
    enc_msg_b[i] = (enc_msg[i/4] >> ( (3 - (i % 4)) * 8)) & 0xFF;
  }
  LORA_send(enc_msg_b, sizeof(enc_msg_b));
  LORA_wait_packet_sent(0);
}
