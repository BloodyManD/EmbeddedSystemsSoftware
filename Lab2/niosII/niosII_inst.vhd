	component niosII is
		port (
			btn_export        : in    std_logic_vector(3 downto 0)  := (others => 'X'); -- export
			clk_100_clk       : in    std_logic                     := 'X';             -- clk
			clk_50_clk        : in    std_logic                     := 'X';             -- clk
			hex_export        : out   std_logic_vector(31 downto 0);                    -- export
			led_export        : out   std_logic_vector(7 downto 0);                     -- export
			reset_100_reset_n : in    std_logic                     := 'X';             -- reset_n
			reset_50_reset_n  : in    std_logic                     := 'X';             -- reset_n
			sdram_wire_addr   : out   std_logic_vector(12 downto 0);                    -- addr
			sdram_wire_ba     : out   std_logic_vector(1 downto 0);                     -- ba
			sdram_wire_cas_n  : out   std_logic;                                        -- cas_n
			sdram_wire_cke    : out   std_logic;                                        -- cke
			sdram_wire_cs_n   : out   std_logic;                                        -- cs_n
			sdram_wire_dq     : inout std_logic_vector(31 downto 0) := (others => 'X'); -- dq
			sdram_wire_dqm    : out   std_logic_vector(3 downto 0);                     -- dqm
			sdram_wire_ras_n  : out   std_logic;                                        -- ras_n
			sdram_wire_we_n   : out   std_logic                                         -- we_n
		);
	end component niosII;

	u0 : component niosII
		port map (
			btn_export        => CONNECTED_TO_btn_export,        --        btn.export
			clk_100_clk       => CONNECTED_TO_clk_100_clk,       --    clk_100.clk
			clk_50_clk        => CONNECTED_TO_clk_50_clk,        --     clk_50.clk
			hex_export        => CONNECTED_TO_hex_export,        --        hex.export
			led_export        => CONNECTED_TO_led_export,        --        led.export
			reset_100_reset_n => CONNECTED_TO_reset_100_reset_n, --  reset_100.reset_n
			reset_50_reset_n  => CONNECTED_TO_reset_50_reset_n,  --   reset_50.reset_n
			sdram_wire_addr   => CONNECTED_TO_sdram_wire_addr,   -- sdram_wire.addr
			sdram_wire_ba     => CONNECTED_TO_sdram_wire_ba,     --           .ba
			sdram_wire_cas_n  => CONNECTED_TO_sdram_wire_cas_n,  --           .cas_n
			sdram_wire_cke    => CONNECTED_TO_sdram_wire_cke,    --           .cke
			sdram_wire_cs_n   => CONNECTED_TO_sdram_wire_cs_n,   --           .cs_n
			sdram_wire_dq     => CONNECTED_TO_sdram_wire_dq,     --           .dq
			sdram_wire_dqm    => CONNECTED_TO_sdram_wire_dqm,    --           .dqm
			sdram_wire_ras_n  => CONNECTED_TO_sdram_wire_ras_n,  --           .ras_n
			sdram_wire_we_n   => CONNECTED_TO_sdram_wire_we_n    --           .we_n
		);

