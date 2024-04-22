#**************************************************************
# This .sdc file is created by Terasic Tool.
# Users are recommended to modify this file to match users logic.
#**************************************************************

#**************************************************************
# Create Clock
#**************************************************************
create_clock -period 20 [get_ports CLOCK_50]
create_clock -period 20 [get_ports CLOCK2_50]
create_clock -period 20 [get_ports CLOCK3_50]

#**************************************************************
# Create Generated Clock
#**************************************************************
derive_pll_clocks

#**************************************************************
# Set Clock Uncertainty
#**************************************************************
derive_clock_uncertainty

#**************************************************************
# SDRAM constraints
#**************************************************************
set sdram_clk syspll_inst|altpll_component|auto_generated|pll1|clk[2]
create_generated_clock -name sdram_clk_pin -source $sdram_clk -offset 0.5 [get_ports {DRAM_CLK}]
set_input_delay -clock sdram_clk_pin -max [expr 6 + 0.6] \
	[get_ports {DRAM_DQ[*]}]
set_input_delay -clock sdram_clk_pin -min [expr 2.7 + 0.4] \
	[get_ports {DRAM_DQ[*]}]
set_output_delay -clock sdram_clk_pin -max [expr 1.5 + 0.6] \
	[get_ports {DRAM_RAS_N DRAM_CKE DRAM_DQ[*] DRAM_CAS_N DRAM_DQM[*] DRAM_CS_N DRAM_WE_N DRAM_ADDR[*] DRAM_BA[*]}]
set_output_delay -clock sdram_clk_pin -min [expr -0.8 + 0.4] \
	[get_ports {DRAM_RAS_N DRAM_CKE DRAM_DQ[*] DRAM_CAS_N DRAM_DQM[*] DRAM_CS_N DRAM_WE_N DRAM_ADDR[*] DRAM_BA[*]}]
set_multicycle_path -from [get_clocks {sdram_clk_pin}] -to [get_clocks {syspll_inst|altpll_component|auto_generated|pll1|clk[1]}] -setup -end 2
