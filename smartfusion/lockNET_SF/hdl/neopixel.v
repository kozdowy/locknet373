// neopixel.v
// clk period = 10 ns
`define PERIOD 125 // 1.25us = 1250 ns = 125
`define HOLD 5000 // 50us
`define PERIOD_PLUS_HOLD `PERIOD + `HOLD

`define HIGH_0 7'd40// 8'd32, 0.4us
`define HIGH_1 7'd80 // 8'd64, 0.8us

`define LOW_0 7'd85 // 0.85us
`define LOW_1 7'd45 // 0.45us 

module neopixel(
           /*** APB3 BUS INTERFACE ***/
           input             pclk, // clock
           input             nreset, // system reset
           input                bus_write_en,
            //input               bus_read_en,
            input np_en,
 //          input             PWRITE, // distinguishes read and write cycles
           input [7:0]      bus_addr, // I/O address
           input wire [31:0] bus_write_data, // data from processor to I/O device (32 bits)
           //output reg [31:0] bus_read_data, // data to processor from I/O device (32-bits)
           output reg        np_out
            //output [23:0] pixel_state
           );

   wire                      write_pulse;
   wire read_pulse;

   assign write_pulse = bus_write_en & np_en;
   //assign read_pulse = bus_read_en & np_en;

   reg [23:0]                neopixel_reg;
   reg [23:0]                neopixel_reg_n;
   reg [4:0]                 send_pixel;
   reg [4:0]                 send_pixel_n;

   reg [13:0]                counter;
   reg [13:0]                counter_n;
   //reg [13:0]                 compare_reg;
   //reg [13:0]                 compare_reg_n;

   reg np_out_n;

   //assign pixel_state = neopixel_reg;

   always @* begin
      counter_n = counter;
      neopixel_reg_n = neopixel_reg;
      np_out_n = np_out;
      //compare_reg_n = compare_reg;
      send_pixel_n = send_pixel;
      //bus_read_data = 32'b0;

      if (send_pixel != 5'b0) begin
         counter_n = counter + 1;
            /*
         if (neopixel_reg[23]) begin
            compare_reg_n = high_one;
         end else begin
            compare_reg_n = high_zero;
         end // if (neopixel_reg[23])
            */

         if ((neopixel_reg[23] && counter < `HIGH_1) ||
             (~neopixel_reg[23] && counter < `HIGH_0)) begin
            np_out_n = 1'b1;
         end else if (counter < `PERIOD) begin
            np_out_n = 1'b0;
         end else if (counter == `PERIOD && send_pixel != 25) begin
            neopixel_reg_n[23:1] = neopixel_reg[22:0];
            neopixel_reg_n[0] = neopixel_reg[23];
            send_pixel_n = send_pixel + 1;
            counter_n = 14'b0;
         end // if (counter < compare_reg)

         if (send_pixel == 25 && counter < `PERIOD_PLUS_HOLD) begin
            np_out_n = 1'b0;
         end else if (send_pixel == 25 && counter == `PERIOD_PLUS_HOLD) begin
            send_pixel_n = 1'b0;
         end // if (send_pixel == 25 && counter < `PERIOD_PLUS_HOLD)

      end else if(write_pulse) begin // ignore write if currently sending
         if (bus_write_data[24]) begin // sets register
            neopixel_reg_n = bus_write_data[23:0];
         end else if (bus_write_data[25]) begin // begins send
            send_pixel_n = 5'b1;
         end else if (bus_write_data[26]) begin // clears register
            neopixel_reg_n = 24'b0;
         end // if (bus_write_data[24])
      end // if (send_pixel != 0)

/*
      if (read_pulse) begin
        bus_read_data = {8'b0, neopixel_reg};
      end // if (read_pulse)
*/

      if (send_pixel == 5'b0) begin
         counter_n = 8'b0;
      end // if (send_pixel == 5'b0)
   end // always @*

   always @(posedge pclk) begin
      if (!nreset) begin
         neopixel_reg <= 24'b0;
         send_pixel <= 8'b0;
         counter <= 8'b0;
         //compare_reg <= 8'b0;
         np_out <= 1'b0;
      end else begin
         neopixel_reg <= neopixel_reg_n;
         send_pixel <= send_pixel_n;
         counter <= counter_n;
         //compare_reg <= compare_reg_n;
         np_out <= np_out_n;
      end // if (!nreset)
   end // always @(posedge pclk)

endmodule
