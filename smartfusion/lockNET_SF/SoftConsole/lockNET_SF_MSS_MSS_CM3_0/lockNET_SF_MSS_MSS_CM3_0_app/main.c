#include <stdio.h>
#include <inttypes.h>
#include "drivers/mss_i2c/mss_i2c.h"
#include <drivers/mss_gpio/mss_gpio.h>
//#include <>
#include "neopixel.h"
#include "servo.h"
#include "contact_switch.h"

/*the target address is a 7(8) bit value! */
// Target address (got it from datasheet)
#define PN532_I2C_ADDRESS (0x48>>1)
#define PN532_I2C_ADDRESS_WRITE 0x48 //DONT USE
#define PN532_I2C_ADDRESS_READ 0x49 // DONT USE


// Initialization bytes
#define PN532_PREAMBLE (0x00)
#define PN532_STARTCODE2 (0xFF)

// Prototypes
void nfc_read(uint8_t *buff, uint8_t n);

// Main program
int main()
{
	// I2C initialization
	MSS_I2C_init(&g_mss_i2c1 , PN532_I2C_ADDRESS, MSS_I2C_PCLK_DIV_256 );
	MSS_GPIO_init();
	MSS_GPIO_config( MSS_GPIO_0, MSS_GPIO_OUTPUT_MODE);
	int i;

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
	// Initialization of the NFC
	// We need to do some stuff with the reset pin of the device
//	volatile uint32_t * reset_pin = (uint32_t *) PIN_RESET;
//	(*reset_pin)=1;
//	for(int i=0;i<10;i++);
//	(*reset_pin)=0;
//	for(int i=0;i<1000;i++);
//	(*reset_pin)=1;
//	for(int i=0;i<100;i++);
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
		// Lets try to communicate with the device with a simple write (see if we get an ack)
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

	// The function readdata in Arduino it's like

//	MSS_I2C_write
//	(
//		&g_mss_i2c1,
//		TARGET_ADDRESS,
//		transmit_buf,
//		sizeof(transmit_buf),
//		MSS_I2C_RELEASE_BUS
//	);
//	MSS_I2C_wait_complete(&g_mss_i2c1, MSS_I2C_NO_TIMEOUT);

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



