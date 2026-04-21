
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

# max_spi_ssn[1:0], max_spi_miso, max_spi_mosi, and max_spi_sck.
set_property IOSTANDARD LVCMOS33    [get_ports max_*]
set_property SLEW SLOW              [get_ports max_spi_*]
set_property SLEW SLOW              [get_ports max_cnvtb]
set_property PACKAGE_PIN B11 [get_ports max_spi_mosi]
set_property PACKAGE_PIN A11 [get_ports max_spi_miso]
set_property PACKAGE_PIN D12 [get_ports max_spi_sck]
set_property PACKAGE_PIN G13 [get_ports {max_spi_ss[0]}]
set_property PACKAGE_PIN B18 [get_ports {max_spi_ss[1]}]
set_property PACKAGE_PIN D13 [get_ports max_intb]
set_property PACKAGE_PIN K16 [get_ports max_cnvtb]


#set_property PACKAGE_PIN G13        [get_ports ja1] ;# CS
#set_property PACKAGE_PIN B11        [get_ports ja2] ;# mosi
#set_property PACKAGE_PIN A11        [get_ports ja3] ;# miso
#set_property PACKAGE_PIN D12        [get_ports ja4] ;# sclk
#set_property PACKAGE_PIN D13        [get_ports ja7] ;# INTB
#set_property PACKAGE_PIN B18        [get_ports ja8] ;# unused
#set_property PACKAGE_PIN A18        [get_ports ja9] ;# unused
#set_property PACKAGE_PIN K16        [get_ports ja10] ;# CNVTB

