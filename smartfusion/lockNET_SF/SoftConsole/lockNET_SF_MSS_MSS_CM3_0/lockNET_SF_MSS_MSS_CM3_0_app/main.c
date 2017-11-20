#include <stdio.h>
#include <inttypes.h>
#include "drivers/mss_i2c/mss_i2c.h"
#include <drivers/mss_gpio/mss_gpio.h>
//#include <>
#include "neopixel.h"
#include "servo.h"
#include "nfc.h"
#include "contact_switch.h"


// Interrupts Handler
__attribute__ ((interrupt)) void Fabric_IRQHandler( void )
{
	// Add interrupt status?

	// For the NFC module
	//int n_bytes_to_read =; //Need to look it in the datasheet
	//uint8_t receive_buf[10];
	//nfc_read(receive_buf,n_bytes_to_read);
}

// Main program
int main()
{
	// MSS_GPIO initialization
	MSS_GPIO_init();
	int i;

	// Testing
	nfc_setup();

	uint8_t ack_buf[7];
	uint8_t receive_buf[14];
	uint8_t command[] = {0x02};
	//uint8_t status_buff[9+8];
	//uint8_t rf;

	nfc_send_command(command, 1);
	for(i=0;i<2000;i++);
	nfc_read(ack_buf, sizeof(ack_buf));

	while(1){
//		nfc_send_command(command, 1);
		for(i=0;i<2000;i++);
		nfc_read(receive_buf, sizeof(receive_buf));
//		for(i=0;i<2000;i++);
	}




//	while(1){
//		for(i=0;i<2000;i++);
//		rf = nfc_GetGeneralStatus(status_buff);
//		if(rf == 1){
//			// Intentionally empty for testing. Use breakpoint
//		}
//	}


		/*MSS_I2C_write
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
	//}

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

