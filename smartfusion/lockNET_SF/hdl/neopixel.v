// pwm.v
`define PERIOD 100 // 1.25us
`define HOLD 4000 // 50us
`define PERIOD_PLUS_HOLD `PERIOD + `HOLD

`define HIGH_0 32 // 0.4us
`define LOW_0 68 // 0.85us
`define HIGH_1 64 // 0.8us
`define LOW_1 34 // 0.45us

module pwm(
           /*** APB3 BUS INTERFACE ***/
           input             PCLK, // clock
           input             PRESERN, // system reset
           input             PSEL, // peripheral select
           input             PENABLE, // distinguishes access phase
           output wire       PREADY, // peripheral ready signal
           output wire       PSLVERR, // error signal
           input             PWRITE, // distinguishes read and write cycles
           input [31:0]      PADDR, // I/O address
           input wire [31:0] PWDATA, // data from processor to I/O device (32 bits)
           output reg [31:0] PRDATA, // data to processor from I/O device (32-bits)
           //output [15:0] test,
           output reg        np_out
           );

   wire                      write_pulse;

   /*
    assign test[15] = PCLK;
    assign test[14:12] = PADDR[4:2];
    assign test[11] = PSEL;
    assign test[10] = PENABLE;
    assign test[9] = PRESERN;
    assign test[8] = PWRITE;
    assign test[7:0] = PWRITE ? PWDATA : PRDATA;
    */

   assign PSLVERR = 0;
   assign PREADY = 1;

   assign write_pulse = PWRITE & PADDR[2];

   reg [23:0]                neopixel_reg;
   reg [23:0]                neopixel_reg_n;
   reg [4:0]                 send_pixel;
   reg [4:0]                 send_pixel_n;

   reg [13:0]                counter;
   reg [13:0]                counter_n;
   reg [7:0]                 compare_reg;
   reg [7:0]                 compare_reg_n;

   assign PRDATA[23:0] = neopixel_reg;
   assign PRDATA[31:24] = 8'b0;

   always @ ( /*AUTOSENSE*/ ) begin
      counter_n = counter;
      neopixel_reg_n = neopixel_reg;
      np_out_n = np_out;
      compare_reg_n = compare_reg;
      send_pixel_n = send_pixel;

      if (send_pixel != 0) begin
         counter_n = counter + 1;
         if (neopixel_reg[23]) begin
            compare_reg_n = `HIGH_1;
         end else begin
            compare_reg_n = `HIGH_0;
         end

         if (counter < compare_reg) begin
            np_out_n = 1'b1;
         end else if (counter < `PERIOD) begin
            np_out_n = 1'b0;
         end else if (counter == `PERIOD) begin
            neopixel_reg_n[31:1] = neopixel_reg[30:0];
            neopixel_reg_n[0] = neopixel_reg[31];
            send_pixel_n = send_pixel + 1;
         end

         if (send_pixel == 25 && counter < `PERIOD_PLUS_HOLD) begin
            np_out_n = 1'b0;
         end else if (send_pixel == 25 && counter == `PERIOD_PLUS_HOLD) begin
            send_pixel_n = 1'b0;
         end

      end

      else if(write_pulse) begin // ignore write if currently sending
         if (PWDATA[24]) begin // sets register
            neopixel_reg_n = PWDATA[23:0];
         end else if (PWDATA[25]) begin // begins send
            send_pixel_n = 5'b1;
         end else if (PWDATA[26]) begin // clears register
            neopixel_reg_n = 24'b0;
         end
      end

      if (send_pixel == 5'b0) begin
         counter_n = 8'b0;
      end
   end // always @ (posedge PCLK)

   always @(posedge PCLK) begin
      if (!PRESERN) begin
         neopixel_reg <= 24'b0;
         send_pixel <= 8'b0;
         counter <= 8'b0;
         compare_reg <= 8'b0;
         np_out <= 1'b0;
      end else begin
         neopixel_reg <= neopixel_reg_n;
         send_pixel <= send_pixel_n;
         counter <= counter_n;
         compare_reg <= compare_reg_n;
         np_out <= np_out_n;
      end
   end

endmodule
