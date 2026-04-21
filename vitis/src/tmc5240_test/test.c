#include <stdlib.h>
#include <stdint.h>
#include "xparameters.h"
#include "xstatus.h"
#include "xgpio_l.h"
#include "sleep.h"

#include "xspi.h"
#include "xintc.h"

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

