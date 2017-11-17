//////////////////////////////////////////////////////////////////////
// Created by SmartDesign Fri Nov 17 12:48:04 2017
// Version: v11.8 11.8.0.26
//////////////////////////////////////////////////////////////////////

`timescale 1ns / 100ps

// lockNET_SF
module lockNET_SF(
    // Inputs
    GPIO_1_IN,
    MAC_0_CRSDV,
    MAC_0_RXD,
    MAC_0_RXER,
    MSS_RESET_N,
    PIN_NFC_IRQ,
    UART_0_RXD,
    // Outputs
    M2F_GPO_0,
    MAC_0_MDC,
    MAC_0_TXD,
    MAC_0_TXEN,
    NP_OUT,
    SERVO_OUT,
    UART_0_TXD,
    // Inouts
    I2C_1_SCL,
    I2C_1_SDA,
    MAC_0_MDIO
);

//--------------------------------------------------------------------
// Input
//--------------------------------------------------------------------
input        GPIO_1_IN;
input        MAC_0_CRSDV;
input  [1:0] MAC_0_RXD;
input        MAC_0_RXER;
input        MSS_RESET_N;
input        PIN_NFC_IRQ;
input        UART_0_RXD;
//--------------------------------------------------------------------
// Output
//--------------------------------------------------------------------
output       M2F_GPO_0;
output       MAC_0_MDC;
output [1:0] MAC_0_TXD;
output       MAC_0_TXEN;
output       NP_OUT;
output       SERVO_OUT;
output       UART_0_TXD;
//--------------------------------------------------------------------
// Inout
//--------------------------------------------------------------------
inout        I2C_1_SCL;
inout        I2C_1_SDA;
inout        MAC_0_MDIO;
//--------------------------------------------------------------------
// Nets
//--------------------------------------------------------------------
wire          apb3_interface_0_FABINT;
wire          CoreAPB3_0_APBmslave0_PENABLE;
wire   [31:0] CoreAPB3_0_APBmslave0_PRDATA;
wire          CoreAPB3_0_APBmslave0_PREADY;
wire          CoreAPB3_0_APBmslave0_PSELx;
wire          CoreAPB3_0_APBmslave0_PSLVERR;
wire   [31:0] CoreAPB3_0_APBmslave0_PWDATA;
wire          CoreAPB3_0_APBmslave0_PWRITE;
wire          GPIO_1_IN;
wire          I2C_1_SCL;
wire          I2C_1_SDA;
wire          lockNET_SF_MSS_0_FAB_CLK;
wire          lockNET_SF_MSS_0_M2F_RESET_N;
wire          lockNET_SF_MSS_0_MSS_MASTER_APB_PENABLE;
wire   [31:0] lockNET_SF_MSS_0_MSS_MASTER_APB_PRDATA;
wire          lockNET_SF_MSS_0_MSS_MASTER_APB_PREADY;
wire          lockNET_SF_MSS_0_MSS_MASTER_APB_PSELx;
wire          lockNET_SF_MSS_0_MSS_MASTER_APB_PSLVERR;
wire   [31:0] lockNET_SF_MSS_0_MSS_MASTER_APB_PWDATA;
wire          lockNET_SF_MSS_0_MSS_MASTER_APB_PWRITE;
wire          M2F_GPO_0_net_0;
wire          MAC_0_CRSDV;
wire          MAC_0_MDC_net_0;
wire          MAC_0_MDIO;
wire   [1:0]  MAC_0_RXD;
wire          MAC_0_RXER;
wire   [1:0]  MAC_0_TXD_net_0;
wire          MAC_0_TXEN_net_0;
wire          MSS_RESET_N;
wire          NP_OUT_net_0;
wire          PIN_NFC_IRQ;
wire          SERVO_OUT_net_0;
wire          UART_0_RXD;
wire          UART_0_TXD_net_0;
wire          UART_0_TXD_net_1;
wire          MAC_0_TXEN_net_1;
wire          MAC_0_MDC_net_1;
wire          M2F_GPO_0_net_1;
wire          NP_OUT_net_1;
wire          SERVO_OUT_net_1;
wire   [1:0]  MAC_0_TXD_net_1;
//--------------------------------------------------------------------
// TiedOff Nets
//--------------------------------------------------------------------
wire          GND_net;
wire          VCC_net;
wire   [31:0] IADDR_const_net_0;
wire   [31:0] PRDATAS1_const_net_0;
wire   [31:0] PRDATAS2_const_net_0;
wire   [31:0] PRDATAS3_const_net_0;
wire   [31:0] PRDATAS4_const_net_0;
wire   [31:0] PRDATAS5_const_net_0;
wire   [31:0] PRDATAS6_const_net_0;
wire   [31:0] PRDATAS7_const_net_0;
wire   [31:0] PRDATAS8_const_net_0;
wire   [31:0] PRDATAS9_const_net_0;
wire   [31:0] PRDATAS10_const_net_0;
wire   [31:0] PRDATAS11_const_net_0;
wire   [31:0] PRDATAS12_const_net_0;
wire   [31:0] PRDATAS13_const_net_0;
wire   [31:0] PRDATAS14_const_net_0;
wire   [31:0] PRDATAS15_const_net_0;
wire   [31:0] PRDATAS16_const_net_0;
//--------------------------------------------------------------------
// Bus Interface Nets Declarations - Unequal Pin Widths
//--------------------------------------------------------------------
wire   [31:0] CoreAPB3_0_APBmslave0_PADDR;
wire   [7:0]  CoreAPB3_0_APBmslave0_PADDR_0_7to0;
wire   [7:0]  CoreAPB3_0_APBmslave0_PADDR_0;
wire   [31:20]lockNET_SF_MSS_0_MSS_MASTER_APB_PADDR_0_31to20;
wire   [19:0] lockNET_SF_MSS_0_MSS_MASTER_APB_PADDR_0_19to0;
wire   [31:0] lockNET_SF_MSS_0_MSS_MASTER_APB_PADDR_0;
wire   [19:0] lockNET_SF_MSS_0_MSS_MASTER_APB_PADDR;
//--------------------------------------------------------------------
// Constant assignments
//--------------------------------------------------------------------
assign GND_net               = 1'b0;
assign VCC_net               = 1'b1;
assign IADDR_const_net_0     = 32'h00000000;
assign PRDATAS1_const_net_0  = 32'h00000000;
assign PRDATAS2_const_net_0  = 32'h00000000;
assign PRDATAS3_const_net_0  = 32'h00000000;
assign PRDATAS4_const_net_0  = 32'h00000000;
assign PRDATAS5_const_net_0  = 32'h00000000;
assign PRDATAS6_const_net_0  = 32'h00000000;
assign PRDATAS7_const_net_0  = 32'h00000000;
assign PRDATAS8_const_net_0  = 32'h00000000;
assign PRDATAS9_const_net_0  = 32'h00000000;
assign PRDATAS10_const_net_0 = 32'h00000000;
assign PRDATAS11_const_net_0 = 32'h00000000;
assign PRDATAS12_const_net_0 = 32'h00000000;
assign PRDATAS13_const_net_0 = 32'h00000000;
assign PRDATAS14_const_net_0 = 32'h00000000;
assign PRDATAS15_const_net_0 = 32'h00000000;
assign PRDATAS16_const_net_0 = 32'h00000000;
//--------------------------------------------------------------------
// Top level output port assignments
//--------------------------------------------------------------------
assign UART_0_TXD_net_1 = UART_0_TXD_net_0;
assign UART_0_TXD       = UART_0_TXD_net_1;
assign MAC_0_TXEN_net_1 = MAC_0_TXEN_net_0;
assign MAC_0_TXEN       = MAC_0_TXEN_net_1;
assign MAC_0_MDC_net_1  = MAC_0_MDC_net_0;
assign MAC_0_MDC        = MAC_0_MDC_net_1;
assign M2F_GPO_0_net_1  = M2F_GPO_0_net_0;
assign M2F_GPO_0        = M2F_GPO_0_net_1;
assign NP_OUT_net_1     = NP_OUT_net_0;
assign NP_OUT           = NP_OUT_net_1;
assign SERVO_OUT_net_1  = SERVO_OUT_net_0;
assign SERVO_OUT        = SERVO_OUT_net_1;
assign MAC_0_TXD_net_1  = MAC_0_TXD_net_0;
assign MAC_0_TXD[1:0]   = MAC_0_TXD_net_1;
//--------------------------------------------------------------------
// Bus Interface Nets Assignments - Unequal Pin Widths
//--------------------------------------------------------------------
assign CoreAPB3_0_APBmslave0_PADDR_0_7to0 = CoreAPB3_0_APBmslave0_PADDR[7:0];
assign CoreAPB3_0_APBmslave0_PADDR_0 = { CoreAPB3_0_APBmslave0_PADDR_0_7to0 };

assign lockNET_SF_MSS_0_MSS_MASTER_APB_PADDR_0_31to20 = 12'h0;
assign lockNET_SF_MSS_0_MSS_MASTER_APB_PADDR_0_19to0 = lockNET_SF_MSS_0_MSS_MASTER_APB_PADDR[19:0];
assign lockNET_SF_MSS_0_MSS_MASTER_APB_PADDR_0 = { lockNET_SF_MSS_0_MSS_MASTER_APB_PADDR_0_31to20, lockNET_SF_MSS_0_MSS_MASTER_APB_PADDR_0_19to0 };

//--------------------------------------------------------------------
// Component instances
//--------------------------------------------------------------------
//--------apb3_interface
apb3_interface apb3_interface_0(
        // Inputs
        .PCLK        ( lockNET_SF_MSS_0_FAB_CLK ),
        .PENABLE     ( CoreAPB3_0_APBmslave0_PENABLE ),
        .PSEL        ( CoreAPB3_0_APBmslave0_PSELx ),
        .PRESETN     ( lockNET_SF_MSS_0_M2F_RESET_N ),
        .PWRITE      ( CoreAPB3_0_APBmslave0_PWRITE ),
        .PADDR       ( CoreAPB3_0_APBmslave0_PADDR_0 ),
        .PWDATA      ( CoreAPB3_0_APBmslave0_PWDATA ),
        .PIN_NFC_IRQ ( PIN_NFC_IRQ ),
        // Outputs
        .PREADY      ( CoreAPB3_0_APBmslave0_PREADY ),
        .PSLVERR     ( CoreAPB3_0_APBmslave0_PSLVERR ),
        .PRDATA      ( CoreAPB3_0_APBmslave0_PRDATA ),
        .NP_OUT      ( NP_OUT_net_0 ),
        .SERVO_OUT   ( SERVO_OUT_net_0 ),
        .FABINT      ( apb3_interface_0_FABINT ) 
        );

//--------CoreAPB3   -   Actel:DirectCore:CoreAPB3:4.1.100
CoreAPB3 #( 
        .APB_DWIDTH      ( 32 ),
        .APBSLOT0ENABLE  ( 1 ),
        .APBSLOT1ENABLE  ( 0 ),
        .APBSLOT2ENABLE  ( 0 ),
        .APBSLOT3ENABLE  ( 0 ),
        .APBSLOT4ENABLE  ( 0 ),
        .APBSLOT5ENABLE  ( 0 ),
        .APBSLOT6ENABLE  ( 0 ),
        .APBSLOT7ENABLE  ( 0 ),
        .APBSLOT8ENABLE  ( 0 ),
        .APBSLOT9ENABLE  ( 0 ),
        .APBSLOT10ENABLE ( 0 ),
        .APBSLOT11ENABLE ( 0 ),
        .APBSLOT12ENABLE ( 0 ),
        .APBSLOT13ENABLE ( 0 ),
        .APBSLOT14ENABLE ( 0 ),
        .APBSLOT15ENABLE ( 0 ),
        .FAMILY          ( 18 ),
        .IADDR_OPTION    ( 0 ),
        .MADDR_BITS      ( 12 ),
        .SC_0            ( 0 ),
        .SC_1            ( 0 ),
        .SC_2            ( 0 ),
        .SC_3            ( 0 ),
        .SC_4            ( 0 ),
        .SC_5            ( 0 ),
        .SC_6            ( 0 ),
        .SC_7            ( 0 ),
        .SC_8            ( 0 ),
        .SC_9            ( 0 ),
        .SC_10           ( 0 ),
        .SC_11           ( 0 ),
        .SC_12           ( 0 ),
        .SC_13           ( 0 ),
        .SC_14           ( 0 ),
        .SC_15           ( 0 ),
        .UPR_NIBBLE_POSN ( 2 ) )
CoreAPB3_0(
        // Inputs
        .PRESETN    ( GND_net ), // tied to 1'b0 from definition
        .PCLK       ( GND_net ), // tied to 1'b0 from definition
        .PWRITE     ( lockNET_SF_MSS_0_MSS_MASTER_APB_PWRITE ),
        .PENABLE    ( lockNET_SF_MSS_0_MSS_MASTER_APB_PENABLE ),
        .PSEL       ( lockNET_SF_MSS_0_MSS_MASTER_APB_PSELx ),
        .PREADYS0   ( CoreAPB3_0_APBmslave0_PREADY ),
        .PSLVERRS0  ( CoreAPB3_0_APBmslave0_PSLVERR ),
        .PREADYS1   ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS1  ( GND_net ), // tied to 1'b0 from definition
        .PREADYS2   ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS2  ( GND_net ), // tied to 1'b0 from definition
        .PREADYS3   ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS3  ( GND_net ), // tied to 1'b0 from definition
        .PREADYS4   ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS4  ( GND_net ), // tied to 1'b0 from definition
        .PREADYS5   ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS5  ( GND_net ), // tied to 1'b0 from definition
        .PREADYS6   ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS6  ( GND_net ), // tied to 1'b0 from definition
        .PREADYS7   ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS7  ( GND_net ), // tied to 1'b0 from definition
        .PREADYS8   ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS8  ( GND_net ), // tied to 1'b0 from definition
        .PREADYS9   ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS9  ( GND_net ), // tied to 1'b0 from definition
        .PREADYS10  ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS10 ( GND_net ), // tied to 1'b0 from definition
        .PREADYS11  ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS11 ( GND_net ), // tied to 1'b0 from definition
        .PREADYS12  ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS12 ( GND_net ), // tied to 1'b0 from definition
        .PREADYS13  ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS13 ( GND_net ), // tied to 1'b0 from definition
        .PREADYS14  ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS14 ( GND_net ), // tied to 1'b0 from definition
        .PREADYS15  ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS15 ( GND_net ), // tied to 1'b0 from definition
        .PREADYS16  ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS16 ( GND_net ), // tied to 1'b0 from definition
        .PADDR      ( lockNET_SF_MSS_0_MSS_MASTER_APB_PADDR_0 ),
        .PWDATA     ( lockNET_SF_MSS_0_MSS_MASTER_APB_PWDATA ),
        .PRDATAS0   ( CoreAPB3_0_APBmslave0_PRDATA ),
        .PRDATAS1   ( PRDATAS1_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS2   ( PRDATAS2_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS3   ( PRDATAS3_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS4   ( PRDATAS4_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS5   ( PRDATAS5_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS6   ( PRDATAS6_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS7   ( PRDATAS7_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS8   ( PRDATAS8_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS9   ( PRDATAS9_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS10  ( PRDATAS10_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS11  ( PRDATAS11_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS12  ( PRDATAS12_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS13  ( PRDATAS13_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS14  ( PRDATAS14_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS15  ( PRDATAS15_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS16  ( PRDATAS16_const_net_0 ), // tied to 32'h00000000 from definition
        .IADDR      ( IADDR_const_net_0 ), // tied to 32'h00000000 from definition
        // Outputs
        .PREADY     ( lockNET_SF_MSS_0_MSS_MASTER_APB_PREADY ),
        .PSLVERR    ( lockNET_SF_MSS_0_MSS_MASTER_APB_PSLVERR ),
        .PWRITES    ( CoreAPB3_0_APBmslave0_PWRITE ),
        .PENABLES   ( CoreAPB3_0_APBmslave0_PENABLE ),
        .PSELS0     ( CoreAPB3_0_APBmslave0_PSELx ),
        .PSELS1     (  ),
        .PSELS2     (  ),
        .PSELS3     (  ),
        .PSELS4     (  ),
        .PSELS5     (  ),
        .PSELS6     (  ),
        .PSELS7     (  ),
        .PSELS8     (  ),
        .PSELS9     (  ),
        .PSELS10    (  ),
        .PSELS11    (  ),
        .PSELS12    (  ),
        .PSELS13    (  ),
        .PSELS14    (  ),
        .PSELS15    (  ),
        .PSELS16    (  ),
        .PRDATA     ( lockNET_SF_MSS_0_MSS_MASTER_APB_PRDATA ),
        .PADDRS     ( CoreAPB3_0_APBmslave0_PADDR ),
        .PWDATAS    ( CoreAPB3_0_APBmslave0_PWDATA ) 
        );

//--------lockNET_SF_MSS
lockNET_SF_MSS lockNET_SF_MSS_0(
        // Inputs
        .MSS_RESET_N ( MSS_RESET_N ),
        .UART_0_RXD  ( UART_0_RXD ),
        .MSSPREADY   ( lockNET_SF_MSS_0_MSS_MASTER_APB_PREADY ),
        .MSSPSLVERR  ( lockNET_SF_MSS_0_MSS_MASTER_APB_PSLVERR ),
        .MAC_0_CRSDV ( MAC_0_CRSDV ),
        .MAC_0_RXER  ( MAC_0_RXER ),
        .GPIO_1_IN   ( GPIO_1_IN ),
        .MSSPRDATA   ( lockNET_SF_MSS_0_MSS_MASTER_APB_PRDATA ),
        .MAC_0_RXD   ( MAC_0_RXD ),
        .FABINT      ( apb3_interface_0_FABINT ),
        // Outputs
        .FAB_CLK     ( lockNET_SF_MSS_0_FAB_CLK ),
        .M2F_RESET_N ( lockNET_SF_MSS_0_M2F_RESET_N ),
        .UART_0_TXD  ( UART_0_TXD_net_0 ),
        .MSSPSEL     ( lockNET_SF_MSS_0_MSS_MASTER_APB_PSELx ),
        .MSSPENABLE  ( lockNET_SF_MSS_0_MSS_MASTER_APB_PENABLE ),
        .MSSPWRITE   ( lockNET_SF_MSS_0_MSS_MASTER_APB_PWRITE ),
        .MAC_0_TXEN  ( MAC_0_TXEN_net_0 ),
        .MAC_0_MDC   ( MAC_0_MDC_net_0 ),
        .M2F_GPO_0   ( M2F_GPO_0_net_0 ),
        .MSSPADDR    ( lockNET_SF_MSS_0_MSS_MASTER_APB_PADDR ),
        .MSSPWDATA   ( lockNET_SF_MSS_0_MSS_MASTER_APB_PWDATA ),
        .MAC_0_TXD   ( MAC_0_TXD_net_0 ),
        // Inouts
        .I2C_1_SCL   ( I2C_1_SCL ),
        .I2C_1_SDA   ( I2C_1_SDA ),
        .MAC_0_MDIO  ( MAC_0_MDIO ) 
        );


endmodule
