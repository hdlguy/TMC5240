

#define SPI_BASEADDR		XPAR_XSPI_0_BASEADDR

void max11311_write(uint8_t dev, uint8_t regnum, uint16_t val){
	uint8_t wbuf[3];
	
	wbuf[0] = (regnum<<1) | (0x00); // write bit
	wbuf[1] = (val>>8) & 0x00ff; // MSB
	wbuf[2] = (val>>0) & 0x00ff; // LSB
	
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
		XSpi_ReadReg((SPI_BASEADDR), XSP_DRR_OFFSET);
		NumBytesRcvd++;
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
	retval = (((uint16_t)(rbuf[1])) << 8) | rbuf[2];  // MSB is in rbuf[1]. LSB is in rbuf[2].
	
	return(retval);		
}


