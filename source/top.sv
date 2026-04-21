// This is a design to communicate with three TMC5240 stepper motor controllers.
// There are three devices so there are three SS lines on the SPI interface.
module top (
    input   logic           clkin100,
    input   logic           rstn,
    output  logic[7:0]      led,
    //
    input   logic           usb_uart_rxd,
    output  logic           usb_uart_txd,
    //
    inout   logic           spi_mosi,
    inout   logic           spi_miso,
    inout   logic           spi_sck,
    inout   logic[2:0]      spi_ss
);

    logic clk;
    assign clk = clkin100;
    
    logic[1:0]  spi_io_i;
    logic[1:0]  spi_io_o;
    logic[1:0]  spi_io_t;
    logic       spi_sck_i;
    logic       spi_sck_o;
    logic       spi_sck_t;
    logic[2:0]  spi_ss_i;
    logic[2:0]  spi_ss_o;    
    logic       spi_ss_t;
    
    system system_i (
        //
        .clkin100           (clk),
        .rstn               (rstn),
        //    
        .usb_uart_rxd       (usb_uart_rxd),
        .usb_uart_txd       (usb_uart_txd),
        //
        .led_gpio_tri_o     (led),
        //
        .spi_io0_i      (spi_io_i[0]),
        .spi_io0_o      (spi_io_o[0]),
        .spi_io0_t      (spi_io_t[0]),
        .spi_io1_i      (spi_io_i[1]),
        .spi_io1_o      (spi_io_o[1]),
        .spi_io1_t      (spi_io_t[1]),
        .spi_sck_i      (spi_sck_i),
        .spi_sck_o      (spi_sck_o),
        .spi_sck_t      (spi_sck_t),
        .spi_ss_i       (spi_ss_i),
        .spi_ss_o       (spi_ss_o),
        .spi_ss_t       (spi_ss_t)
    );
    
    IOBUF spi_io0_iobuf (.I(spi_io_o[0]), .IO(spi_mosi),   .O(spi_io_i[0]), .T(spi_io_t[0]));
    IOBUF spi_io1_iobuf (.I(spi_io_o[1]), .IO(spi_miso),   .O(spi_io_i[1]), .T(spi_io_t[1]));
    IOBUF spi_sck_iobuf (.I(spi_sck_o),   .IO(spi_sck),    .O(spi_sck_i),   .T(spi_sck_t));
    IOBUF spi_ss_iobuf0 (.I(spi_ss_o[0]), .IO(spi_ss[0]),  .O(spi_ss_i[0]), .T(spi_ss_t));    
    IOBUF spi_ss_iobuf1 (.I(spi_ss_o[1]), .IO(spi_ss[1]),  .O(spi_ss_i[1]), .T(spi_ss_t));    
    IOBUF spi_ss_iobuf2 (.I(spi_ss_o[2]), .IO(spi_ss[2]),  .O(spi_ss_i[2]), .T(spi_ss_t));    
    
    // debug
    logic dbg_mosi, dbg_miso, dbg_sck;
    logic[1:0] dbg_ss;
    assign dbg_mosi = spi_io_i[0];
    assign dbg_miso = spi_io_i[1];
    assign dbg_sck  = spi_sck_i;
    assign dbg_ss   = spi_ss_i;
    spi_ila spi_ila_inst(.clk(clk), .probe0({dbg_mosi, dbg_miso, dbg_sck}), .probe1(dbg_ss)); //3,2

endmodule
