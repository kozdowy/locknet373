#include <stdio.h>
#include <inttypes.h>
#include "drivers/mss_i2c/mss_i2c.h"
#include <drivers/mss_gpio/mss_gpio.h>
//#include <>
#include "neopixel.h"
#include "servo.h"
#include "contact_switch.h"


// Interrupts Handler
__attribute__ ((interrupt)) void Fabric_IRQHandler( void )
{
	// Add interrupt status?

	// For the NFC module
	int n_bytes_to_read =; //Need to look it in the datasheet
	uint8_t receive_buf[10];
	nfc_read(receive_buf,n_bytes_to_read);
}

// Main program
int main()
{
	// MSS_GPIO initialization
	MSS_GPIO_init();
	int i;

	// Testing
	MSS_I2C_init(&g_mss_i2c1 , PN532_I2C_ADDRESS, MSS_I2C_PCLK_DIV_256 );
	MSS_GPIO_config( MSS_GPIO_0, MSS_GPIO_OUTPUT_MODE);


	// Delays required for setup
	MSS_GPIO_set_output(MSS_GPIO_0 ,(uint8_t) 1);
	for(i=0;i<100000;i++);
	MSS_GPIO_set_output(MSS_GPIO_0 ,(uint8_t) 0);
	for(i=0;i<3000000;i++); // We need 400 milisecs (we get 418 msec => OK)
	MSS_GPIO_set_output(MSS_GPIO_0 ,(uint8_t)  1);
	for(i=0;i<100000;i++); //We need 10 msecs (we get 18 msec => OK)

	uint8_t receive_buf[10];
	uint8_t transmit_buf[] = {0x01};

	while(1){
		nfc_read(receive_buf, sizeof(receive_buf));

		/*
		MSS_I2C_write
		(
			&g_mss_i2c1,
			PN532_I2C_ADDRESS,
			transmit_buf,
			sizeof(transmit_buf),
			MSS_I2C_RELEASE_BUS
		);
		MSS_I2C_wait_complete(&g_mss_i2c1, MSS_I2C_NO_TIMEOUT);
		*/

		//for(i=0;i<100000;i++);
	}

	/*
	NP_init();

	CS_init();

	while (1){
		if (CS_get_door_status() == CS_DOOR_OPEN){
			NP_set_pixel_c(0x55, 0x00, 0x00);
			NP_apply();
		} else{
			NP_set_pixel(NP_COLOUR_GREEN);
			NP_apply();
		}
	}
	return 0;
	*/

	return(0);

}




/* I2C write function
 * Description:
 * Receives:
 */
//void nfc_write(uint8_t *transmit_buf, uint8_t cmdlen)




/* NFC read function using I2C
 * Description:
 * Receives: buff -> Buffer (address) where the written data will be stored
 * 			 n	  -> Number of bytes to read
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



