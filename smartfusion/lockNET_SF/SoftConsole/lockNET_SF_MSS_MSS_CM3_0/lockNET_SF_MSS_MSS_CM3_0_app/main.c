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
//#include "lora_client_ex.h"
#include "rsa.h"
#include "unit_tests.h"
//#include "lora_server_ex.h"

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
void handle_access_control(uint8_t* lora_buf);
uint8_t verify_nfc_list(uint8_t* nfc_response);
void unlock_door(void);
uint8_t lock_door(void);

// Main program
int main(void)
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
  LORA_init();
	RSA_init();
	NP_init();
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

	while(1){
		if (CS_get_door_status() == CS_DOOR_OPEN){
			NP_set_pixel(NP_COLOUR_YELLOW);
			NP_apply();
		} else{
      if (LORA_is_available()){
        if (LORA_recv(lora_buf, &lora_len)){
          handle_lora_response(lora_buf);
        }
      }
			if(!nfc_is_interrupt_handled()){
				nfc_read_response();
				if(!nfc_last_was_ack()){
					int i = 0;
          uint8_t* nfc_response = nfc_get_response_buffer();
          verify_nfc_list(nfc_id);
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

void handle_access_control(uint8_t* lora_buf){
  uint32_t resp_msg[2];
  uint32_t decr_msg[2];
  resp_msg[0] = (lora_buf[0] << 24) | (lora_buf[1] << 16) |
    (lora_buf[2] << 8) | (lora_buf[3]);
  resp_msg[1] = (lora_buf[4] << 24) | (lora_buf[5] << 16) |
    (lora_buf[6] << 8) | (lora_buf[7]);

  RSA_run(resp_msg, RSA_LOCK_PRIV, RSA_MODULUS, RSA_RESIDUE);
  while (!RSA_is_ready());
  RSA_read_result(decr_msg);

  uint8_t access = decr_msg[1] && 0xFF;
  if (access == ACCESS_GRANTED){
    unlock_door();
  } else if (access == ACCESS_DENIED){
    lock_door();
  }
}

void unlock_door(void){
  SERVO_unlock();
  NP_set_pixel(NP_COLOUR_GREEN);
  NP_apply();
  LOCK_timer = 10000000;
}

uint8_t lock_door(void){
  uint8_t resp = 0;
  if(CS_get_door_status() == CS_DOOR_CLOSED){
    resp = 1;
    SERVO_lock();
  }
  NP_set_pixel(NP_COLOUR_RED);
  NP_apply();
  LOCK_timer = 10000000;
  return resp;
}

void verify_nfc_list(uint8_t* nfc_response){
  uint8_t nfc_id[4];
  for (i = 17; i >= 14; --i){
    nfc_id[i - 14] = nfc_response[i];
  }
  uint32_t nfc_msg[2];
  uint32_t enc_msg[2];
  nfc_msg[0] = 0;
  nfc_msg[1] = (nfc_id[0] << 24) | (nfc_id[1] << 16) |
    (nfc_id[2] << 8) | (nfc_id[3]);
  RSA_run(enc_msg, RSA_GATEWAY_PUB, RSA_MODULUS, RSA_RESIDUE);
  while (!RSA_is_ready());
  RSA_read_result(enc_msg);
  uint8_t enc_msg_b[8];
  int i;
  for (i = 0; i < 8; ++i){
    enc_msg_b[i] = (nfc_enc[i/4] >> ( (3 - (i % 4)) * 8)) & 0xFF;
  }
  LORA_send(enc_msg_b, sizeof(enc_msg_b));
  LORA_wait_packet_sent(0);
}
