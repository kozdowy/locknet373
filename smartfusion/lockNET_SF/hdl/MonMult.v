///////////////////////////////////////////////////////////////////////////////////////////////////
//
// File: MonMult.v
// File history:
//      Created: 11/26/17: Skeleton and basic implementation
//      <Revision number>: <Date>: <Comments>
//
// Description: 
//
// Verilog implementation of Montgomery's modular multiplication algorithm.
//
// Takes in three operands, A, B, and M, where A and B are in the M-residue. A, B, and M are 64-digits long.
// i.e. A = A*R mod M, and B = B*R mod M where R=2^(k) where k := (length of M in bits).
//
// For this implementation, it is assumed R := 2^k. 
//
// The output P := A*B*2^-64 mod M, when is_ready := 1, and P := undefinded when is_ready := 0.
//
// Targeted device: <Family::SmartFusion> <Die::A2F200M3F> <Package::484 FBGA>
// Author: LockNet
//
/////////////////////////////////////////////////////////////////////////////////////////////////// 

module MonMult(
    input pclk,
    input nreset,
    input GO,
    input [63:0] A,
    input [63:0] B,
    input [63:0] M,
    output reg [63:0] P,
    output reg is_ready);

    reg [64:0] P_n;
    reg is_ready_n;

    reg [6:0] counter;
    reg [6:0] counter_n;

    always @* begin
        P_n = P;
        is_ready_n = is_ready;
        counter_n = counter;
        
//
        if (counter < 63) begin
            is_ready_n = 0;
            counter_n = counter + 1;
            P_n = (P + (A[counter] * B) + ((P[0] ^ (A[counter] & B[0])) * M)) >> 1;
        end else if (counter == 63) begin
            if( P >= M ) begin
                P_n = P - M;
            end else begin
                P_n = P;
            end
            is_ready_n = 0;
            counter_n = counter + 1;
        end else if (counter == 64) begin
            P_n = P;
            is_ready_n = 1;
            counter_n = counter;
        end
    end
            
    always @(posedge pclk) begin
        if(!nreset || !GO) begin
            P <= 64'b0;
            is_ready <= 0;
            counter <= 7'b0;
        end else begin
            counter <= counter_n;
            is_ready <= is_ready_n;
            P <= P_n;
        end
    end
    
endmodule

