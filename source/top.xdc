
set_property CFGBVS VCCO [current_design]
set_property CONFIG_VOLTAGE 2.5 [current_design]
set_property BITSTREAM.CONFIG.CONFIGRATE 50 [current_design]
set_property BITSTREAM.CONFIG.SPI_BUSWIDTH 4 [current_design]
set_property BITSTREAM.GENERAL.COMPRESS TRUE [current_design]

create_clock -period 10.000 -name clk100 [get_ports clkin100]

set_property IOSTANDARD LVCMOS33 [get_ports clkin100]
set_property PACKAGE_PIN E3 [get_ports clkin100]

set_property IOSTANDARD LVCMOS33    [get_ports {led[*]}]
set_property DRIVE 4                [get_ports {led[*]}]
set_property SLEW SLOW              [get_ports {led[*]}]
set_property PACKAGE_PIN T10 [get_ports {led[7]}]
set_property PACKAGE_PIN T9 [get_ports {led[6]}]
set_property PACKAGE_PIN J5 [get_ports {led[5]}]
set_property PACKAGE_PIN H5 [get_ports {led[4]}]
set_property PACKAGE_PIN H6 [get_ports {led[3]}]
set_property PACKAGE_PIN J2 [get_ports {led[2]}]
set_property PACKAGE_PIN J4 [get_ports {led[1]}]
set_property PACKAGE_PIN F6 [get_ports {led[0]}]

set_property IOSTANDARD LVCMOS33 [get_ports usb_uart*]
set_property PACKAGE_PIN A9 [get_ports usb_uart_rxd]
set_property PACKAGE_PIN D10 [get_ports usb_uart_txd]

set_property IOSTANDARD LVCMOS33 [get_ports rstn]
set_property PACKAGE_PIN C2 [get_ports rstn]

set_property IOSTANDARD LVCMOS33    [get_ports spi_*]
set_property SLEW SLOW              [get_ports spi_*]
set_property PACKAGE_PIN D12        [get_ports spi_mosi]
set_property PACKAGE_PIN B11        [get_ports spi_miso]
set_property PACKAGE_PIN A11        [get_ports spi_sck]
set_property PACKAGE_PIN B18        [get_ports spi_ss[0]]
set_property PACKAGE_PIN D13        [get_ports spi_ss[1]]
set_property PACKAGE_PIN G13        [get_ports spi_ss[2]]

## Pmod Header JA
#set_property -dict { PACKAGE_PIN G13   IOSTANDARD LVCMOS33 } [get_ports { ja[0] }]; #IO_0_15 Sch=ja[1]
#set_property -dict { PACKAGE_PIN B11   IOSTANDARD LVCMOS33 } [get_ports { ja[1] }]; #IO_L4P_T0_15 Sch=ja[2]
#set_property -dict { PACKAGE_PIN A11   IOSTANDARD LVCMOS33 } [get_ports { ja[2] }]; #IO_L4N_T0_15 Sch=ja[3]
#set_property -dict { PACKAGE_PIN D12   IOSTANDARD LVCMOS33 } [get_ports { ja[3] }]; #IO_L6P_T0_15 Sch=ja[4]
#set_property -dict { PACKAGE_PIN D13   IOSTANDARD LVCMOS33 } [get_ports { ja[4] }]; #IO_L6N_T0_VREF_15 Sch=ja[7]
#set_property -dict { PACKAGE_PIN B18   IOSTANDARD LVCMOS33 } [get_ports { ja[5] }]; #IO_L10P_T1_AD11P_15 Sch=ja[8]
#set_property -dict { PACKAGE_PIN A18   IOSTANDARD LVCMOS33 } [get_ports { ja[6] }]; #IO_L10N_T1_AD11N_15 Sch=ja[9]
#set_property -dict { PACKAGE_PIN K16   IOSTANDARD LVCMOS33 } [get_ports { ja[7] }]; #IO_25_15 Sch=ja[10]
