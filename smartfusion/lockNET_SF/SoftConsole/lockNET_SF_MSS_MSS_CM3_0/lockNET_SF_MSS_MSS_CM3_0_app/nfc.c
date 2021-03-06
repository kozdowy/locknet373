#include "nfc.h"
#include "drivers/mss_i2c/mss_i2c.h"
#include <drivers/mss_gpio/mss_gpio.h>
#include <unistd.h>

//ack and nakframes
const uint8_t PN532_ACK_FRAME[] = {0x01, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00};
const uint8_t PN532_NACK_FRAME[] = {0x01, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00};
const uint8_t PN532_TURN_ON_RF_INFO[]= {0x01, 0x01};
const uint8_t PN532_POWERDOWN_INFO[] = {0x88, 0x01};
const uint8_t PN532_INLISTPASSIVETARGET_INFO[] = {0x01, 0x00};
const uint8_t PN532_SAMCONFIG_INFO[] = {0x01, 0x14, 0x01};

uint8_t last_command_sent;

uint8_t NFC_ACK_BUF[7];
const uint8_t NFC_ACK_LENGTH = 7;
uint8_t NFC_RESPONSE_BUFFER[100];
const uint8_t NFC_RESPONSE_BUFFER_LENGTH = 100;


volatile uint8_t interrupt_handled = 1;
uint8_t last_was_ack = 0;

/*
 * nfc.c
 *
 *  Created on: Nov 17, 2017
 *      Author: peces
 */


/* Function for setting up the NFC module
 * Description:
 * Receives: 	Nothing
 * Returns:		Nothing
 */
void nfc_init(void){
	MSS_I2C_init(&g_mss_i2c1 , PN532_I2C_ADDRESS, MSS_I2C_PCLK_DIV_960 );
	MSS_GPIO_config( MSS_GPIO_0, MSS_GPIO_OUTPUT_MODE);
	MSS_GPIO_config( MSS_GPIO_1, MSS_GPIO_INPUT_MODE | MSS_GPIO_IRQ_EDGE_NEGATIVE );
	MSS_GPIO_enable_irq(MSS_GPIO_1);
	int i;

	// Delays required for set up
	MSS_GPIO_set_output(MSS_GPIO_0 ,(uint8_t) 1);
	for(i=0;i<100000;i++);
	MSS_GPIO_set_output(MSS_GPIO_0 ,(uint8_t) 0);
	for(i=0;i<3000000;i++); // We need 400 milisecs (we get 418 msec => OK)
	MSS_GPIO_set_output(MSS_GPIO_0 ,(uint8_t)  1);
	for(i=0;i<100000;i++); //We need 10 msecs (we get 18 msec => OK)
	/* times tested with oscilloscope */
	for(i = 0; i < NFC_RESPONSE_BUFFER_LENGTH; ++i){
		NFC_RESPONSE_BUFFER[i] = 0;
	}
	nfc_SAMConfig();
	while(interrupt_handled);
	nfc_read_response();
	while(interrupt_handled);
	nfc_read_response();
	return;
}



/* NFC read function using I2C
 * Description:
 * Receives: buff -> Buffer (address) where the written data will be stored
 * 			 n	  -> Number of bytes to read
 * 			 read_ack -> if (read_ack == 1) the ack frame will be read in before the information frame.
 * Returns:	 0 if read is successful, 1 if read is not successful
 */

uint8_t nfc_read_ack(){
	MSS_I2C_read
		(
				&g_mss_i2c1,
				PN532_I2C_ADDRESS,
				NFC_ACK_BUF,
				NFC_ACK_LENGTH,
				MSS_I2C_RELEASE_BUS
		);
	MSS_I2C_wait_complete(&g_mss_i2c1, MSS_I2C_NO_TIMEOUT);
	int i = 0;
	int isAck = 1;
	int isNack = 1;
	for(i = 0; i < NFC_ACK_LENGTH; ++i){
		if (NFC_ACK_BUF[i] != PN532_ACK_FRAME[i])
			isAck = 0;
		if (NFC_ACK_BUF[i] != PN532_NACK_FRAME[i])
			isNack = 0;
	}
	if(!isAck || isNack)
		return 0;
	return 1;
}

uint8_t nfc_read_response(){
	nfc_set_interrupt_handled(1);
	if(last_was_ack){
		int i = 0;
		for(i = 0; i < NFC_RESPONSE_BUFFER_LENGTH; ++i){
			NFC_RESPONSE_BUFFER[i] = 0;
		}
		int count = 0;
		while(NFC_RESPONSE_BUFFER[0] != 0x01 && count < 10){
			MSS_I2C_read
				(
						&g_mss_i2c1,
						PN532_I2C_ADDRESS,
						NFC_RESPONSE_BUFFER,
						nfc_get_response_size(),
						MSS_I2C_RELEASE_BUS
				);
			MSS_I2C_wait_complete(&g_mss_i2c1, MSS_I2C_NO_TIMEOUT);
			++count;
		}
		last_was_ack = 0;
		return 0;
	} else {
		if(nfc_read_ack()){
			last_was_ack = 1;
			return 1;
		}else{
			return 0;
		}
	}
}

uint8_t *nfc_get_response_buffer(){
	return NFC_RESPONSE_BUFFER;
}


/*
 * Description:
 * Receives:
 * Returns:	 Nothing
 */
void nfc_send_command(const uint8_t command, const uint8_t info_buf[], const uint8_t len){
	uint16_t length = 0;
	uint8_t buffer[20];
	buffer[length++] = (uint8_t) PN532_PREAMBLE; // Preamble
	buffer[length++] = 0x00; // Start bits
	buffer[length++] = 0xFF; // Start bits
	buffer[length++] = (len + 2); // Length of (n_data_bytes + TFI byte + command)
	buffer[length++] = (uint8_t)(0x100 - (len + 2)); // LCS
	buffer[length++] = (uint8_t) PN532_HOST_TO_NFC; // Write
	uint8_t dcs = (uint8_t)PN532_HOST_TO_NFC + command;
	buffer[length++] = command;
	int i = 0;
	for(i = 0; i < len; ++i){
		dcs += info_buf[i];
		buffer[length++] = info_buf[i];
	}
	buffer[length++] = (uint8_t)(0x100 - dcs); // DCS
	buffer[length++] = PN532_POSTAMBLE; // Postamble

	MSS_I2C_write
		(
				&g_mss_i2c1,
				PN532_I2C_ADDRESS,
				buffer,
				length,
				MSS_I2C_RELEASE_BUS
		);
	MSS_I2C_wait_complete(&g_mss_i2c1, MSS_I2C_NO_TIMEOUT);
	return;
}


/*
 * Description: Function to get all the parameters of the status of the NFC
 * Receives:	Array LENGTH 9(+8) where the info will be stored
 * Returns:	 field (if external RF is present=1, 0 otherwise)
 */
uint8_t nfc_GetGeneralStatus(){
	uint8_t command[] = {};
	nfc_send_command(PN532_COMMAND_GETSTATUS, command, 0);
	last_command_sent = PN532_COMMAND_GETSTATUS;
	return 1;
}

uint8_t nfc_InListPassiveTarget(){

	//send D4 4A 01 00
	//receive D5 4B <num targets> <tg (1) sens_res (2) sel_res (1) nfcidlength (1) nfcid (4?) atslength (1) ats (y)>

	nfc_send_command(PN532_COMMAND_INLISTPASSIVETARGET,
					PN532_INLISTPASSIVETARGET_INFO,
					2);
	last_command_sent = PN532_COMMAND_INLISTPASSIVETARGET;
	return 1; //response_buf[6];
}

uint8_t nfc_SAMConfig(){
  uint8_t command[] = {0x01, 0x14, 0x01};
  nfc_send_command(PN532_COMMAND_SAMCONFIG, PN532_SAMCONFIG_INFO, 3);
  last_command_sent = PN532_COMMAND_SAMCONFIG;
  return 1;// response_buf[6];
}

uint8_t nfc_GetFirmwareVersion(){
	uint8_t command[] = {};
	nfc_send_command(PN532_COMMAND_GETFIRMWARE, command, 0);
	last_command_sent = PN532_COMMAND_GETFIRMWARE;
	return 1;
}

uint8_t nfc_is_interrupt_handled(void){
	return interrupt_handled;
}

void nfc_set_interrupt_handled(uint8_t val){
	interrupt_handled = val;
}

uint8_t nfc_get_response_size(){
	switch(last_command_sent){
	case PN532_COMMAND_GETFIRMWARE: return 14;
	case PN532_COMMAND_POWERDOWN: return 5;
	case PN532_COMMAND_GETSTATUS: return 20;
	case PN532_COMMAND_INAUTOPOLL: return 5;
	case PN532_COMMAND_RF_CONFIG: return 5;
	case PN532_COMMAND_INLISTPASSIVETARGET: return 20;
	case PN532_COMMAND_SAMCONFIG: return 10;
	default: return NULL;
	}
}

uint8_t nfc_last_was_ack(){
	return last_was_ack;
}



