#include "nfc.h"
#include "drivers/mss_i2c/mss_i2c.h"
#include <drivers/mss_gpio/mss_gpio.h>
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
void nfc_setup(void){
	MSS_I2C_init(&g_mss_i2c1 , PN532_I2C_ADDRESS, MSS_I2C_PCLK_DIV_960 );
	MSS_GPIO_config( MSS_GPIO_0, MSS_GPIO_OUTPUT_MODE);
	int i;

	// Delays required for set up
	MSS_GPIO_set_output(MSS_GPIO_0 ,(uint8_t) 1);
	for(i=0;i<100000;i++);
	MSS_GPIO_set_output(MSS_GPIO_0 ,(uint8_t) 0);
	for(i=0;i<3000000;i++); // We need 400 milisecs (we get 418 msec => OK)
	MSS_GPIO_set_output(MSS_GPIO_0 ,(uint8_t)  1);
	for(i=0;i<100000;i++); //We need 10 msecs (we get 18 msec => OK)
	/* times tested with oscilloscope */
	return;
}



/* NFC read function using I2C
 * Description:
 * Receives: buff -> Buffer (address) where the written data will be stored
 * 			 n	  -> Number of bytes to read
 * Returns:	 Nothing
 */

void nfc_read(uint8_t *buff, uint8_t n){
	// Read transaction
	MSS_I2C_read
	    (
	    	    &g_mss_i2c1,
	    	    PN532_I2C_ADDRESS,
	    	    buff,
	    	    n,
	    	    MSS_I2C_RELEASE_BUS
	    );
	MSS_I2C_wait_complete(&g_mss_i2c1, MSS_I2C_NO_TIMEOUT);
	return;
}


/*
 * Description:
 * Receives:
 * Returns:	 Nothing
 */
void nfc_send_command(uint8_t* command, uint8_t len){
	uint16_t length = 0;
	uint8_t buffer[20];
	buffer[length++] = (uint8_t) PN532_PREAMBLE; // Preamble
	buffer[length++] = 0x00; // Start bits
	buffer[length++] = 0xFF; // Start bits
	buffer[length++] = (len + 1); // Length of (n_data_bytes + TFI byte)
	buffer[length++] = (uint8_t)(0x100 - (len + 1)); // LCS
	buffer[length++] = (uint8_t) PN532_HOST_TO_NFC; // Write
	uint8_t dcs = PN532_HOST_TO_NFC;
	int i = 0;
	for(i = 0; i < len; ++i){
		dcs += command[i];
		buffer[length++] = command[i];
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
 * Receives:	Array LENGTH 9 where the info will be stored
 * Returns:	 field (if external RF is present=1, 0 otherwise)
 */
void nfc_GetGeneralStatus(uint8_t *array){
	uint8_t command[] = {(uint8_t)PN532_COMMAND_GETSTATUS};
	nfc_send_command(command,1);
	for(i=0;i<2000;i++);
	nfc_read(array, 9);

	return array[3];
}







