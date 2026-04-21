#include "xparameters.h"
#include "xstatus.h"
#include "xspi_l.h"
#include "xil_printf.h"
#include "sleep.h"
#include "max11311.h"
// This little program runs on the Microblaze and accesses the MAX11311 on the MAX11311PMB# Peripheral Module.
// It configures the part and then periodically reads the ID and internal temperature.
// The ports are externally jumpered P0->P1, ... P10->P11. 
// Ports P0, P2, and P4 are configured as DACs. P1, P3, and P5 are configured as ADCs. 
// Those DACs are written incrementing values and the ADCs read back.
// Ports P6, P8, and P10 are configured as GPO. Ports P7, P9, and P11 are configured as GPI.
// Incrementing values are written to those three GPO ports.

int main()
{
	xil_printf("\n\r\n\rmax11311_test\n\r");
	
	u32 Control;

	// enable SPI master mode.
	Control = XSpi_ReadReg(SPI_BASEADDR, XSP_CR_OFFSET);
	Control |= XSP_CR_MASTER_MODE_MASK;
	XSpi_WriteReg(SPI_BASEADDR, XSP_CR_OFFSET, Control);

	// configure temperature monitors
	max11311_write(0, 0x18, 0x0003);
	
	// write device control
	max11311_write(0, 0x10, 0x8000); // reset
	max11311_write(0, 0x10, 0x0143);
	
	// port[0:5] configuration, 0x5100=DAC, 0x7100=ADC
	max11311_write(0, 0x22, 0x5100);
	max11311_write(0, 0x23, 0x7100);
	max11311_write(0, 0x24, 0x5100);
	max11311_write(0, 0x25, 0x7100);
	max11311_write(0, 0x26, 0x5100);
	max11311_write(0, 0x27, 0x7100);
	
    // first we have to set the dac values for the gpo and gpi, 0x500 makes the GPO = 3.3V.
	max11311_write(0, 0x6b, 0x500);
	max11311_write(0, 0x6c, 0x500);
	max11311_write(0, 0x6d, 0x500);
	max11311_write(0, 0x6e, 0x500);
	max11311_write(0, 0x6f, 0x500);
	max11311_write(0, 0x70, 0x500);
    usleep(1000);

	// port[6:11] configuration, 0x3000=GPO, 0x1000=GPI, thresholds are set by DAC values
	max11311_write(0, 0x2b, 0x3100);
	max11311_write(0, 0x2c, 0x1100);
	max11311_write(0, 0x2d, 0x3100);
	max11311_write(0, 0x2e, 0x1100);
	max11311_write(0, 0x2f, 0x3100);
	max11311_write(0, 0x30, 0x1100);

		
    // loop and print values
	uint16_t rval, adcval[12], dacval[12], temp;
	uint32_t whilecount=0;
	uint16_t gpodat, gpidat;
	for(;;){
		
		xil_printf("\n\rwhilecount = 0x%08x\n\r", whilecount);
		
		// read the device ID
		rval = max11311_read(0, 0x00);
		xil_printf("MAX11311 ID = 0x%04x\n\r", rval);
		
		// read back Device Control
		rval = max11311_read(0, 0x10);
		xil_printf("MAX11311 Dev Control = 0x%04x\n\r", rval);

		// read the internal temperature
		rval = max11311_read(0, 0x08);
		temp = ((int16_t)(rval*16))/16; // convert 12-bit signed word to 16-bit signed by doing an unsigned multiply 16 and then a signed divide by 16.
		xil_printf("MAX11311 int temp = 0x%04x = %d.%dC\n\r", rval, temp/8, 125*(temp%8));

		// read and print the adc values
		adcval[0] = max11311_read(0, 0x42);
		adcval[1] = max11311_read(0, 0x43);
		adcval[2] = max11311_read(0, 0x44);
		adcval[3] = max11311_read(0, 0x45);
		adcval[4] = max11311_read(0, 0x46);
		adcval[5] = max11311_read(0, 0x47);
		
		adcval[6] = max11311_read(0, 0x4b);
		adcval[7] = max11311_read(0, 0x4c);
		adcval[8] = max11311_read(0, 0x4d);
		adcval[9] = max11311_read(0, 0x4e);
		adcval[10] = max11311_read(0, 0x4f);
		adcval[11] = max11311_read(0, 0x50);

		xil_printf("ADC: ");
		for (int i=0; i<6; i++) { xil_printf("0x%04x ", adcval[i]);} xil_printf("\n\r");
		
		// write  DACs
		for (int i=0; i<12; i++) { dacval[i] = (whilecount & 0x0fff); }
		max11311_write(0, 0x62, dacval[0]);
		max11311_write(0, 0x63, dacval[1]);
		max11311_write(0, 0x64, dacval[2]);
		max11311_write(0, 0x65, dacval[3]);
		max11311_write(0, 0x66, dacval[4]);
		max11311_write(0, 0x67, dacval[5]);		

		//max11311_write(0, 0x6b, dacval[6]);
		//max11311_write(0, 0x6c, dacval[7]);
		//max11311_write(0, 0x6d, dacval[8]);
		//max11311_write(0, 0x6e, dacval[9]);
		//max11311_write(0, 0x6f, dacval[10]);
		//max11311_write(0, 0x70, dacval[11]);

		// read back the DAC values and print
		dacval[0] = max11311_read(0, 0x62);
		dacval[1] = max11311_read(0, 0x63);
		dacval[2] = max11311_read(0, 0x64);
		dacval[3] = max11311_read(0, 0x65);
		dacval[4] = max11311_read(0, 0x66);
		dacval[5] = max11311_read(0, 0x67);
		
		dacval[6] = max11311_read(0, 0x6b);
		dacval[7] = max11311_read(0, 0x6c);
		dacval[8] = max11311_read(0, 0x6d);
		dacval[9] = max11311_read(0, 0x6e);
		dacval[10] = max11311_read(0, 0x6f);
		dacval[11] = max11311_read(0, 0x70);

		xil_printf("DAC: ");
		for (int i=0; i<12; i++) { xil_printf("0x%04x ", dacval[i]);} xil_printf("\n\r");

		// write the GPO values
		gpodat = 0;
		gpodat |= ((whilecount>>0) & 0x01) << 11;
		gpodat |= ((whilecount>>1) & 0x01) << 13;
		gpodat |= ((whilecount>>2) & 0x01) << 15;
		max11311_write(0, 0x0d, gpodat);		

		// read the GPI values
		rval = max11311_read(0, 0x0b);
		gpidat = 0;
		gpidat |= ((rval>>12) & 0x01) << 0;
		gpidat |= ((rval>>14) & 0x01) << 1;
		rval = max11311_read(0, 0x0c);
		gpidat |= ((rval>>0) & 0x01) << 2;
		xil_printf("GPO Data = 0x%04X,  GPI Data = 0x%04x", whilecount&0x07, gpidat);
		
		
		
		usleep(2000000);
		whilecount++;
	}
	
	xil_printf("rval = 0x%04x\n\r", rval);

}	

