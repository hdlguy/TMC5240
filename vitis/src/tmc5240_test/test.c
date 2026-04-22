#include <stdlib.h>
#include <stdint.h>
#include "xparameters.h"
#include "xstatus.h"
#include "xgpio_l.h"
#include "sleep.h"

#include "xspi.h"
#include "xintc.h"

// these are from the AD driver - https://github.com/analogdevicesinc/TMC-API/blob/master/tmc/ic/TMC5240/README.md
#include "TMC5240_HW_Abstraction.h"
#include "TMC5240.h"

int main()
{
    // get pointers to WDT and GPIO
    uint32_t* gpio_ptr = (uint32_t *) XPAR_XGPIO_0_BASEADDR;

    xil_printf("\n\rHello World!\n\r");


    uint32_t whilecount=0;
    while(1) {

        xil_printf("0x%08x: hello\n\r", whilecount);

        // increment the LEDs
        gpio_ptr[XGPIO_DATA_OFFSET/4] = 0x00ff & whilecount;    

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


TMC5240BusType tmc5240_getBusType(uint16_t icID){
    TMC5240BusType retval;
    xil_printf("TMC5240BusType: icID = 0x%04x\n\r", icID);
    retval = IC_BUS_SPI;
    return(retval);
}