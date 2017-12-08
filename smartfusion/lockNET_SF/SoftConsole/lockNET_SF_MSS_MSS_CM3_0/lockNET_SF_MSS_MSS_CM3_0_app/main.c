#include <stdio.h>
#include <inttypes.h>
#include "drivers/mss_i2c/mss_i2c.h"
#include <drivers/mss_gpio/mss_gpio.h>
#include <drivers/mss_uart/mss_uart.h>
//#include "neopixel.h"
//#include "servo.h"
//#include "nfc.h"
//#include "contact_switch.h"
//#include "lora.h"
//#include "lora_client_ex.h"
#include "rsa.h"
//#include "lora_server_ex.h"

uint8_t last_was_ack = 0;
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
	// Add interrupt status?
	MSS_GPIO_clear_irq(MSS_GPIO_1);
	// For the NFC module
	if (!last_was_ack){
		last_was_ack = 1;
	} else {
		last_was_ack = 0;
	}
	set_interrupt_handled(1);
}

// Main program
int main()
{
	//NVIC_EnableIRQ(Fabric_IRQn);
	RSA_init();
	RSA_test();
	//while (!RSA_result_ready);
	while (!RSA_is_ready());
	uint32_t result[2];
	RSA_read_result(result);
	printf("result = %x %x", result[0], result[1]);
//	MSS_GPIO_init();
//
//	MSS_GPIO_config( MSS_GPIO_8, MSS_GPIO_OUTPUT_MODE);
//	MSS_GPIO_config( MSS_GPIO_9, MSS_GPIO_INPUT_MODE | MSS_GPIO_IRQ_EDGE_POSITIVE );
//	MSS_GPIO_enable_irq(MSS_GPIO_9);
//	//MSS_GPIO_config( MSS_GPIO_10, MSS_GPIO_INOUT_MODE);
//	MSS_GPIO_set_output(MSS_GPIO_8, 1);
//
//	int init_res = LORA_init();
//
//	uint8_t send_buf[] = {0x01, 0x03, 0x05, 0x07};
//
//	LORA_write(RH_RF95_REG_0D_FIFO_ADDR_PTR, 0);
//	LORA_burst_write(RH_RF95_REG_00_FIFO, send_buf, 4);
//
//	int i;
//	for (i = 0; i < 100000; ++i);
//
//	LORA_write(RH_RF95_REG_0D_FIFO_ADDR_PTR, 0);
//	uint8_t read_buf[4];
//	LORA_burst_read(RH_RF95_REG_00_FIFO, read_buf, 4);
//
//	LORA_client_ex_setup();
	//LORA_read_addr(LORA_RegFifoTxBaseAddr);
	/*
	// MSS_GPIO initialization
	MSS_GPIO_init();
	int i;

	// Testing
	nfc_setup();

	uint8_t ack_buf[7];
	uint8_t receive_buf[20];
	uint8_t command[] = {};
	//uint8_t status_buff[9+8];
	//uint8_t rf;
	//uint8_t rf = nfc_GetGeneralStatus(receive_buf);
	while(1) nfc_InListPassiveTarget(receive_buf);
	//nfc_send_command(0x02, command, 0);
	//for(i=0;i<2000;i++);
	//nfc_read(ack_buf, sizeof(ack_buf), 0);
	//while(rf != 0xD5){
		//nfc_GetGeneralStatus(receive_buf);
		//rf = nfc_InListPassiveTarget(receive_buf);
//		nfc_send_command(command, 1);
		//for(i=0;i<20000;i++);
		//nfc_read(receive_buf, sizeof(receive_buf), 0);
//		for(i=0;i<2000;i++);
	//}




//	while(1){
//		for(i=0;i<2000;i++);
//		rf = nfc_GetGeneralStatus(status_buff);
//		if(rf == 1){
//			// Intentionally empty for testing. Use breakpoint
//		}
//	}
 * */


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
