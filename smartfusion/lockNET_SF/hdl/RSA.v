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
    output reg [31:0] bus_read_data, // data to processor from I/O device (32-bits)
    output reg result_valid
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
reg bit_switch_n;

reg [31:0] message_upper; //message must be at most as large as the modulus.
reg [31:0] message_lower;
reg [31:0] message_upper_n; 
reg [31:0] message_lower_n;

reg [31:0] modulus_upper; //64-bit (8-byte) modulous (the important part).
reg [31:0] modulus_lower;
reg [31:0] modulus_upper_n;
reg [31:0] modulus_lower_n;

reg [31:0] exponent_upper; //64-bit (8-byte) exponent (65,537 is nearly universally the public exponenet, but the private exponent is normally choosen to be close to the size of the modulus).
reg [31:0] exponent_lower; 
reg [31:0] exponent_upper_n; 
reg [31:0] exponent_lower_n;
 
reg [31:0] residue_upper; //This needs to be equal to R^2 mod M, where R = 2^KEY_LENGTH and M=modulus.
reg [31:0] residue_lower;
reg [31:0] residue_upper_n;
reg [31:0] residue_lower_n;

reg rsa_encrypt = 0; //When this is set to 1, the module will begin encryption/decryption. 
reg rsa_encrypt_n;

reg [4:0] encrypt_state;
reg [4:0] encrypt_state_n;
reg [6:0] counter;
reg [6:0] counter_n;

reg [31:0] result_upper;
reg [31:0] result_lower;
reg [31:0] result_upper_n;
reg [31:0] result_lower_n;

reg [31:0] temp_result_upper;
reg [31:0] temp_result_lower;
reg [31:0] temp_result_upper_n;
reg [31:0] temp_result_lower_n;

reg [31:0] Z_upper;
reg [31:0] Z_lower;
reg [31:0] Z_upper_n;
reg [31:0] Z_lower_n;

//reg result_valid;
reg result_valid_n;

//reg [31:0] rsa_read_out;
//reg [31:0] rsa_read_out_n;

reg MonMult_GO;
reg MonMult_GO_n;
reg [63:0] A;
reg [63:0] A_n;
reg [63:0] B;
reg [63:0] B_n;
wire [63:0] P;
wire MonMult_ready;

MonMult MonMult_0(
    .pclk(pclk),
    .nreset(nreset),
    .GO(MonMult_GO_n),
    .A(A),
    .B(B),
    .M({modulus_upper, modulus_lower}),
    .P(P),
    .is_ready(MonMult_ready));


wire rsa_write;
wire rsa_read;
assign rsa_write = bus_write_en && RSA_ENABLE;
assign rsa_read = bus_read_en && RSA_ENABLE;

always @* begin
    bit_switch_n = bit_switch;
    message_upper_n = message_upper;
    message_lower_n = message_lower;
    modulus_upper_n = modulus_upper;
    modulus_lower_n = modulus_lower;
    exponent_upper_n = exponent_upper;
    exponent_lower_n = exponent_lower;
    residue_upper_n = residue_upper;
    residue_lower_n = residue_lower;
    rsa_encrypt_n = rsa_encrypt;
    result_upper_n = result_upper;
    result_lower_n = result_lower;
    temp_result_upper_n = temp_result_upper;
    temp_result_lower_n = temp_result_lower;
    Z_upper_n = Z_upper;
    Z_lower_n = Z_lower;
    //result_valid_n = result_valid;
    //rsa_read_out_n = rsa_read_out;
    //MonMult_GO_n = MonMult_GO;
    A_n = A;
    B_n = B;
    //encrypt_state_n = encrypt_state;
    counter_n = counter;

    bus_read_data = 32'bx;

    if(bus_addr == `BIT_SWITCH_ADDR & rsa_write) begin
        bit_switch_n = bus_write_data[0];
    end else if(bus_addr == `MESSAGE_ADDR & rsa_write) begin
        if(bit_switch)
            message_upper_n = bus_write_data;
        else
            message_lower_n = bus_write_data;
        rsa_encrypt_n = 1'b0;
        result_valid_n = 1'b0;
    end else if(bus_addr == `MODULUS_ADDR & rsa_write) begin
        if(bit_switch) 
            modulus_upper_n = bus_write_data;
        else
            modulus_lower_n = bus_write_data;
        rsa_encrypt_n = 1'b0;
        result_valid_n = 1'b0;
    end else if(bus_addr == `EXPONENT_ADDR & rsa_write) begin
        if(bit_switch)
            exponent_upper_n = bus_write_data;
        else
            exponent_lower_n = bus_write_data;
        rsa_encrypt_n = 1'b0;
        result_valid_n = 1'b0;
    end else if(bus_addr == `RESIDUE_ADDR & rsa_write) begin
        if(bit_switch)
            residue_upper_n = bus_write_data;
        else
            residue_lower_n = bus_write_data;
        rsa_encrypt_n = 1'b0;
        result_valid_n = 1'b0;
    end else if(bus_addr == `RSA_ENCRYPT_ADDR) begin
        if (rsa_write) begin
            rsa_encrypt_n = bus_write_data[0];
            if(rsa_encrypt == 1'b1 && bus_write_data[0] == 0) begin
                result_valid_n = 1'b0;
            end
        end
        if (rsa_read) begin
            bus_read_data = {31'b0, rsa_encrypt}; 
        end
    end else if (bus_addr == `RESULT_ADDR & rsa_read) begin
            bus_read_data = (bit_switch) ? result_upper : result_lower;
    end else if (bus_addr == `RESULT_VALID_ADDR & rsa_read) begin
            bus_read_data = {31'b0, result_valid};
    end
    /*
    //write to MMIO
    if(rsa_write) begin
        if(bus_addr == `BIT_SWITCH_ADDR) begin
            bit_switch_n = bus_write_data[0];
        end else if(bus_addr == `MESSAGE_ADDR) begin
            if(bit_switch)
                message_upper_n = bus_write_data;
            else
                message_lower_n = bus_write_data;
            rsa_encrypt_n = 1'b0;
            result_valid_n = 1'b0;
        end else if(bus_addr == `MODULUS_ADDR) begin
            if(bit_switch) 
                modulus_upper_n = bus_write_data;
            else
                modulus_lower_n = bus_write_data;
            rsa_encrypt_n = 1'b0;
            result_valid_n = 1'b0;
        end else if(bus_addr == `EXPONENT_ADDR) begin
            if(bit_switch)
                exponent_upper_n = bus_write_data;
            else
                exponent_lower_n = bus_write_data;
            rsa_encrypt_n = 1'b0;
            result_valid_n = 1'b0;
        end else if(bus_addr == `RESIDUE_ADDR) begin
            if(bit_switch)
                residue_upper_n = bus_write_data;
            else
                residue_lower_n = bus_write_data;
            rsa_encrypt_n = 1'b0;
            result_valid_n = 1'b0;
        end else if(bus_addr == `RSA_ENCRYPT_ADDR) begin
            rsa_encrypt_n = bus_write_data[0];
            if(rsa_encrypt == 1'b1 && bus_write_data[0] == 0) begin
                result_valid_n = 1'b0;
            end
        end
    end 
    
    //read from MMIO
    if(rsa_read) begin
        case(bus_addr)
            //`BIT_SWITCH_ADDR: bus_read_data = {31'b0, bit_switch};
            //`MESSAGE_ADDR: bus_read_data = (bit_switch)? message_upper : message_lower; 
            //`MODULUS_ADDR: bus_read_data = (bit_switch)? modulus_upper : modulus_lower; 
            //`EXPONENT_ADDR: bus_read_data =(bit_switch)? exponent_upper : exponent_lower; 
            //`RESIDUE_ADDR: bus_read_data = (bit_switch)? residue_upper : residue_lower; 
            `RSA_ENCRYPT_ADDR: bus_read_data = {31'b0, rsa_encrypt}; 
            `RESULT_ADDR: bus_read_data = (bit_switch)? result_upper : result_lower;
            `RESULT_VALID_ADDR: bus_read_data = {31'b0, result_valid};
        endcase
    end
    */

    MonMult_GO_n = encrypt_state[0] | (encrypt_state == 5'd4);
    result_valid_n = (encrypt_state == 5'd11);
    encrypt_state_n = encrypt_state;

    //Encrypt FSM
    if(rsa_encrypt) begin
        if(encrypt_state == 5'd0) begin
            //MonMult_GO_n = 1'b0;
            A_n = 64'd1;
            B_n = {residue_upper, residue_lower};
            encrypt_state_n = 5'd1;
        end else if(encrypt_state == 5'd1) begin
            //MonMult_GO_n = 1'b1;
            if(MonMult_ready) begin
                encrypt_state_n = 5'd2;
            end
            /*
            else begin
                encrypt_state_n = 5'd1;
            end
            */
        end else if(encrypt_state == 5'd2) begin
            result_upper_n = P[63:32];
            result_lower_n = P[31:0];
            //MonMult_GO_n = 1'b0;
            A_n = {message_upper, message_lower};
            B_n = {residue_upper, residue_lower};
            encrypt_state_n = 5'd3;
        end else if(encrypt_state == 5'd3) begin
            //MonMult_GO_n = 1'b1;
            if(MonMult_ready & MonMult_GO) begin
                encrypt_state_n = 5'd4;
            end
            /*
            else begin
                encrypt_state_n = 5'd3;
            end
            */
        end else if(encrypt_state == 5'd4) begin
            Z_upper_n = P[63:32];
            Z_lower_n = P[31:0];
            //MonMult_GO_n = 1'b0;
            counter_n = 1'b0;
            encrypt_state_n = 5'd5;
        end else if(encrypt_state == 5'd5) begin
            A_n = {result_upper, result_lower};
            B_n = {Z_upper, Z_lower};
            //MonMult_GO_n = 1'b1;
            if(MonMult_ready & MonMult_GO) begin
                encrypt_state_n = 5'd6;
            end
            /*
            else begin
                encrypt_state_n = 5'd5;
            end
            */
        end else if(encrypt_state == 5'd6) begin
            temp_result_upper_n = P[63:32];
            temp_result_lower_n = P[31:0];
            //MonMult_GO_n = 1'b0;
            A_n = {Z_upper, Z_lower};
            B_n = {Z_upper, Z_lower};
            encrypt_state_n = 5'd7;
        end else if(encrypt_state == 5'd7) begin
            //MonMult_GO_n = 1'b1;
            if(MonMult_ready & MonMult_GO) begin
                encrypt_state_n = 5'd8;
            end
            /*
            else begin
                encrypt_state_n = 5'd7;
            end
            */
        end else if(encrypt_state == 5'd8) begin
            Z_upper_n = P[63:32];
            Z_lower_n = P[31:0];
            //MonMult_GO_n = 1'b0;
            counter_n = counter + 1'b1;
            if((counter[5] & exponent_upper[counter[4:0]]) |
               (~counter[5] & exponent_lower[counter])) begin
                result_upper_n = temp_result_upper;
                result_lower_n = temp_result_lower;
            end else begin
                result_upper_n = result_upper;
                result_lower_n = result_lower;
            end
            if(counter[6]) begin
                encrypt_state_n = 5'd9;
            end else begin
                encrypt_state_n = 5'd5;
            end
        end else if(encrypt_state == 5'd9) begin
            A_n = {result_upper, result_lower};
            B_n = 64'd1;
            //MonMult_GO_n = 1'b1;
            if(MonMult_ready & MonMult_GO) begin
                encrypt_state_n = 5'd10;
            end
        end else if(encrypt_state == 5'd10) begin
            result_upper_n = P[63:32];
            result_lower_n = P[31:0];
            //MonMult_GO_n = 1'b0;
            encrypt_state_n = 5'd11;
        end
    end
end

always @(posedge pclk) begin
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
        rsa_encrypt <= 1'b0;
        result_upper <= 32'b0;
        result_lower <= 32'b0;
        temp_result_upper <= 32'b0;
        temp_result_lower <= 32'b0;
        Z_upper <= 32'b0;
        Z_lower <= 32'b0;
        result_valid <= 1'b0;
        //rsa_read_out <= 32'b0;
        MonMult_GO <= 1'b0;
        A <= 64'b0;
        B <= 64'b0;
        encrypt_state <= 5'b0;
        counter <= 7'b0;
    end else begin 
        bit_switch <= bit_switch_n;
        message_upper <= message_upper_n;
        message_lower <= message_lower_n;
        modulus_upper <= modulus_upper_n;
        modulus_lower <= modulus_lower_n;
        exponent_upper <= exponent_upper_n;
        exponent_lower <= exponent_lower_n;
        residue_upper <= residue_upper_n;
        residue_lower <= residue_lower_n;
        rsa_encrypt <= rsa_encrypt_n;
        result_upper <= result_upper_n;
        result_lower <= result_lower_n;
        temp_result_upper <= temp_result_upper_n;
        temp_result_lower <= temp_result_lower_n;
        Z_upper <= Z_upper_n;
        Z_lower <= Z_lower_n;
        result_valid <= result_valid_n;
        //rsa_read_out <= rsa_read_out_n;
        MonMult_GO <= MonMult_GO_n;
        A <= A_n;
        B <= B_n;
        encrypt_state <= encrypt_state_n;
        counter <= counter_n;
    end
end


endmodule

