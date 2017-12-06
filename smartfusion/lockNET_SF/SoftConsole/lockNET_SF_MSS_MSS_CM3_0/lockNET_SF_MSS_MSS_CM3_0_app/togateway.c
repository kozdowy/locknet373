/*
 * togateway.c
 *
 *  Created on: Nov 30, 2017
 *      Author: peces
 */
#include "lora_client_ex.h"
#include <stdio.h>

void LORA_client_to_gateway_setup (void){
	  if (LORA_init() == 1){
		  printf("init failed\r\n");
		  return;
	  }

	  while (1){
		  LORA_client_to_gateway_loop();
		  printf("Test\n\r");
	  }

}

void LORA_client_to_gateway_loop(void)
{
	printf("Sending to LoRa Server\r\n");
  uint8_t data[] = "Hello, this is device 2";
  LORA_send(data, sizeof(data));

  LORA_wait_packet_sent(0);
  // Now wait for a reply
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);


  if (LORA_wait_available_timeout(650))
  {
    // Should be a reply message for us now
    if (LORA_recv(buf, &len))
   {
      printf("got reply: ");
      printf((char*)buf);
      printf("\r\n");
//      Serial.print("RSSI: ");
//      printf(rf95.lastRssi(), DEC);
    }
    else
    {
      printf("recv failed");
    }
  }
  else
  {
    printf("No reply, is rf95_server running?\r\n");
  }
  //delay(400);
  int i;
  for (i = 0; i < 10000; ++i);
  return;
}
