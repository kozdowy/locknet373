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

`define KEY_LENGTH 64 //This is insecure... but this is just a proof of concept.
`define BIT_SWITCH_ADDR 8'h08
`define MESSAGE_ADDR 8'h0C
`define MODULUS_ADDR 8'h10
`define EXPONENT_ADDR 8'h14
`define RESIDUE_ADDR 8'h18
`define RSA_ENCRYPT_ADDR 8'h1C
`define RESULT_ADDR 8'h20
`define RESULT_VALID_ADDR 8'h24

module rsa(
    input pclk, // clock
    input nreset, // system reset
    input bus_write_en,
    input bus_read_en,
    input RSA_ENABLE,
    input [7:0] bus_addr, // I/O address
    input wire [31:0] bus_write_data, // data from processor to I/O device (32 bits)
    output reg [31:0] bus_read_data // data to processor from I/O device (32-bits)   
);

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This is used to minimize internal addresses.
// When set to 0, a r/w operation will assume lower 32bits of register.
// When set to 1, will assume upper 32bits.
// i.e. A write to address 8'h0C when bit_switch is 1 will set message_upper = bus_write_data.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
reg bit_switch = 0;

reg [31:0] message_upper; //message must be at most as large as the modulus.
reg [31:0] message_lower;
wire [31:0] message;
assign message = (bit_switch)? message_upper : message_lower;

reg [31:0] modulus_upper; //64-bit (8-byte) modulous (the important part).
reg [31:0] modulus_lower;
wire [31:0] modulus;
assign modulus = (bit_switch)? modulus_upper : modulus_lower;

reg [31:0] exponent_upper; //64-bit (8-byte) exponent (65,537 is nearly universally the public exponenet, but the private exponent is normally choosen to be close to the size of the modulus).
reg [31:0] exponent_lower;
wire [31:0] exponent;
assign exponent = (bit_switch)? exponent_upper : exponent_lower;
 
reg [31:0] residue_upper; //This needs to be equal to R^2 mod M, where R = 2^KEY_LENGTH and M=modulus.
reg [31:0] residue_lower;
wire [31:0] residue;
assign residue = (bit_switch)? residue_upper : residue_lower;

reg rsa_encrypt = 0; //When this is set to 1, the module will begin encryption/decryption. 


reg [4:0] encrypt_state;
reg [6:0] counter;

reg [31:0] result_upper;
reg [31:0] result_lower;
wire [31:0] result;
assign result = (bit_switch)? result_upper : result_lower;

reg [31:0] temp_result_upper;
reg [31:0] temp_result_lower;

reg [31:0] Z_upper;
reg [31:0] Z_lower;

reg result_valid;

reg [31:0] rsa_read_out;

reg MonMult_GO;
reg [63:0] A;
reg [63:0] B;
wire [63:0] P;
wire MonMult_ready;

MonMult MonMult_0(
    .pclk(pclk),
    .nreset(nreset),
    .GO(MonMult_GO),
    .A(A),
    .B(B),
    .M({modulus_upper, modulus_lower}),
    .P(P),
    .is_ready(MonMult_ready));


wire rsa_write;
wire rsa_read;
assign rsa_write = bus_write_en && RSA_ENABLE;
assign rsa_read = bus_read_en && RSA_ENABLE;

always @(posedge pclk) begin 
    //write to MMIO
    if(!nreset) begin
        bit_switch <= 1'b0;
        message_upper <= 32'b0;
        message_lower <= 32'b0;
        modulus_upper <= 32'b0;
        modulus_lower <= 32'b0;
        exponent_upper <= 32'b0;
        exponent_lower <= 32'b0;
        residue_upper <= 32'b0;
        residue_lower <= 32'b0;
    end else if(rsa_write) begin
        if(bus_addr == `BIT_SWITCH_ADDR) begin
            bit_switch <= bus_write_data[0];
        end else if(bus_addr == `MESSAGE_ADDR) begin
            if(bit_switch)
                message_upper <= bus_write_data;
            else
                message_lower <= bus_write_data;
            rsa_encrypt <= 1'b0;
        end else if(bus_addr == `MODULUS_ADDR) begin
            if(bit_switch)
                modulus_upper <= bus_write_data;
            else
                modulus_lower <= bus_write_data;
            rsa_encrypt <= 1'b0;
        end else if(bus_addr == `EXPONENT_ADDR) begin
            if(bit_switch)
                exponent_upper <= bus_write_data;
            else
                exponent_lower <= bus_write_data;
            rsa_encrypt <= 1'b0;
        end else if(bus_addr == `RESIDUE_ADDR) begin
            if(bit_switch)
                residue_upper <= bus_write_data;
            else
                residue_lower <= bus_write_data;
            rsa_encrypt <= 1'b0;
        end else if(bus_addr == `RSA_ENCRYPT_ADDR) begin
            rsa_encrypt <= bus_write_data[0];
        end
    end
end
   
always @(posedge pclk) begin 
    //read from MMIO
    if(rsa_read) begin
        case(bus_addr)
            `BIT_SWITCH_ADDR: bus_read_data = {31'b0, bit_switch};
            `MESSAGE_ADDR: bus_read_data = message; 
            `MODULUS_ADDR: bus_read_data = modulus; 
            `EXPONENT_ADDR: bus_read_data = exponent; 
            `RESIDUE_ADDR: bus_read_data = residue; 
            `RSA_ENCRYPT_ADDR: bus_read_data = {31'b0, rsa_encrypt}; 
            `RESULT_ADDR: bus_read_data = result;
            `RESULT_VALID_ADDR: bus_read_data = {31'b0, result_valid};
            default: bus_read_data = 32'bx;
        endcase
    end
end

always @(posedge pclk) begin
    //Encrypt FSM
    if(rsa_encrypt && nreset) begin
        if(encrypt_state == 5'd0) begin
            MonMult_GO <= 0;
            A <= 64'd1;
            B <= {residue_upper, residue_lower};
            encrypt_state <= 5'd1;
        end else if(encrypt_state == 5'd1) begin
            MonMult_GO <= 1;
            encrypt_state <= (MonMult_ready)? 5'd2 : 5'd1;
        end else if(encrypt_state == 5'd2) begin
            result_upper <= P[63:32];
            result_lower <= P[31:0];
            MonMult_GO <= 0;
            A <= {message_upper, message_lower};
            B <= {residue_upper, residue_lower};
            encrypt_state <= 5'd3;
        end else if(encrypt_state == 5'd3) begin
            MonMult_GO <= 1;
            encrypt_state <= (MonMult_ready)? 5'd4 : 5'd3;
        end else if(encrypt_state == 5'd4) begin
            Z_upper <= P[63:32];
            Z_lower <= P[31:0];
            MonMult_GO <= 0;
            counter <= 0;
            encrypt_state <= 5'd5;
        end else if(encrypt_state == 5'd5) begin
            A <= {result_upper, result_lower};
            B <= {Z_upper, Z_lower};
            MonMult_GO <= 1;
            encrypt_state <= (MonMult_ready)? 5'd6 : 5'd5;
        end else if(encrypt_state == 5'd6) begin
            temp_result_upper <= P[63:32];
            temp_result_lower <= P[31:0];
            MonMult_GO <= 0;
            A <= {Z_upper, Z_lower};
            B <= {Z_upper, Z_lower};
            encrypt_state <= 5'd7;
        end else if(encrypt_state == 5'd7) begin
            MonMult_GO <= 1;
            encrypt_state <= (MonMult_ready)? 5'd8 : 5'd7;
        end else if(encrypt_state == 5'd8) begin
            Z_upper <= P[63:32];
            Z_lower <= P[31:0];
            MonMult_GO <= 0;
            counter <= counter + 1;
            if((counter >= 32  && exponent_upper[counter - 32] == 1'b1) ||
               (counter < 32 && exponent_lower[counter] == 1'b1)) begin
                result_upper <= temp_result_upper;
                result_lower <= temp_result_lower;
            end else begin
                result_upper <= result_upper;
                result_lower <= result_lower;
            end
            if(counter == 63) begin
                encrypt_state <= 5'd9;
            end else begin
                encrypt_state <= 5'd5;
            end
        end else if(encrypt_state == 5'd9) begin
            A <= {result_upper, result_lower};
            B <= 64'd1;
            MonMult_GO <= 1;
            encrypt_state <= (MonMult_ready)? 5'd10 : 5'd9;
        end else if(encrypt_state == 5'd10) begin
            result_upper <= P[63:32];
            result_lower <= P[31:0];
            MonMult_GO <= 0;
            encrypt_state <= 5'd11;
        end else if(encrypt_state == 5'd11) begin
            result_valid <= 1'b1;
        end
    end else begin
        result_upper <= 32'b0;
        result_lower <= 32'b0;
        temp_result_upper <= 32'b0;
        temp_result_lower <= 32'b0;
        Z_upper <= 32'b0;
        Z_lower <= 32'b0;
        result_valid <= 1'b0;
        MonMult_GO <= 1'b0;
        A <= 64'b0;
        B <= 64'b0;
        encrypt_state <= 5'b0;
        counter <= 7'b0;
    end
end
//
//always @(posedge pclk) begin
    //if(!nreset) begin
        //bit_switch <= 1'b0;
        //message_upper <= 32'b0;
        //message_lower <= 32'b0;
        //modulus_upper <= 32'b0;
        //modulus_lower <= 32'b0;
        //exponent_upper <= 32'b0;
        //exponent_lower <= 32'b0;
        //residue_upper <= 32'b0;
        //residue_lower <= 32'b0;
        //rsa_encrypt <= 1'b0;
        //result_upper <= 32'b0;
        //result_lower <= 32'b0;
        //temp_result_upper <= 32'b0;
        //temp_result_lower <= 32'b0;
        //Z_upper <= 32'b0;
        //Z_lower <= 32'b0;
        //result_valid <= 1'b0;
        //rsa_read_out <= 32'b0;
        //MonMult_GO <= 1'b0;
        //A <= 64'b0;
        //B <= 64'b0;
        //encrypt_state <= 5'b0;
        //counter <= 7'b0;
    //end else begin 
        //bit_switch <= bit_switch_n;
        //message_upper <= message_upper_n;
        //message_lower <= message_lower_n;
        //modulus_upper <= modulus_upper_n;
        //modulus_lower <= modulus_lower_n;
        //exponent_upper <= exponent_upper_n;
        //exponent_lower <= exponent_lower_n;
        //residue_upper <= residue_upper_n;
        //residue_lower <= residue_lower_n;
        //rsa_encrypt <= rsa_encrypt_n;
        //result_upper <= result_upper_n;
        //result_lower <= result_lower_n;
        //temp_result_upper <= temp_result_upper_n;
        //temp_result_lower <= temp_result_lower_n;
        //Z_upper <= Z_upper_n;
        //Z_lower <= Z_lower_n;
        //result_valid <= result_valid_n;
        //rsa_read_out <= rsa_read_out_n;
        //MonMult_GO <= MonMult_GO_n;
        //A <= A_n;
        //B <= B_n;
        //encrypt_state <= encrypt_state_n;
        //counter <= counter_n;
    //end
//end


endmodule

