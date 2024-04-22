
module niosII (
	clk_50_clk,
	reset_50_reset_n,
	hex_export,
	led_export,
	btn_export,
	sdram_wire_addr,
	sdram_wire_ba,
	sdram_wire_cas_n,
	sdram_wire_cke,
	sdram_wire_cs_n,
	sdram_wire_dq,
	sdram_wire_dqm,
	sdram_wire_ras_n,
	sdram_wire_we_n,
	clk_100_clk,
	reset_100_reset_n);	

	input		clk_50_clk;
	input		reset_50_reset_n;
	output	[31:0]	hex_export;
	output	[7:0]	led_export;
	input	[3:0]	btn_export;
	output	[12:0]	sdram_wire_addr;
	output	[1:0]	sdram_wire_ba;
	output		sdram_wire_cas_n;
	output		sdram_wire_cke;
	output		sdram_wire_cs_n;
	inout	[31:0]	sdram_wire_dq;
	output	[3:0]	sdram_wire_dqm;
	output		sdram_wire_ras_n;
	output		sdram_wire_we_n;
	input		clk_100_clk;
	input		reset_100_reset_n;
endmodule
