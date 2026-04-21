/******************************************************************************
* Copyright (C) 2002 - 2023 Xilinx, Inc.  All rights reserved.
* Copyright (c) 2022 - 2023 Advanced Micro Devices, Inc. All Rights Reserved.
* SPDX-License-Identifier: MIT
******************************************************************************/

/******************************************************************************/
/**
* @file  xspi_low_level_example.c
*
* This file contains a design example using the low-level driver of the
* SPI driver (XSpi). These macros are found in xspi_l.h.  A simple loopback
* test is done within an SPI device in polled mode. This example works only with
* 8-bit wide data transfers.
*
* @note
* This example works only with 8-bit wide data transfers in standard SPI mode.
* This example will not work if the axi_qspi device is confiured in dual/quad
* modes.
*
* To make this example work for 16 bit transfers change u8 Buffer[BUFFER_SIZE]
* to u16 Buffer[BUFFER_SIZE]. The SPI Core should also be configured for 16 bit
* access during the build time.
*
* To make this example work for 32 bit transfers change u8 Buffer[BUFFER_SIZE]
* to u32 Buffer[BUFFER_SIZE]. The SPI Core should also be configured for 32 bit
* access during the build time.
*
*
*<pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- ----------------------------------------------------------
* 1.00b rpm  04/24/02 First release
* 1.00b jhl  09/10/02 Added code to ensure it works with a fast processor.
* 1.00b sv   05/16/05 Minor changes to comply to Doxygen and coding guidelines
* 3.00a ktn  10/28/09 Converted all register accesses to 32 bit access.
* 3.02a sdm  05/04/11 Added a note about dual/quad modes in axi_qspi.
* 4.2   ms   01/23/17 Added xil_printf statement in main function to
*                     ensure that "Successfully ran" and "Failed" strings
*                     are available in all examples. This is a fix for
*                     CR-965028.
* 4.11  sb   07/11/23 Added support for system device-tree flow.
*
*</pre>
*******************************************************************************/

/***************************** Include Files **********************************/

#include "xparameters.h"
#include "xstatus.h"
#include "xspi_l.h"
#include "xil_printf.h"

/************************** Constant Definitions ******************************/

/*
 * The following constants map to the XPAR parameters created in the
 * xparameters.h file. They are defined here such that a user can easily
 * change all the needed parameters in one place.
 */
#ifndef SDT
#define SPI_BASEADDR		XPAR_SPI_0_BASEADDR
#else
#define SPI_BASEADDR		XPAR_XSPI_0_BASEADDR
#endif

/**************************** Type Definitions ********************************/


/***************** Macros (Inline Functions) Definitions **********************/


/************************** Function Prototypes *******************************/

int XSpi_LowLevelExample(void); //(u32 BaseAddress);

/************************** Variable Definitions ******************************/


/*
 *  This is the size of the buffer to be transmitted/received in this example.
 */
#define BUFFER_SIZE			 32

/*
 * The buffer used for Transmission/Reception of the SPI test data
 */
u16 Buffer[BUFFER_SIZE];

void max11311_write(uint8_t dev, uint8_t regnum, uint16_t val){
	uint8_t wbuf[3];
	
	wbuf[0] = (regnum<<1) | (0x00); // write bit
	wbuf[1] = (val>>8) & 0x00ff;
	wbuf[2] = (val>>0) & 0x00ff;

// xil_printf("wbuf[0:2] = %02x %02x %02x\n\r", wbuf[0], wbuf[1], wbuf[2]);
// xil_printf("%02x %02x %04x\n\r", dev, regnum, val);
		
	
	XSpi_WriteReg((SPI_BASEADDR), XSP_DTR_OFFSET, wbuf[0]);
	XSpi_WriteReg((SPI_BASEADDR), XSP_DTR_OFFSET, wbuf[1]);
	XSpi_WriteReg((SPI_BASEADDR), XSP_DTR_OFFSET, wbuf[2]);
		
	XSpi_WriteReg(SPI_BASEADDR, XSP_SSR_OFFSET, 0xffffffff ^ (0x00000001<<dev));
	
	// * Enable the device.
	uint32_t Control;
	Control = XSpi_ReadReg(SPI_BASEADDR, XSP_CR_OFFSET);
	Control |= XSP_CR_ENABLE_MASK;
	Control &= ~XSP_CR_TRANS_INHIBIT_MASK;
	XSpi_WriteReg(SPI_BASEADDR, XSP_CR_OFFSET, Control);

	while (!(XSpi_ReadReg(SPI_BASEADDR, XSP_SR_OFFSET) & XSP_SR_TX_EMPTY_MASK));

	int NumBytesRcvd = 0;
	while ((XSpi_ReadReg(SPI_BASEADDR, XSP_SR_OFFSET) & XSP_SR_RX_EMPTY_MASK) == 0) {
		Buffer[NumBytesRcvd++] = XSpi_ReadReg((SPI_BASEADDR), XSP_DRR_OFFSET);
	}

	// disable the device.
	Control = XSpi_ReadReg(SPI_BASEADDR, XSP_CR_OFFSET);
	Control &= ~XSP_CR_ENABLE_MASK;
	Control &= ~XSP_CR_TRANS_INHIBIT_MASK;
	XSpi_WriteReg(SPI_BASEADDR, XSP_CR_OFFSET, Control);	
		
}

uint16_t max11311_read(uint8_t dev, uint8_t regnum){
	uint8_t wbuf[16], rbuf[16];
	
	wbuf[0] = (regnum << 1) | (0x01); // read bit
	wbuf[1] = 0;
	wbuf[2] = 0;
	
// xil_printf("wbuf[0:2] = %02x %02x %02x\n\r", wbuf[0], wbuf[1], wbuf[2]);
// xil_printf("%02x %02x\n\r", dev, regnum);
	
	XSpi_WriteReg((SPI_BASEADDR), XSP_DTR_OFFSET, wbuf[0]);
	XSpi_WriteReg((SPI_BASEADDR), XSP_DTR_OFFSET, wbuf[1]);
	XSpi_WriteReg((SPI_BASEADDR), XSP_DTR_OFFSET, wbuf[2]);
		
	XSpi_WriteReg(SPI_BASEADDR, XSP_SSR_OFFSET, 0xffffffff ^ (0x00000001<<dev));
	
	// * Enable the device.
	uint32_t Control;
	Control = XSpi_ReadReg(SPI_BASEADDR, XSP_CR_OFFSET);
	Control |= XSP_CR_ENABLE_MASK;
	Control &= ~XSP_CR_TRANS_INHIBIT_MASK;
	XSpi_WriteReg(SPI_BASEADDR, XSP_CR_OFFSET, Control);

	while (!(XSpi_ReadReg(SPI_BASEADDR, XSP_SR_OFFSET) & XSP_SR_TX_EMPTY_MASK));

	int NumBytesRcvd = 0;
	while ((XSpi_ReadReg(SPI_BASEADDR, XSP_SR_OFFSET) & XSP_SR_RX_EMPTY_MASK) == 0) {
		rbuf[NumBytesRcvd++] = XSpi_ReadReg((SPI_BASEADDR), XSP_DRR_OFFSET);
	}
	
	// disable the device.
	Control = XSpi_ReadReg(SPI_BASEADDR, XSP_CR_OFFSET);
	Control &= ~XSP_CR_ENABLE_MASK;
	Control &= ~XSP_CR_TRANS_INHIBIT_MASK;
	XSpi_WriteReg(SPI_BASEADDR, XSP_CR_OFFSET, Control);	
		
	
	uint16_t retval;
	retval = (((uint16_t)(rbuf[1])) << 8) | rbuf[0];
	
	return(retval);		
}


// int XSpi_LowLevelExample() //(u32 BaseAddress)
int main()
{
	xil_printf("low_level_test\n\r");
	
	u32 BaseAddress = SPI_BASEADDR;
	u32 Control;
	// int NumBytesSent;
	// int NumBytesRcvd;
	// int Nbuf = 16;
	// uint8_t wbuf[Nbuf], rbuf[Nbuf];

	// enable master mode.
	Control = XSpi_ReadReg(BaseAddress, XSP_CR_OFFSET);
	Control |= XSP_CR_MASTER_MODE_MASK;
	XSpi_WriteReg(BaseAddress, XSP_CR_OFFSET, Control);

	// writes and reads
	uint16_t rval;
	for(;;){
		for (int i=0; i<128; i++){
			max11311_write(1, i, 0xf0f3);
			rval = max11311_read(1, i);
		}
	}
	
	xil_printf("rval = 0x%04x\n\r", rval);


}	


// 	// send three bytes
// 	XSpi_WriteReg((BaseAddress), XSP_DTR_OFFSET, 0xfa);
// 	XSpi_WriteReg((BaseAddress), XSP_DTR_OFFSET, 0xf3);
// 	XSpi_WriteReg((BaseAddress), XSP_DTR_OFFSET, 0x55);
// 	NumBytesSent = 3;
	

// 	// slave select	
// 	XSpi_WriteReg(BaseAddress, XSP_SSR_OFFSET, 0xfffffffe);


// 	// Enable the device.
// 	Control = XSpi_ReadReg(BaseAddress, XSP_CR_OFFSET);
// 	Control |= XSP_CR_ENABLE_MASK;
// 	Control &= ~XSP_CR_TRANS_INHIBIT_MASK;
// 	XSpi_WriteReg(BaseAddress, XSP_CR_OFFSET, Control);


// 	 // Initialize the rx buffer with zeroes so that it can be used to receive data.
// 	for (int i = 0; i < Nbuf; i++) { rbuf[i] = 0x0; }

// 	// Wait for the transmit FIFO to transition to empty before checking
// 	while (!(XSpi_ReadReg(BaseAddress, XSP_SR_OFFSET) & XSP_SR_TX_EMPTY_MASK));


// 	//now receive the data just looped back until the receiver is empty. */
// 	while ((XSpi_ReadReg(BaseAddress, XSP_SR_OFFSET) & XSP_SR_RX_EMPTY_MASK) == 0) {
// 		rbuf[NumBytesRcvd++] = XSpi_ReadReg((BaseAddress), XSP_DRR_OFFSET);
// 	}

// 	// disable the device.
// 	Control = XSpi_ReadReg(BaseAddress, XSP_CR_OFFSET);
// 	Control &= ~XSP_CR_ENABLE_MASK;
// 	Control &= ~XSP_CR_TRANS_INHIBIT_MASK;
// 	XSpi_WriteReg(BaseAddress, XSP_CR_OFFSET, Control);

// 	// If no data was sent or the data that was sent was not received, then return an error
// 	if ((NumBytesSent != NumBytesRcvd) || (NumBytesSent == 0)) {
// 		xil_printf("FAIL: NumBytesRcvd = %d\n\r", NumBytesRcvd);
// 		return XST_FAILURE;
// 	} else {
// 		xil_printf("PASS: NumBytesRcvd = %d\n\r", NumBytesRcvd);
// 		return XST_SUCCESS;
// 	}
// }
