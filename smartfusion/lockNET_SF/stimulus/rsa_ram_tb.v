///////////////////////////////////////////////////////////////////////////////////////////////////
// Company: <Name>
//
// File: rsa_tb.v
// File history:
//      <Revision number>: <Date>: <Comments>
//      <Revision number>: <Date>: <Comments>
//      <Revision number>: <Date>: <Comments>
//
// Description: 
//
// <Description here>
//
// Targeted device: <Family::SmartFusion> <Die::A2F200M3F> <Package::484 FBGA>
// Author: <Name>
//
/////////////////////////////////////////////////////////////////////////////////////////////////// 

`timescale 1ns/100ps

`define BIT_SWITCH_ADDR 8'h08
`define MESSAGE_ADDR 8'h0C
`define MODULUS_ADDR 8'h10
`define EXPONENT_ADDR 8'h14
`define RESIDUE_ADDR 8'h18
`define RSA_ENCRYPT_ADDR 8'h1C
`define RESULT_ADDR 8'h20
`define RESULT_VALID_ADDR 8'h24

`define MESSAGE 64'hDCBAABCDABCDDCBA
`define MSG_ENC 64'h0f886e095e89cf95
`define PUB_EXP 64'h0000000000010001
`define PRI_EXP 64'h044de026cabdb311

module rsa_ram_tb;

parameter SYSCLK_PERIOD = 100;// 10MHZ

reg SYSCLK;
reg NSYSRESET;


reg bus_write_en;
reg bus_read_en;
reg RSA_ENABLE;
reg [7:0] bus_addr; // I/O address
reg [31:0] bus_write_data; // data from processor to I/O device (32 bits)
wire [31:0] bus_read_data; // data to processor from I/O device (32-bits)
wire result_valid;
wire [63:0] P;
wire P_ready;

reg [63:0] msg;
reg [63:0] exp;

wire [31:0] RAM_DINA;
wire [31:0] RAM_DINB;
wire [6:0] RAM_ADDRA;
wire [6:0] RAM_ADDRB;
wire [31:0] RAM_DOUTA;
wire [31:0] RAM_DOUTB;
wire RAM_RWA;
wire RAM_RWB;
wire RAM_BLKA;
wire RAM_BLKB;
//wire SYSRESET;
//assign SYSRESET = ~NSYSRESET;

initial
begin
    SYSCLK = 1'b0;
    NSYSRESET = 1'b0;
    bus_read_en = 1'b0;
    bus_write_en = 1'b0;
    RSA_ENABLE = 1'b0;
    bus_addr = 8'b0;
    //msg = `MESSAGE;
    msg = `MSG_ENC;
    //exp = `PUB_EXP;
    exp = `PRI_EXP;
    //bus_read_data = 32'b0;
end


//////////////////////////////////////////////////////////////////////
// Reset Pulse
//////////////////////////////////////////////////////////////////////
initial
begin
    #(SYSCLK_PERIOD * 10 )
        NSYSRESET = 1'b1;

    #(SYSCLK_PERIOD * 10 )
        NSYSRESET = 1'b0;

    #(SYSCLK_PERIOD * 10 )
        NSYSRESET = 1'b1;
    
    #(SYSCLK_PERIOD * 10 )

    bus_write_en = 1'b1;
    RSA_ENABLE = 1'b1;

    // sending message
    bus_addr = `BIT_SWITCH_ADDR;
    bus_write_data = 32'b0;
    #(SYSCLK_PERIOD)

    bus_addr = `MESSAGE_ADDR;
    bus_write_data = msg[31:0];
    #(SYSCLK_PERIOD)

    bus_addr = `BIT_SWITCH_ADDR;
    bus_write_data = 32'b1;
    #(SYSCLK_PERIOD)

    bus_addr = `MESSAGE_ADDR;
    bus_write_data = msg[63:32];
    #(SYSCLK_PERIOD)

    // sending exponent
    bus_addr = `BIT_SWITCH_ADDR;
    bus_write_data = 32'b0;
    #(SYSCLK_PERIOD)

    bus_addr = `EXPONENT_ADDR;
    bus_write_data = exp[31:0];
    #(SYSCLK_PERIOD)

    bus_addr = `BIT_SWITCH_ADDR;
    bus_write_data = 32'b1;
    #(SYSCLK_PERIOD)

    bus_addr = `EXPONENT_ADDR;
    bus_write_data = exp[63:32];
    #(SYSCLK_PERIOD)

    bus_addr = `BIT_SWITCH_ADDR;
    bus_write_data = 32'b0;
    #(SYSCLK_PERIOD)

    bus_addr = `MODULUS_ADDR;
    bus_write_data = 32'h24029417;
    #(SYSCLK_PERIOD)

    bus_addr = `BIT_SWITCH_ADDR;
    bus_write_data = 32'b1;
    #(SYSCLK_PERIOD)

    bus_addr = `MODULUS_ADDR;
    bus_write_data = 32'heda515ef;
    #(SYSCLK_PERIOD)

    bus_addr = `BIT_SWITCH_ADDR;
    bus_write_data = 32'b0;

    #(SYSCLK_PERIOD)
    bus_addr = `RESIDUE_ADDR;
    bus_write_data = 32'h5a1f75d5;

    #(SYSCLK_PERIOD)
    bus_addr = `BIT_SWITCH_ADDR;
    bus_write_data = 32'b1;

    #(SYSCLK_PERIOD)
    bus_addr = `RESIDUE_ADDR;
    bus_write_data = 32'h859cfcfb;

    #(SYSCLK_PERIOD)
    bus_addr = `BIT_SWITCH_ADDR;
    bus_write_data = 32'b0;

    #(SYSCLK_PERIOD)
    bus_addr = `RSA_ENCRYPT_ADDR;
    bus_write_data = 32'h0001;

    #(SYSCLK_PERIOD)
    bus_write_en = 1'b0;

    while (!result_valid) begin
        //if (P_ready) begin
        //    $display("P = %x", P);
        //end
        #(SYSCLK_PERIOD);
    end

    #(SYSCLK_PERIOD * 10);
    $finish;
//write message
//write exponent
//write modulus
//write residue
//write rsa_encrypt

//wait until result_valid
end


//////////////////////////////////////////////////////////////////////
// Clock Driver
//////////////////////////////////////////////////////////////////////
always @(SYSCLK)
    #(SYSCLK_PERIOD / 2.0) SYSCLK <= !SYSCLK;


//////////////////////////////////////////////////////////////////////
// Instantiate Unit Under Test:  lockNET_SF
//////////////////////////////////////////////////////////////////////
rsa rsa_dut(
    .pclk(SYSCLK), // clock
    .nreset(NSYSRESET), // system reset
    .bus_write_en(bus_write_en),
    .bus_read_en(bus_read_en),
    .RSA_ENABLE(RSA_ENABLE),
    .bus_addr(bus_addr), // I/O address
    .bus_write_data(bus_write_data), // data from processor to I/O device (32 bits)
    .bus_read_data(bus_read_data), // data to processor from I/O device (32-bits)
    .result_valid(result_valid),
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

RSA_64b_RAM rsa_ram(
       .DINA(RAM_DINA),
       .DOUTA(RAM_DOUTA),
       .DINB(RAM_DINB),
       .DOUTB(RAM_DOUTB),
       .ADDRA(RAM_ADDRA),
       .ADDRB(RAM_ADDRB),
       .RWA(RAM_RWA),
       .RWB(RAM_RWB),
       .BLKA(RAM_BLKA),
       .BLKB(RAM_BLKB),
       .CLKAB(SYSCLK),
       .RESET(NSYSRESET)
);

endmodule

