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
`define MSG_ENC 64'h19992ea93fb8845b
`define PUB_EXP 64'h0000000000010001
`define PRI_EXP 64'h044de026cabdb311

module rsa_tb;

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

reg [63:0] msg;
reg [63:0] exp;

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
    bus_write_data = 32'h46647445;

    #(SYSCLK_PERIOD)
    bus_addr = `BIT_SWITCH_ADDR;
    bus_write_data = 32'b1;

    #(SYSCLK_PERIOD)
    bus_addr = `RESIDUE_ADDR;
    bus_write_data = 32'h4c0f058c;

    #(SYSCLK_PERIOD)
    bus_addr = `BIT_SWITCH_ADDR;
    bus_write_data = 32'b0;

    #(SYSCLK_PERIOD)
    bus_addr = `RSA_ENCRYPT_ADDR;
    bus_write_data = 32'h0001;

    #(SYSCLK_PERIOD)
    bus_write_en = 1'b0;

    while (!result_valid) begin
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
    .result_valid(result_valid)
);

endmodule

