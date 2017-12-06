// servo.v
`define SERVO_PERIOD 2000000

module servo(
           /*** APB3 BUS INTERFACE ***/
           input             pclk, // clock
           input             nreset, // system reset
           input                bus_write_en,
            //input               bus_read_en,
            input servo_en,
 //          input             PWRITE, // distinguishes read and write cycles
           input [7:0]      bus_addr, // I/O address
           input wire [31:0] bus_write_data, // data from processor to I/O device (32 bits)
           //output reg [31:0] bus_read_data, // data to processor from I/O device (32-bits)
           output reg        servo_out
           );

   wire                      write_pulse;

   assign write_pulse = bus_write_en & servo_en;
   //assign read_pulse = bus_read_en & servo_en;

   reg [31:0]                pulse_comp;
   reg [31:0]                pulse_comp_n;

   reg [31:0]                counter;
   reg [31:0]                counter_n;

   reg servo_out_n;

   always @* begin
      counter_n = counter + 1'b1;
      pulse_comp_n = pulse_comp;
      servo_out_n = servo_out;
      //bus_read_data = 32'b0;

      if(write_pulse) begin
         pulse_comp_n = bus_write_data;
      end // if (write_pulse)

/*
      if (read_pulse) begin
        bus_read_data = pulse_comp;
      end // if (read_pulse)
*/

      if (counter < pulse_comp) begin
        servo_out_n = 1'b1;
      end else if (counter < `SERVO_PERIOD) begin
        servo_out_n = 1'b0;
      end else if (counter == `SERVO_PERIOD) begin
        servo_out_n = 1'b0;
        counter_n = 32'b0;
      end // if (counter < pulse_comp)
   end // always @*

   always @(posedge pclk) begin
      if (!nreset) begin
         pulse_comp <= 32'b0;
         counter <= 32'b0;
         servo_out <= 1'b0;
      end else begin
         pulse_comp <= pulse_comp_n;
         counter <= counter_n;
         servo_out <= servo_out_n;
      end // if (!nreset)
   end // always @(posedge pclk)

endmodule
