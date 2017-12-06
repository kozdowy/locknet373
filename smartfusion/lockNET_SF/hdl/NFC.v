module nfc(
			input pclk,
			input nreset,
			input bus_write_en, 
			input bus_read_en,
			input bus_addr,
			input bus_write_data,
			input bus_read_data,
            output fabint,
            input irq_pin
            );

    reg fabint;

    always @(posedge pclk) begin
        if (~nreset && (irq_pin == 0)) 
            fabint <= 1;
        else
            fabint <= 0;
    end
        
endmodule
