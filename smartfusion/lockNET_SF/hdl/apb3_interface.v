// apb3_interface.v
module apb3_interface(
					PCLK,
					PENABLE,
					PSEL,
					PRESETN,
					PWRITE,
					PREADY,
					PSLVERR,
					PADDR,
					PWDATA,
					PRDATA,
					NP_OUT,
                    SERVO_OUT
                    //TEST
);

// TODO: include stuff for the reset pin on nfc module

// APB Bus Interface
input PCLK,PENABLE, PSEL, PRESETN, PWRITE;
input [31:0] PWDATA;
input [7:0] PADDR;
output [31:0] PRDATA;
output PREADY, PSLVERR;
output NP_OUT;
output SERVO_OUT;
//output TEST;

// Test Interface
// output [4:0] TPS; // Use for your debugging

wire [31:0] NP_PRDATA;
wire [31:0] SERVO_PRDATA;

 
assign BUS_WRITE_EN = (PENABLE && PWRITE && PSEL);
assign BUS_READ_EN = (!PWRITE && PSEL); //Data is ready during first cycle to make it availble on the bus when PENABLE is asserted

assign PREADY = 1'b1;
assign PSLVERR = 1'b0;

wire NP_EN;
wire SERVO_EN;

assign NP_EN = PADDR[7:0] == 8'h00;
assign SERVO_EN = PADDR[7:0] == 8'h04;

assign PRDATA = (NP_EN ? NP_PRDATA :
                (SERVO_EN ? SERVO_PRDATA :
                (32'b0)));

neopixel pxl_0(	.pclk(PCLK),
			    .nreset(PRESETN),
			    .bus_write_en(BUS_WRITE_EN),
			    .bus_read_en(BUS_READ_EN),
			    .bus_addr(PADDR),
			    .bus_write_data(PWDATA),
			    .bus_read_data(NP_PRDATA),
                .np_en(NP_EN),
                .np_out(NP_OUT)
                //.green_t(TEST)
			);

servo servo_0(	.pclk(PCLK),
			    .nreset(PRESETN),
			    .bus_write_en(BUS_WRITE_EN),
			    .bus_read_en(BUS_READ_EN),
			    .bus_addr(PADDR),
			    .bus_write_data(PWDATA),
			    .bus_read_data(SERVO_PRDATA),
                .servo_en(SERVO_EN),
                .servo_out(SERVO_OUT)
			);

endmodule