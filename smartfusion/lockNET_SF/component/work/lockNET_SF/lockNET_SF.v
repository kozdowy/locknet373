//////////////////////////////////////////////////////////////////////
// Created by SmartDesign Thu Dec 07 23:43:04 2017
// Version: v11.8 11.8.0.26
//////////////////////////////////////////////////////////////////////

`timescale 1ns / 100ps

// lockNET_SF
module lockNET_SF(
    // Inputs
    GPIO_1_IN,
    GPIO_9_IN,
    MSS_RESET_N,
    SPI_1_DI,
    UART_0_RXD,
    UART_1_RXD,
    // Outputs
    FABINT,
    GPIO_8_OUT,
    M2F_GPO_0,
    NP_OUT,
    RAM_RESET,
    SERVO_OUT,
    SPI_1_DO,
    UART_0_TXD,
    UART_1_TXD,
    // Inouts
    GPIO_10_BI,
    I2C_1_SCL,
    I2C_1_SDA,
    SPI_1_CLK,
    SPI_1_SS
);

//--------------------------------------------------------------------
// Input
//--------------------------------------------------------------------
input  GPIO_1_IN;
input  GPIO_9_IN;
input  MSS_RESET_N;
input  SPI_1_DI;
input  UART_0_RXD;
input  UART_1_RXD;
//--------------------------------------------------------------------
// Output
//--------------------------------------------------------------------
output FABINT;
output GPIO_8_OUT;
output M2F_GPO_0;
output NP_OUT;
output RAM_RESET;
output SERVO_OUT;
output SPI_1_DO;
output UART_0_TXD;
output UART_1_TXD;
//--------------------------------------------------------------------
// Inout
//--------------------------------------------------------------------
inout  GPIO_10_BI;
inout  I2C_1_SCL;
inout  I2C_1_SDA;
inout  SPI_1_CLK;
inout  SPI_1_SS;
//--------------------------------------------------------------------
// Nets
//--------------------------------------------------------------------
wire   [6:0]  apb3_interface_0_RAM_ADDRA;
wire   [6:0]  apb3_interface_0_RAM_ADDRB;
wire          apb3_interface_0_RAM_BLKA;
wire          apb3_interface_0_RAM_BLKB;
wire   [31:0] apb3_interface_0_RAM_DINA_0;
wire   [31:0] apb3_interface_0_RAM_DINB_0;
wire          apb3_interface_0_RAM_RWA;
wire          apb3_interface_0_RAM_RWB;
wire          CoreAPB3_0_APBmslave0_PENABLE;
wire   [31:0] CoreAPB3_0_APBmslave0_PRDATA;
wire          CoreAPB3_0_APBmslave0_PREADY;
wire          CoreAPB3_0_APBmslave0_PSELx;
wire          CoreAPB3_0_APBmslave0_PSLVERR;
wire   [31:0] CoreAPB3_0_APBmslave0_PWDATA;
wire          CoreAPB3_0_APBmslave0_PWRITE;
wire          FABINT_net_0;
wire          GPIO_1_IN;
wire          GPIO_8_OUT_net_0;
wire          GPIO_9_IN;
wire          GPIO_10_BI;
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
wire          MSS_RESET_N;
wire          NP_OUT_0;
wire          RAM_RESET_net_0;
wire   [31:0] RSA_64b_RAM_0_DOUTA_0;
wire   [31:0] RSA_64b_RAM_0_DOUTB_0;
wire          SERVO_OUT_0;
wire          SPI_1_CLK;
wire          SPI_1_DI;
wire          SPI_1_DO_0;
wire          SPI_1_SS;
wire          UART_0_RXD;
wire          UART_0_TXD_0;
wire          UART_1_RXD;
wire          UART_1_TXD_0;
wire          M2F_GPO_0_net_1;
wire          GPIO_8_OUT_net_1;
wire          SPI_1_DO_0_net_0;
wire          UART_1_TXD_0_net_0;
wire          UART_0_TXD_0_net_0;
wire          NP_OUT_0_net_0;
wire          SERVO_OUT_0_net_0;
wire          FABINT_net_1;
wire          RAM_RESET_net_1;
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
assign M2F_GPO_0_net_1    = M2F_GPO_0_net_0;
assign M2F_GPO_0          = M2F_GPO_0_net_1;
assign GPIO_8_OUT_net_1   = GPIO_8_OUT_net_0;
assign GPIO_8_OUT         = GPIO_8_OUT_net_1;
assign SPI_1_DO_0_net_0   = SPI_1_DO_0;
assign SPI_1_DO           = SPI_1_DO_0_net_0;
assign UART_1_TXD_0_net_0 = UART_1_TXD_0;
assign UART_1_TXD         = UART_1_TXD_0_net_0;
assign UART_0_TXD_0_net_0 = UART_0_TXD_0;
assign UART_0_TXD         = UART_0_TXD_0_net_0;
assign NP_OUT_0_net_0     = NP_OUT_0;
assign NP_OUT             = NP_OUT_0_net_0;
assign SERVO_OUT_0_net_0  = SERVO_OUT_0;
assign SERVO_OUT          = SERVO_OUT_0_net_0;
assign FABINT_net_1       = FABINT_net_0;
assign FABINT             = FABINT_net_1;
assign RAM_RESET_net_1    = RAM_RESET_net_0;
assign RAM_RESET          = RAM_RESET_net_1;
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
        .PCLK      ( lockNET_SF_MSS_0_FAB_CLK ),
        .PENABLE   ( CoreAPB3_0_APBmslave0_PENABLE ),
        .PSEL      ( CoreAPB3_0_APBmslave0_PSELx ),
        .PRESETN   ( lockNET_SF_MSS_0_M2F_RESET_N ),
        .PWRITE    ( CoreAPB3_0_APBmslave0_PWRITE ),
        .PADDR     ( CoreAPB3_0_APBmslave0_PADDR_0 ),
        .PWDATA    ( CoreAPB3_0_APBmslave0_PWDATA ),
        .RAM_DOUTA ( RSA_64b_RAM_0_DOUTA_0 ),
        .RAM_DOUTB ( RSA_64b_RAM_0_DOUTB_0 ),
        // Outputs
        .PREADY    ( CoreAPB3_0_APBmslave0_PREADY ),
        .PSLVERR   ( CoreAPB3_0_APBmslave0_PSLVERR ),
        .NP_OUT    ( NP_OUT_0 ),
        .SERVO_OUT ( SERVO_OUT_0 ),
        .FABINT    ( FABINT_net_0 ),
        .RAM_RWA   ( apb3_interface_0_RAM_RWA ),
        .RAM_RWB   ( apb3_interface_0_RAM_RWB ),
        .RAM_BLKA  ( apb3_interface_0_RAM_BLKA ),
        .RAM_BLKB  ( apb3_interface_0_RAM_BLKB ),
        .RAM_RESET ( RAM_RESET_net_0 ),
        .PRDATA    ( CoreAPB3_0_APBmslave0_PRDATA ),
        .RAM_DINA  ( apb3_interface_0_RAM_DINA_0 ),
        .RAM_DINB  ( apb3_interface_0_RAM_DINB_0 ),
        .RAM_ADDRA ( apb3_interface_0_RAM_ADDRA ),
        .RAM_ADDRB ( apb3_interface_0_RAM_ADDRB ) 
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
        .MSSPREADY   ( lockNET_SF_MSS_0_MSS_MASTER_APB_PREADY ),
        .MSSPSLVERR  ( lockNET_SF_MSS_0_MSS_MASTER_APB_PSLVERR ),
        .GPIO_1_IN   ( GPIO_1_IN ),
        .FABINT      ( FABINT_net_0 ),
        .GPIO_9_IN   ( GPIO_9_IN ),
        .SPI_1_DI    ( SPI_1_DI ),
        .UART_1_RXD  ( UART_1_RXD ),
        .UART_0_RXD  ( UART_0_RXD ),
        .MSSPRDATA   ( lockNET_SF_MSS_0_MSS_MASTER_APB_PRDATA ),
        // Outputs
        .FAB_CLK     ( lockNET_SF_MSS_0_FAB_CLK ),
        .M2F_RESET_N ( lockNET_SF_MSS_0_M2F_RESET_N ),
        .MSSPSEL     ( lockNET_SF_MSS_0_MSS_MASTER_APB_PSELx ),
        .MSSPENABLE  ( lockNET_SF_MSS_0_MSS_MASTER_APB_PENABLE ),
        .MSSPWRITE   ( lockNET_SF_MSS_0_MSS_MASTER_APB_PWRITE ),
        .M2F_GPO_0   ( M2F_GPO_0_net_0 ),
        .GPIO_8_OUT  ( GPIO_8_OUT_net_0 ),
        .SPI_1_DO    ( SPI_1_DO_0 ),
        .UART_1_TXD  ( UART_1_TXD_0 ),
        .UART_0_TXD  ( UART_0_TXD_0 ),
        .MSSPADDR    ( lockNET_SF_MSS_0_MSS_MASTER_APB_PADDR ),
        .MSSPWDATA   ( lockNET_SF_MSS_0_MSS_MASTER_APB_PWDATA ),
        // Inouts
        .I2C_1_SCL   ( I2C_1_SCL ),
        .I2C_1_SDA   ( I2C_1_SDA ),
        .SPI_1_CLK   ( SPI_1_CLK ),
        .SPI_1_SS    ( SPI_1_SS ),
        .GPIO_10_BI  ( GPIO_10_BI ) 
        );

//--------RSA_64b_RAM
RSA_64b_RAM RSA_64b_RAM_0(
        // Inputs
        .RWA   ( apb3_interface_0_RAM_RWA ),
        .RWB   ( apb3_interface_0_RAM_RWB ),
        .BLKA  ( apb3_interface_0_RAM_BLKA ),
        .BLKB  ( apb3_interface_0_RAM_BLKB ),
        .CLKAB ( lockNET_SF_MSS_0_FAB_CLK ),
        .RESET ( lockNET_SF_MSS_0_M2F_RESET_N ),
        .DINA  ( apb3_interface_0_RAM_DINA_0 ),
        .DINB  ( apb3_interface_0_RAM_DINB_0 ),
        .ADDRA ( apb3_interface_0_RAM_ADDRA ),
        .ADDRB ( apb3_interface_0_RAM_ADDRB ),
        // Outputs
        .DOUTA ( RSA_64b_RAM_0_DOUTA_0 ),
        .DOUTB ( RSA_64b_RAM_0_DOUTB_0 ) 
        );


endmodule
