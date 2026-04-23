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
#include "TMC5240_HW_Abstraction.h"


int main()
{
    // get pointers to WDT and GPIO
    uint32_t* gpio_ptr = (uint32_t *) XPAR_XGPIO_0_BASEADDR;

    xil_printf("\n\rHello World!\n\r");
    
    // enable SPI master mode.
    u32 Control;
    Control = XSpi_ReadReg(SPI_BASEADDR, XSP_CR_OFFSET);
    Control |= XSP_CR_MASTER_MODE_MASK;
    XSpi_WriteReg(SPI_BASEADDR, XSP_CR_OFFSET, Control);
    

    // initialize the TMC5240
    tmc5240_init(0);
    tmc5240_init(1);
    tmc5240_init(2);


    // command the stepper motor to turn until the limit switch is activated, then go in reverse.
    


    uint32_t whilecount=0;
    float tempf[3];
    while(1) {

        xil_printf("0x%08x:\n\r", whilecount);

        // increment the LEDs
        gpio_ptr[XGPIO_DATA_OFFSET/4] = 0x00ff & whilecount;    

        // read the temperature registers.
        for (uint8_t dev=0; dev<3; dev++){ tempf[dev] = tmc5240_readTemp(dev); }
        for (uint8_t dev=0; dev<3; dev++){ xil_printf("%d: temp = %d.%03d\n\r", dev, (int)tempf[dev], (int)(1000.0*(tempf[dev] - (int)(tempf[dev]))) ); }
        

        // delay for about 1 second
        usleep(1000000);

        whilecount++;
    }
    
    return 0;
}


