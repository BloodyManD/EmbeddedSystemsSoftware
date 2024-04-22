
module niosII (
	btn_export,
	clk_100_clk,
	clk_50_clk,
	hex_export,
	led_export,
	reset_100_reset_n,
	reset_50_reset_n,
	sdram_wire_addr,
	sdram_wire_ba,
	sdram_wire_cas_n,
	sdram_wire_cke,
	sdram_wire_cs_n,
	sdram_wire_dq,
	sdram_wire_dqm,
	sdram_wire_ras_n,
	sdram_wire_we_n);	

	input	[3:0]	btn_export;
	input		clk_100_clk;
	input		clk_50_clk;
	output	[31:0]	hex_export;
	output	[7:0]	led_export;
	input		reset_100_reset_n;
	input		reset_50_reset_n;
	output	[12:0]	sdram_wire_addr;
	output	[1:0]	sdram_wire_ba;
	output		sdram_wire_cas_n;
	output		sdram_wire_cke;
	output		sdram_wire_cs_n;
	inout	[31:0]	sdram_wire_dq;
	output	[3:0]	sdram_wire_dqm;
	output		sdram_wire_ras_n;
	output		sdram_wire_we_n;
endmodule
