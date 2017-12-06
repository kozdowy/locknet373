`timescale 1 ns/100 ps
// Version: v11.8 11.8.0.26


module RSA_RAM(
       WD,
       RD,
       WEN,
       REN,
       WADDR,
       RADDR,
       RWCLK,
       RESET
    );
input  [31:0] WD;
output [31:0] RD;
input  WEN;
input  REN;
input  [7:0] WADDR;
input  [7:0] RADDR;
input  RWCLK;
input  RESET;

    wire VCC, GND;
    wire GND_power_net1;
    wire VCC_power_net1;
    assign GND = GND_power_net1;
    assign VCC = VCC_power_net1;
    
    RAM512X18 #( .MEMORYFILE("RSA_RAM_R0C0.mem") )  RSA_RAM_R0C0 (
        .RADDR8(GND), .RADDR7(RADDR[7]), .RADDR6(RADDR[6]), .RADDR5(
        RADDR[5]), .RADDR4(RADDR[4]), .RADDR3(RADDR[3]), .RADDR2(
        RADDR[2]), .RADDR1(RADDR[1]), .RADDR0(RADDR[0]), .WADDR8(GND), 
        .WADDR7(WADDR[7]), .WADDR6(WADDR[6]), .WADDR5(WADDR[5]), 
        .WADDR4(WADDR[4]), .WADDR3(WADDR[3]), .WADDR2(WADDR[2]), 
        .WADDR1(WADDR[1]), .WADDR0(WADDR[0]), .WD17(GND), .WD16(WD[15])
        , .WD15(WD[14]), .WD14(WD[13]), .WD13(WD[12]), .WD12(WD[11]), 
        .WD11(WD[10]), .WD10(WD[9]), .WD9(WD[8]), .WD8(GND), .WD7(
        WD[7]), .WD6(WD[6]), .WD5(WD[5]), .WD4(WD[4]), .WD3(WD[3]), 
        .WD2(WD[2]), .WD1(WD[1]), .WD0(WD[0]), .RW0(GND), .RW1(VCC), 
        .WW0(GND), .WW1(VCC), .PIPE(VCC), .REN(REN), .WEN(WEN), .RCLK(
        RWCLK), .WCLK(RWCLK), .RESET(RESET), .RD17(), .RD16(RD[15]), 
        .RD15(RD[14]), .RD14(RD[13]), .RD13(RD[12]), .RD12(RD[11]), 
        .RD11(RD[10]), .RD10(RD[9]), .RD9(RD[8]), .RD8(), .RD7(RD[7]), 
        .RD6(RD[6]), .RD5(RD[5]), .RD4(RD[4]), .RD3(RD[3]), .RD2(RD[2])
        , .RD1(RD[1]), .RD0(RD[0]));
    RAM512X18 #( .MEMORYFILE("RSA_RAM_R0C1.mem") )  RSA_RAM_R0C1 (
        .RADDR8(GND), .RADDR7(RADDR[7]), .RADDR6(RADDR[6]), .RADDR5(
        RADDR[5]), .RADDR4(RADDR[4]), .RADDR3(RADDR[3]), .RADDR2(
        RADDR[2]), .RADDR1(RADDR[1]), .RADDR0(RADDR[0]), .WADDR8(GND), 
        .WADDR7(WADDR[7]), .WADDR6(WADDR[6]), .WADDR5(WADDR[5]), 
        .WADDR4(WADDR[4]), .WADDR3(WADDR[3]), .WADDR2(WADDR[2]), 
        .WADDR1(WADDR[1]), .WADDR0(WADDR[0]), .WD17(GND), .WD16(WD[31])
        , .WD15(WD[30]), .WD14(WD[29]), .WD13(WD[28]), .WD12(WD[27]), 
        .WD11(WD[26]), .WD10(WD[25]), .WD9(WD[24]), .WD8(GND), .WD7(
        WD[23]), .WD6(WD[22]), .WD5(WD[21]), .WD4(WD[20]), .WD3(WD[19])
        , .WD2(WD[18]), .WD1(WD[17]), .WD0(WD[16]), .RW0(GND), .RW1(
        VCC), .WW0(GND), .WW1(VCC), .PIPE(VCC), .REN(REN), .WEN(WEN), 
        .RCLK(RWCLK), .WCLK(RWCLK), .RESET(RESET), .RD17(), .RD16(
        RD[31]), .RD15(RD[30]), .RD14(RD[29]), .RD13(RD[28]), .RD12(
        RD[27]), .RD11(RD[26]), .RD10(RD[25]), .RD9(RD[24]), .RD8(), 
        .RD7(RD[23]), .RD6(RD[22]), .RD5(RD[21]), .RD4(RD[20]), .RD3(
        RD[19]), .RD2(RD[18]), .RD1(RD[17]), .RD0(RD[16]));
    GND GND_power_inst1 (.Y(GND_power_net1));
    VCC VCC_power_inst1 (.Y(VCC_power_net1));
    
endmodule

// _Disclaimer: Please leave the following comments in the file, they are for internal purposes only._


// _GEN_File_Contents_

// Version:11.8.0.26
// ACTGENU_CALL:1
// BATCH:T
// FAM:PA3SOC2
// OUTFORMAT:Verilog
// LPMTYPE:LPM_RAM
// LPM_HINT:TWO
// INSERT_PAD:NO
// INSERT_IOREG:NO
// GEN_BHV_VHDL_VAL:F
// GEN_BHV_VERILOG_VAL:F
// MGNTIMER:F
// MGNCMPL:T
// DESDIR:N:/Git/locknet373/smartfusion/lockNET_SF/smartgen\RSA_RAM
// GEN_BEHV_MODULE:F
// SMARTGEN_DIE:IP4X3M1
// SMARTGEN_PACKAGE:fg484
// AGENIII_IS_SUBPROJECT_LIBERO:T
// WWIDTH:32
// WDEPTH:256
// RWIDTH:32
// RDEPTH:256
// CLKS:1
// CLOCK_PN:RWCLK
// RESET_PN:RESET
// RESET_POLARITY:0
// INIT_RAM:T
// DEFAULT_WORD:0x00000000
// CASCADE:0
// WCLK_EDGE:RISE
// PMODE2:1
// DATA_IN_PN:WD
// WADDRESS_PN:WADDR
// WE_PN:WEN
// DATA_OUT_PN:RD
// RADDRESS_PN:RADDR
// RE_PN:REN
// WE_POLARITY:0
// RE_POLARITY:0
// PTYPE:1

// _End_Comments_

