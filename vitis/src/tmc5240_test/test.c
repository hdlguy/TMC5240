#include <stdlib.h>
#include <stdint.h>
#include "xparameters.h"
#include "xstatus.h"
#include "xgpio_l.h"
#include "sleep.h"

#include "xspi.h"
#include "xintc.h"

#include "tmc5240_spi.h"

// see - https://github.com/analogdevicesinc/TMC-API/blob/master/tmc/ic/TMC5240/README.md
// #include "TMC5240_HW_Abstraction.h"


void spin(uint8_t dev)
{
    // command stepper motor 0 to turn    
    tmc5240_write(dev, TMC5240_IHOLD_IRUN,      0x04041f10);   	// 12:8 = Irun, 4:0 = Ihold
    tmc5240_write(dev, TMC5240_RAMPMODE,        0x00000001);      // 1:0 = ramp mode, 1=pos vmax
    tmc5240_write(dev, TMC5240_AMAX,            0x00000400);
    tmc5240_write(dev, TMC5240_VMAX,            0x00101000);  
}

int main()
{
    // get pointers to WDT and GPIO
    uint32_t* gpio_ptr = (uint32_t *) XPAR_XGPIO_0_BASEADDR;

    xil_printf("\n\r*************** Hello World! ***********\n\r");
    
    // enable SPI master mode.
    u32 Control;
    Control = XSpi_ReadReg(SPI_BASEADDR, XSP_CR_OFFSET);
    Control |= XSP_CR_MASTER_MODE_MASK;
    Control |= XSP_CR_CLK_POLARITY_MASK;  // CPOL = 1
    Control |= XSP_CR_CLK_PHASE_MASK;     // CPHA = 1
    XSpi_WriteReg(SPI_BASEADDR, XSP_CR_OFFSET, Control);
    
    // initialize the TMC5240
    tmc5240_init(0);
    tmc5240_init(1);
    tmc5240_init(2);

    // dump the regs
    tmc5240_print_regs(0);
    tmc5240_print_regs(1);
    tmc5240_print_regs(2);

    // tell the motors to spin
    spin(0);
    spin(1);
    spin(2);

    uint32_t whilecount=0;
    float tempf[3];
    uint16_t tmc_version[3];
    while(1) {

        xil_printf("\n\r0x%08x:\n\r", whilecount);

        // increment the LEDs
        gpio_ptr[XGPIO_DATA_OFFSET/4] = 0x00ff & whilecount;  

        // read device versions
        for (uint8_t dev=0; dev<3; dev++){ tmc_version[dev] = tmc5240_readVersion(dev); }
        
        // read the temperature registers.
        for (uint8_t dev=0; dev<3; dev++){ tempf[dev] = tmc5240_readTemp(dev); }

        xil_printf("Versions: 0x%04x 0x%04x 0x%04x\n\r", tmc_version[0], tmc_version[1], tmc_version[2]);
        for (uint8_t dev=0; dev<3; dev++){ xil_printf("%d:temp = %d.%03d   ", dev, (int)tempf[dev], (int)(1000.0*(tempf[dev] - (int)(tempf[dev]))) ); }
        xil_printf("\n\r");
        
        // delay for 1 second
        usleep(1000000);

        whilecount++;
    }
    
    return 0;
}


