#include <stdlib.h>
#include <stdint.h>
#include "xparameters.h"
#include "xstatus.h"
#include "xgpio_l.h"
#include "sleep.h"

#include "xspi.h"
#include "xintc.h"

#include "tmc5240_spi.h"

// these are from the AD driver - https://github.com/analogdevicesinc/TMC-API/blob/master/tmc/ic/TMC5240/README.md
//#include "TMC5240_HW_Abstraction.h"
//#include "TMC5240.h"

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


    uint32_t whilecount=0;
    uint32_t rval;
    while(1) {

        xil_printf("0x%08x: hello\n\r", whilecount);

        // increment the LEDs
        gpio_ptr[XGPIO_DATA_OFFSET/4] = 0x00ff & whilecount;    

        // read the temperature register.
        rval = tmc5240_read(0, 0x51);
        xil_printf("rval = 0x%08x\n\r", rval);
        int16_t temp = ((rval & 0x00001fff) << 3)/8;
        xil_printf("temp = 0x%04x = %d = %d\n\r", temp, temp, (int)((temp-2038.0)/7.7));
        

        // delay for about 1 second
        usleep(1000000);

        whilecount++;
    }
    
    return 0;
}

// extern void tmc5240_readWriteSPI(uint16_t icID, uint8_t *data, size_t dataLength);
// extern bool tmc5240_readWriteUART(uint16_t icID, uint8_t *data, size_t writeLength, size_t readLength);
// extern TMC5240BusType tmc5240_getBusType(uint16_t icID);
// extern uint8_t tmc5240_getNodeAddress(uint16_t icID);


//TMC5240BusType tmc5240_getBusType(uint16_t icID){
//    TMC5240BusType retval;
//    xil_printf("TMC5240BusType: icID = 0x%04x\n\r", icID);
//    retval = IC_BUS_SPI;
//    return(retval);
//}
