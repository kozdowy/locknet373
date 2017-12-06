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
        is_ready_n = is_ready; //counter[6];
        counter_n = counter; // + ~counter[6];
//
        if (~(& counter[5:0]) & ~counter[6]) begin
            is_ready_n = 1'b0;
            counter_n = counter + 1'b1;
            P_n = (P + (A[counter] ? B : 64'b0) + ((P[0] ^ (A[counter] & B[0])) ? M : 64'b0)) >> 1;
        end else if ((& counter[5:0])) begin
            P_n = (P + (A[counter] ? B : 64'b0) + ((P[0] ^ (A[counter] & B[0])) ? M : 64'b0)) >> 1;
            if( P_n >= M ) begin
                P_n = P_n - M;
            end
            is_ready_n = 1'b0;
            counter_n = counter + 1'b1;
        end
        
        else if (counter[6]) begin
            //P_n = P;
            is_ready_n = 1'b1;
            counter_n = counter;
        end
        
    end
            
    always @(posedge pclk) begin
        if(~nreset | ~GO) begin
            P <= 64'b0;
            is_ready <= 1'b0;
            counter <= 7'b0;
        end else begin
            counter <= counter_n;
            is_ready <= is_ready_n;
            P <= P_n;
        end
    end
    
endmodule

