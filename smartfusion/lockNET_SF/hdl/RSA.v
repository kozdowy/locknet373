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
`define TEMP_RESULT_ADDR 8'h28

`define RAM_READ 1'b1
`define RAM_WRITE 1'b0

module rsa(
    input pclk, // clock
    input nreset, // system reset
    input bus_write_en,
    input bus_read_en,
    input RSA_ENABLE,
    input [7:0] bus_addr, // I/O address
    input wire [31:0] bus_write_data, // data from processor to I/O device (32 bits)
    output reg [31:0] bus_read_data, // data to processor from I/O device (32-bits)
    output reg result_valid,
    output reg [31:0] RAM_DINA,
    output reg [31:0] RAM_DINB,
    output reg [6:0] RAM_ADDRA,
    output reg [6:0] RAM_ADDRB,
    output reg RAM_RWA,
    output reg RAM_RWB,
    output reg RAM_BLKA,
    output reg RAM_BLKB,
    input [31:0] RAM_DOUTA,
    input [31:0] RAM_DOUTB
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

reg [31:0] RAM_DINA_n;
reg [31:0] RAM_DINB_n;
reg [6:0] RAM_ADDRA_n;
reg [6:0] RAM_ADDRB_n;
reg RAM_RWA_n;
reg RAM_RWB_n;
reg RAM_BLKA_n;
reg RAM_BLKB_n;
reg [2:0] read_wait_counter;
reg [2:0] read_wait_counter_n;

reg [31:0] modulus_upper; //64-bit (8-byte) modulous (the important part).
reg [31:0] modulus_lower;
reg [31:0] modulus_upper_n;
reg [31:0] modulus_lower_n;
/*
reg [31:0] message_upper; //message must be at most as large as the modulus.
reg [31:0] message_lower;
reg [31:0] message_upper_n; 
reg [31:0] message_lower_n;

reg [31:0] exponent_upper; //64-bit (8-byte) exponent (65,537 is nearly universally the public exponenet, but the private exponent is normally choosen to be close to the size of the modulus).
reg [31:0] exponent_lower; 
reg [31:0] exponent_upper_n; 
reg [31:0] exponent_lower_n;
 
reg [31:0] residue_upper; //This needs to be equal to R^2 mod M, where R = 2^KEY_LENGTH and M=modulus.
reg [31:0] residue_lower;
reg [31:0] residue_upper_n;
reg [31:0] residue_lower_n;

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
*/

reg rsa_encrypt = 0; //When this is set to 1, the module will begin encryption/decryption. 
reg rsa_encrypt_n;

reg [4:0] encrypt_state;
reg [4:0] encrypt_state_n;
reg [6:0] counter;
reg [6:0] counter_n;

reg result_valid_n;

reg MonMult_GO;
reg MonMult_GO_n;
reg [63:0] A;
reg [63:0] A_n;
reg [63:0] B;
reg [63:0] B_n;
wire [65:0] P;
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

always @* begin
    bit_switch_n = bit_switch;
    //message_upper_n = message_upper;
    //message_lower_n = message_lower;
    modulus_upper_n = modulus_upper;
    modulus_lower_n = modulus_lower;
    //exponent_upper_n = exponent_upper;
    //exponent_lower_n = exponent_lower;
    //residue_upper_n = residue_upper;
    //residue_lower_n = residue_lower;
    rsa_encrypt_n = rsa_encrypt;
    //result_upper_n = result_upper;
    //result_lower_n = result_lower;
    //temp_result_upper_n = temp_result_upper;
    //temp_result_lower_n = temp_result_lower;
    //Z_upper_n = Z_upper;
    //Z_lower_n = Z_lower;
    //result_valid_n = result_valid;
    //rsa_read_out_n = rsa_read_out;
    //MonMult_GO_n = MonMult_GO;
    A_n = A;
    B_n = B;
    //encrypt_state_n = encrypt_state;
    counter_n = counter;
    RAM_DINA_n = RAM_DINA;//32'b0;
    RAM_DINB_n = RAM_DINB;//32'b0;
    RAM_ADDRA_n = RAM_ADDRA;//7'b0;
    RAM_ADDRB_n = RAM_ADDRB;//7'b0;
    RAM_RWA_n = RAM_RWA;//1'b0;
    RAM_RWB_n = RAM_RWB;//1'b0;
    RAM_BLKA_n = RAM_BLKA;//1'b1;
    RAM_BLKB_n = RAM_BLKB;//1'b1;

    bus_read_data = 32'bx;

    if(bus_addr == `MODULUS_ADDR && rsa_write) begin
        if(bit_switch)
            modulus_upper_n = bus_write_data;
        else
            modulus_lower_n = bus_write_data;
    end

    if(bus_addr == `BIT_SWITCH_ADDR & rsa_write) begin
        bit_switch_n = bus_write_data[0];
    end else if((bus_addr == `MESSAGE_ADDR ||
                 bus_addr == `MODULUS_ADDR ||
                 bus_addr == `EXPONENT_ADDR ||
                 bus_addr == `RESIDUE_ADDR)
                && rsa_write) begin
        if(bit_switch) begin
            RAM_DINB_n = bus_write_data;
            RAM_BLKB_n = 1'b0;
            RAM_ADDRB_n = bus_addr[6:0] + 1;
            RAM_RWB_n = `RAM_WRITE;
        end else begin
            RAM_DINA_n = bus_write_data;
            RAM_BLKA_n = 1'b0;
            RAM_ADDRA_n = bus_addr[6:0];
            RAM_RWA_n = `RAM_WRITE;
        end
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
            // preserve output of final monmult and return that
            bus_read_data = (bit_switch) ? P[63:32] : P[31:0];
    end else if (bus_addr == `RESULT_VALID_ADDR & rsa_read) begin
            bus_read_data = {31'b0, result_valid};
    end

    MonMult_GO_n = MonMult_GO;
    //result_valid_n = (encrypt_state == 5'd11);
    result_valid_n = (encrypt_state == 5'd21);
    encrypt_state_n = encrypt_state;

    //Encrypt FSM
    if(rsa_encrypt) begin
        if(encrypt_state == 5'd0) begin
            // read residue in to A_n
            RAM_ADDRA_n = `RESIDUE_ADDR;
            RAM_ADDRB_n = `RESIDUE_ADDR + 1;
            RAM_RWA_n = `RAM_READ;
            RAM_RWB_n = `RAM_READ;
            RAM_BLKA_n = 1'b0;
            RAM_BLKB_n = 1'b0;
            MonMult_GO_n = 1'b0;
            read_wait_counter_n = 3'd0;
            //A_n = {residue_upper, residue_lower};
            B_n = 64'd1;
            encrypt_state_n = 5'd1;
        end else if(encrypt_state == 5'd1) begin
            if(read_wait_counter == 3'd5) begin
                A_n = {RAM_DOUTB, RAM_DOUTA};
                MonMult_GO_n = 1'b1;
                encrypt_state_n = 5'd2;
            end else begin
                read_wait_counter_n = read_wait_counter + 1;
            end
        end else if(encrypt_state == 5'd2) begin
            //MonMult_GO_n = 1'b1;
            if(MonMult_ready) begin
                encrypt_state_n = 5'd3;
            end
        end else if(encrypt_state == 5'd3) begin
            RAM_ADDRA_n = `RESULT_ADDR;
            RAM_ADDRB_n = `RESULT_ADDR + 1;
            RAM_RWA_n = `RAM_WRITE;
            RAM_RWB_n = `RAM_WRITE;
            RAM_BLKA_n = 1'b0;
            RAM_BLKB_n = 1'b0;
            RAM_DINA_n = P[31:0];
            RAM_DINB_n = P[63:32];
            MonMult_GO_n = 1'b0;
            encrypt_state_n = 5'd4;
        end else if(encrypt_state == 5'd4) begin
            // read message in to B
            RAM_ADDRA_n = `MESSAGE_ADDR;
            RAM_ADDRB_n = `MESSAGE_ADDR + 1;
            RAM_RWA_n = `RAM_READ;
            RAM_RWB_n = `RAM_READ;
            RAM_BLKA_n = 1'b0;
            RAM_BLKB_n = 1'b0;
            read_wait_counter_n = 3'd0;
            encrypt_state_n = 5'd5;
        end else if(encrypt_state == 5'd5) begin
            if(read_wait_counter == 3'd5) begin
                B_n = {RAM_DOUTB, RAM_DOUTA};
                MonMult_GO_n = 1'b1;
                encrypt_state_n = 5'd6;
            end else begin
                read_wait_counter_n = read_wait_counter + 1;
            end
        end else if(encrypt_state == 5'd6) begin
            if(MonMult_ready) begin
                encrypt_state_n = 5'd7;
            end
        end else if(encrypt_state == 5'd7) begin
            MonMult_GO_n = 1'b0;
            // read result in to A
            RAM_ADDRA_n = `RESULT_ADDR;
            RAM_ADDRB_n = `RESULT_ADDR + 1;
            RAM_RWA_n = `RAM_READ;
            RAM_RWB_n = `RAM_READ;
            RAM_BLKA_n = 1'b0;
            RAM_BLKB_n = 1'b0;
            //A_n = {result_upper, result_lower};
            B_n = P[63:0];
            read_wait_counter_n = 3'd0;
            counter_n = 1'b0;
            encrypt_state_n = 5'd8;
        end else if(encrypt_state == 5'd8) begin
            if(read_wait_counter == 3'd5) begin
                A_n = {RAM_DOUTB, RAM_DOUTA};
                MonMult_GO_n = 1'b1;
                encrypt_state_n = 5'd9;
            end else begin
                read_wait_counter_n = read_wait_counter + 1;
            end
        end else if(encrypt_state == 5'd9) begin
            if(MonMult_ready) begin
                encrypt_state_n = 5'd10;
            end
        end else if(encrypt_state == 5'd10) begin
            RAM_ADDRA_n = `TEMP_RESULT_ADDR;
            RAM_ADDRB_n = `TEMP_RESULT_ADDR + 1;
            RAM_RWA_n = `RAM_WRITE;
            RAM_RWB_n = `RAM_WRITE;
            RAM_BLKA_n = 1'b0;
            RAM_BLKB_n = 1'b0;
            RAM_DINA_n = P[31:0];
            RAM_DINB_n = P[63:32];
            MonMult_GO_n = 1'b0;
            A_n = B;
            encrypt_state_n = 5'd11;
        end else if(encrypt_state == 5'd11) begin
            MonMult_GO_n = 1'b1;
            if(MonMult_ready & MonMult_GO) begin
                encrypt_state_n = 5'd12;
            end
        end else if(encrypt_state == 5'd12) begin
            B_n = P[63:0];
            MonMult_GO_n = 1'b0;
            RAM_ADDRA_n = `EXPONENT_ADDR;
            RAM_ADDRB_n = `EXPONENT_ADDR + 1;
            RAM_RWA_n = `RAM_READ;
            RAM_RWB_n = `RAM_READ;
            RAM_BLKA_n = 1'b0;
            RAM_BLKB_n = 1'b0;
            read_wait_counter_n = 3'd0;
            encrypt_state_n = 5'd13;
        end else if(encrypt_state == 5'd13) begin
            if(read_wait_counter == 3'd5) begin
                encrypt_state_n = 5'd14;
            end else begin
                read_wait_counter_n = read_wait_counter + 1;
            end
        end else if(encrypt_state == 5'd14) begin
            counter_n = counter + 1'b1;
            if((counter[5] & RAM_DOUTB[counter[4:0]]) |
               (~counter[5] & RAM_DOUTA[counter[4:0]])) begin
                RAM_ADDRA_n = `TEMP_RESULT_ADDR;
                RAM_ADDRB_n = `TEMP_RESULT_ADDR + 1;
                RAM_RWA_n = `RAM_READ;
                RAM_RWB_n = `RAM_READ;
                RAM_BLKA_n = 1'b0;
                RAM_BLKB_n = 1'b0;
                read_wait_counter_n = 3'd0;
                encrypt_state_n = 5'd15;
            end else begin
                encrypt_state_n = 5'd17;
            end
        end else if(encrypt_state == 5'd15) begin
            if(read_wait_counter == 3'd5) begin
                RAM_DINA_n = RAM_DOUTA;
                RAM_DINB_n = RAM_DOUTB;
                encrypt_state_n = 5'd16;
            end else begin
                read_wait_counter_n = read_wait_counter + 1;
            end
        end else if(encrypt_state == 5'd16) begin
            RAM_ADDRA_n = `RESULT_ADDR;
            RAM_ADDRB_n = `RESULT_ADDR + 1;
            RAM_RWA_n = `RAM_WRITE;
            RAM_RWB_n = `RAM_WRITE;
            RAM_BLKA_n = 1'b0;
            RAM_BLKB_n = 1'b0;
            //RAM_DINA_n = RAM_DOUTA;
            //RAM_DINB_n = RAM_DOUTB;
            encrypt_state_n = 5'd17;
        end else if(encrypt_state == 5'd17) begin
            RAM_ADDRA_n = `RESULT_ADDR;
            RAM_ADDRB_n = `RESULT_ADDR + 1;
            RAM_RWA_n = `RAM_READ;
            RAM_RWB_n = `RAM_READ;
            RAM_BLKA_n = 1'b0;
            RAM_BLKB_n = 1'b0;
            read_wait_counter_n = 3'd0;
            encrypt_state_n = 5'd18;
        end else if(encrypt_state == 5'd18) begin
            if(read_wait_counter == 3'd5) begin
                encrypt_state_n = 5'd19;
            end else begin
                read_wait_counter_n = read_wait_counter + 1;
            end
        end else if(encrypt_state == 5'd19) begin
            if(counter[6]) begin
                A_n = 64'd1;
                B_n = {RAM_DOUTB, RAM_DOUTA};
                encrypt_state_n = 5'd20;
                MonMult_GO_n = 1'b1;
            end else begin
                A_n = {RAM_DOUTB, RAM_DOUTA};
                //B_n is always set to Z;
                encrypt_state_n = 5'd9;
                MonMult_GO_n = 1'b1;
            end
        end else if(encrypt_state == 5'd20) begin
            if(MonMult_ready) begin
                encrypt_state_n = 5'd21;
            end
        end else if(encrypt_state == 5'd21) begin
            //
            //result_upper_n = P[63:32];
            //result_lower_n = P[31:0];
            encrypt_state_n = 5'd21;
        end
    end
end

always @(posedge pclk) begin
    if(!nreset) begin
        bit_switch <= 1'b0;
        //message_upper <= 32'b0;
        //message_lower <= 32'b0;
        modulus_upper <= 32'b0;
        modulus_lower <= 32'b0;
        //exponent_upper <= 32'b0;
        //exponent_lower <= 32'b0;
        //residue_upper <= 32'b0;
        //residue_lower <= 32'b0;
        rsa_encrypt <= 1'b0;
        //result_upper <= 32'b0;
        //result_lower <= 32'b0;
        //temp_result_upper <= 32'b0;
        //temp_result_lower <= 32'b0;
        //Z_upper <= 32'b0;
        //Z_lower <= 32'b0;
        result_valid <= 1'b0;
        MonMult_GO <= 1'b0;
        A <= 64'b0;
        B <= 64'b0;
        encrypt_state <= 5'b0;
        counter <= 7'b0;
    end else begin 
        RAM_DINA <= RAM_DINA_n;
        RAM_DINB <= RAM_DINB_n;
        RAM_ADDRA <= RAM_ADDRA_n;
        RAM_ADDRB <= RAM_ADDRB_n;
        RAM_RWA <= RAM_RWA_n;
        RAM_RWB <= RAM_RWB_n;
        RAM_BLKA <= RAM_BLKA_n;
        RAM_BLKB <= RAM_BLKB_n;
        bit_switch <= bit_switch_n;
        read_wait_counter <= read_wait_counter_n;
        //message_upper <= message_upper_n;
        //message_lower <= message_lower_n;
        modulus_upper <= modulus_upper_n;
        modulus_lower <= modulus_lower_n;
        //exponent_upper <= exponent_upper_n;
        //exponent_lower <= exponent_lower_n;
        //residue_upper <= residue_upper_n;
        //residue_lower <= residue_lower_n;
        rsa_encrypt <= rsa_encrypt_n;
        //result_upper <= result_upper_n;
        //result_lower <= result_lower_n;
        //temp_result_upper <= temp_result_upper_n;
        //temp_result_lower <= temp_result_lower_n;
        //Z_upper <= Z_upper_n;
        //Z_lower <= Z_lower_n;
        result_valid <= result_valid_n;
        MonMult_GO <= MonMult_GO_n;
        A <= A_n;
        B <= B_n;
        encrypt_state <= encrypt_state_n;
        counter <= counter_n;
    end
end


endmodule

