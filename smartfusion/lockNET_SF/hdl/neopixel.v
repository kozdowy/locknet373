// pwm.v
module pwm(
    /*** APB3 BUS INTERFACE ***/
    input PCLK, 				// clock
    input PRESERN, 				// system reset
    input PSEL, 				// peripheral select
    input PENABLE, 				// distinguishes access phase
    output wire PREADY, 		// peripheral ready signal
    output wire PSLVERR,		// error signal
    input PWRITE,				// distinguishes read and write cycles
    input [31:0] PADDR,			// I/O address
    input wire [31:0] PWDATA,	// data from processor to I/O device (32 bits)
    output reg [31:0] PRDATA,	// data to processor from I/O device (32-bits)

    //output [15:0] test,

    //input [1:0] SW,
    output reg pwm_out
    //output write_pulse
);

/*
 * Receive 4 control bits, 4 bits detailing which pixel, then 24 bits with colours (GRB)
 * Move in to shift register with G[7] first
 * pulse msb, shift with 1.25us cycle
 * allow setting of shift registers first, then can send "apply" to send the data
 */

    `define period 125000
    
    //wire [1:0] SW_db;
    wire write_pulse;

/*
assign test[15] = PCLK;
assign test[14:12] = PADDR[4:2];
assign test[11] = PSEL;
assign test[10] = PENABLE;
assign test[9] = PRESERN;
assign test[8] = PWRITE;
assign test[7:0] = PWRITE ? PWDATA : PRDATA;
*/

    assign PSLVERR = 0;
    assign PREADY = 1;

    assign write_pulse = PWRITE & PADDR[2];

    reg [31:0] pulse_width;

    //Button_Debouncer db1(.clk(PCLK), .PB_in(SW[1]), .PB_out(SW_db[1]));
    //Button_Debouncer db0(.clk(PCLK), .PB_in(SW[0]), .PB_out(SW_db[0]));

    initial begin
        pulse_width <= 32'd60000;
    end

    always @(posedge PCLK) 		//register control for APB3 reads
    begin
        PRDATA[31:2] <= 30'h00000000;				//initialize the PRDATA
        PRDATA[1:0] <= 2'b11; 	//read switch values to PRDATA
    end


    always @(posedge PCLK) begin
       // pulse_width <= pulse_width;
       if(!PRESERN)
           pulse_width <= pulse_width;  // LED should start turned "off"
												// recall it is active low. 
      else if(write_pulse) begin
            pulse_width <= PWDATA[31:0];
        end
    end

    reg [31:0] count;

    always @(posedge PCLK) begin
        if (count == `period)
            count <= 0;
        else
            count <= count + 1;
        if (count < pulse_width)
            pwm_out <= 1;
        else
            pwm_out <= 0;
    end

endmodule

/*
//
// *** Button debouncer. ***
// Changes in the input are ignored until they have been consistent for 2^16 clocks.
//
module Button_Debouncer(
input clk,
input PB_in, // button input
output reg PB_out // debounced output
);
    reg [15:0] PB_cnt; // 16-bit counter
    reg [1:0] sync; // used as two flip-flops to synchronize
    // button to the clk domain.
    // First use two flipflops to synchronize the PB signal the "clk" clock domain
    always @(posedge clk)
        sync[1:0] <= {sync[0],~PB_in};
    wire PB_idle = (PB_out==sync[1]);    // See if we have a new input state for PB
    wire PB_cnt_max = &PB_cnt;           // true when all bits of PB_cnt are 1's
                                         // using & in this way is a
                                         // "reduction operation"
    // When the push-button is pushed or released, we increment the counter.
    // The counter has to be maxed out before we decide that the push-button
    // state has changed
    always @(posedge clk) begin
        if(PB_idle)
            PB_cnt<= 16'd0; // input and output are the same so clear counter
        else begin
            PB_cnt<= PB_cnt + 16'd1; // input different than output, count
            if(PB_cnt_max)
                PB_out<= ~PB_out;   // if the counter is maxed out,
                                    // change PB output
        end
    end
endmodule
*/