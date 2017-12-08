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
                    SERVO_OUT,
                    FABINT,
                    RAM_DINA,
                    RAM_DINB,
                    RAM_ADDRA,
                    RAM_ADDRB,
                    RAM_RWA,
                    RAM_RWB,
                    RAM_BLKA,
                    RAM_BLKB,
                    RAM_RESET,
                    RAM_DOUTA,
                    RAM_DOUTB
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
//input PIN_NFC_IRQ;
output FABINT;
//output RSA_VALID;
//output TEST;
output [31:0] RAM_DINA;
output [31:0] RAM_DINB;
output [6:0] RAM_ADDRA;
output [6:0] RAM_ADDRB;
output RAM_RWA;
output RAM_RWB;
output RAM_BLKA;
output RAM_BLKB;
output RAM_RESET;
input [31:0] RAM_DOUTA;
input [31:0] RAM_DOUTB;

// Test Interface
// output [4:0] TPS; // Use for your debugging

wire [31:0] NP_PRDATA;
wire [31:0] SERVO_PRDATA;
wire [31:0] RSA_PRDATA;

 
assign BUS_WRITE_EN = (PENABLE & PWRITE & PSEL);
assign BUS_READ_EN = (~PWRITE & PSEL); //Data is ready during first cycle to make it availble on the bus when PENABLE is asserted

assign PREADY = 1'b1;
assign PSLVERR = 1'b0;

wire NP_EN;
wire SERVO_EN;
wire RSA_EN;

//wire RSA_VALID;

assign NP_EN = ~(| PADDR[7:0]);
assign SERVO_EN = PADDR[7:0] == 8'h04;
//RSA has many internal addresses. Look at RSA.v to see assignments.
assign RSA_EN = (PADDR[7:0] == 8'h08 || PADDR[7:0] == 8'h0C
                || PADDR[7:0] == 8'h10 || PADDR[7:0] == 8'h14
                || PADDR[7:0] == 8'h18 || PADDR[7:0] == 8'h1C
                || PADDR[7:0] == 8'h20 || PADDR[7:0] == 8'h24);


assign PRDATA = (NP_EN ? NP_PRDATA :
                (SERVO_EN ? SERVO_PRDATA :
                (RSA_EN ? RSA_PRDATA :
                (32'b0))));

//assign PRDATA = RSA_PRDATA;


neopixel pxl_0(	.pclk(PCLK),
			    .nreset(PRESETN),
			    .bus_write_en(BUS_WRITE_EN),
			    //.bus_read_en(BUS_READ_EN),
			    .bus_addr(PADDR),
			    .bus_write_data(PWDATA),
			    //.bus_read_data(NP_PRDATA),
                .np_en(NP_EN),
                .np_out(NP_OUT)
                //.green_t(TEST)
			);

servo servo_0(	.pclk(PCLK),
			    .nreset(PRESETN),
			    .bus_write_en(BUS_WRITE_EN),
			    //.bus_read_en(BUS_READ_EN),
			    .bus_addr(PADDR),
			    .bus_write_data(PWDATA),
			    //.bus_read_data(SERVO_PRDATA),
                .servo_en(SERVO_EN),
                .servo_out(SERVO_OUT)
			);

/*
nfc nfc(
                .pclk(PCLK),
			    .nreset(PRESETN),
			    .bus_write_en(BUS_WRITE_EN),
			    .bus_read_en(BUS_READ_EN),
			    .bus_addr(PADDR),
			    .bus_write_data(PWDATA),
			    .bus_read_data(PRDATA),
                .fabint(FABINT),
                .irq_pin(PIN_NFC_IRQ)
          );
*/

rsa rsa_0(
                .pclk(PCLK), // clock
                .nreset(PRESETN), // system reset
                .bus_write_en(BUS_WRITE_EN),
                .bus_read_en(BUS_READ_EN),
                .RSA_ENABLE(RSA_EN),
                .bus_addr(PADDR), // I/O address
                .bus_write_data(PWDATA), // data from processor to I/O device (32 bits)
                .bus_read_data(RSA_PRDATA), // data to processor from I/O device (32-bits)   
                .result_valid(FABINT),
                .RAM_DINA(RAM_DINA),
                .RAM_DINB(RAM_DINB),
                .RAM_ADDRA(RAM_ADDRA),
                .RAM_ADDRB(RAM_ADDRB),
                .RAM_RWA(RAM_RWA),
                .RAM_RWB(RAM_RWB),
                .RAM_BLKA(RAM_BLKA),
                .RAM_BLKB(RAM_BLKB),
                .RAM_DOUTA(RAM_DOUTA),
                .RAM_DOUTB(RAM_DOUTB)
            );

endmodule