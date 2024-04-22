	niosII u0 (
		.clk_50_clk        (<connected-to-clk_50_clk>),        //     clk_50.clk
		.reset_50_reset_n  (<connected-to-reset_50_reset_n>),  //   reset_50.reset_n
		.hex_export        (<connected-to-hex_export>),        //        hex.export
		.led_export        (<connected-to-led_export>),        //        led.export
		.btn_export        (<connected-to-btn_export>),        //        btn.export
		.sdram_wire_addr   (<connected-to-sdram_wire_addr>),   // sdram_wire.addr
		.sdram_wire_ba     (<connected-to-sdram_wire_ba>),     //           .ba
		.sdram_wire_cas_n  (<connected-to-sdram_wire_cas_n>),  //           .cas_n
		.sdram_wire_cke    (<connected-to-sdram_wire_cke>),    //           .cke
		.sdram_wire_cs_n   (<connected-to-sdram_wire_cs_n>),   //           .cs_n
		.sdram_wire_dq     (<connected-to-sdram_wire_dq>),     //           .dq
		.sdram_wire_dqm    (<connected-to-sdram_wire_dqm>),    //           .dqm
		.sdram_wire_ras_n  (<connected-to-sdram_wire_ras_n>),  //           .ras_n
		.sdram_wire_we_n   (<connected-to-sdram_wire_we_n>),   //           .we_n
		.clk_100_clk       (<connected-to-clk_100_clk>),       //    clk_100.clk
		.reset_100_reset_n (<connected-to-reset_100_reset_n>)  //  reset_100.reset_n
	);
