

#define SPI_BASEADDR		XPAR_XSPI_0_BASEADDR

void tmc5240_write(uint8_t dev, uint8_t regnum, uint32_t val){
	uint8_t wbuf[5];
	
	wbuf[0] = regnum | 0x80;        // set write bit
	wbuf[1] = (val>>24) & 0x00ff;   // MSB
	wbuf[2] = (val>>16) & 0x00ff; 
	wbuf[3] = (val>> 8) & 0x00ff; 
	wbuf[4] = (val>> 0) & 0x00ff;   // LSB
	
	XSpi_WriteReg((SPI_BASEADDR), XSP_DTR_OFFSET, wbuf[0]);
	XSpi_WriteReg((SPI_BASEADDR), XSP_DTR_OFFSET, wbuf[1]);
	XSpi_WriteReg((SPI_BASEADDR), XSP_DTR_OFFSET, wbuf[2]);
	XSpi_WriteReg((SPI_BASEADDR), XSP_DTR_OFFSET, wbuf[3]);
	XSpi_WriteReg((SPI_BASEADDR), XSP_DTR_OFFSET, wbuf[4]);
		
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

uint32_t tmc5240_read(uint8_t dev, uint8_t regnum){
	uint8_t wbuf[16], rbuf[16];
	
	wbuf[0] = regnum | 0x00; // read
	wbuf[1] = 0;
	wbuf[2] = 0;
	wbuf[3] = 0;
	wbuf[4] = 0;
	
	XSpi_WriteReg((SPI_BASEADDR), XSP_DTR_OFFSET, wbuf[0]);
	XSpi_WriteReg((SPI_BASEADDR), XSP_DTR_OFFSET, wbuf[1]);
	XSpi_WriteReg((SPI_BASEADDR), XSP_DTR_OFFSET, wbuf[2]);
	XSpi_WriteReg((SPI_BASEADDR), XSP_DTR_OFFSET, wbuf[3]);
	XSpi_WriteReg((SPI_BASEADDR), XSP_DTR_OFFSET, wbuf[4]);
		
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
		
	
	uint32_t retval;
    retval = 0;
    retval |= ((uint32_t)rbuf[1]) << 24;
    retval |= ((uint32_t)rbuf[2]) << 16;
    retval |= ((uint32_t)rbuf[3]) <<  8;
    retval |= ((uint32_t)rbuf[4]) <<  0;
	
	return(retval);		
}


//extern void tmc5240_readWriteSPI(uint16_t icID, uint8_t *data, size_t dataLength);

