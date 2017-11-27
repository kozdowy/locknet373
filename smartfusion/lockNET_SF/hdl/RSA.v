///////////////////////////////////////////////////////////////////////////////////////////////////
// Company: <Name>
//
// File: rsa.v
// File history:
//      Created: 11/26/17: Skeleton and basic implementation
//      <Revision number>: <Date>: <Comments>
//
// Description: 
//
// Takes in a stream of data and either encrypts or decrpyts it based on what is requested.
//
// Targeted device: <Family::SmartFusion> <Die::A2F200M3F> <Package::484 FBGA>
// Author: Erik Liubakka
//
/////////////////////////////////////////////////////////////////////////////////////////////////// 

`define KEY_LENGTH 64 //This is insecure...

module rsa(
    input pclk, // clock
    input nreset, // system reset
    input bus_write_en,
    input bus_read_en,
    input [7:0] bus_addr, // I/O address
    input reg [KEY_LENGTH - 1:0] modulus, //64-bit (8-byte) modulous (the important part).
    input reg [23:0] exponent, //24-bit (3-byte) exponent (65,537 is nearly universally the prime exponenet).
    input wire [31:0] bus_write_data, // data from processor to I/O device (32 bits)
    output reg [31:0] bus_read_data, // data to processor from I/O device (32-bits)   
);





endmodule

