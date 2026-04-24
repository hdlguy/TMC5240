
#include "TMC5240_HW_Abstraction.h"


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

float tmc5240_readTemp(uint8_t dev)
{
    uint32_t rval;
    rval = tmc5240_read(dev, TMC5240_ADC_TEMP);  // the first read establishes the register address.
    rval = tmc5240_read(dev, TMC5240_ADC_TEMP);  // the second read returns valid data.   
    int16_t temp = ((rval & 0x00001fff) << 3)/8; 
    float float_temp = (temp-2038.0)/7.7;
    return(float_temp);
}

uint16_t tmc5240_readVersion(uint8_t dev)
{
    uint32_t rval;
    rval = tmc5240_read(dev, TMC5240_INP_OUT);  // the first read establishes the register address.
    rval = tmc5240_read(dev, TMC5240_INP_OUT);  // the second read returns valid data.   
    return((rval & 0xff070000) >> 16);
}

int32_t tmc5240_readVelocity(uint8_t dev)
{
    uint32_t rval;
    rval = tmc5240_read(dev, TMC5240_VACTUAL);  // the first read establishes the register address.
    rval = tmc5240_read(dev, TMC5240_VACTUAL);  // the second read returns valid data.   
    return ((int32_t)(rval<<8))/256  ;
}

void tmc5240_init(uint8_t dev)
{
	uint32_t wval;
	uint8_t regnum;
	
	regnum = TMC5240_GCONF;
	wval = 0x00000000;
	tmc5240_write(dev, regnum, wval);
	
	regnum = TMC5240_GSTAT;
	wval = 0xffffffff;
	tmc5240_write(dev, regnum, wval);

	regnum = TMC5240_INP_OUT;
	wval = 0x00000000;
	tmc5240_write(dev, regnum, wval);
	
	
	regnum = TMC5240_X_COMPARE_REPEAT;
	wval = 0x00000008;
	tmc5240_write(dev, regnum, wval);
	
	regnum = TMC5240_DRV_CONF;
	wval = 0x00000000;
	tmc5240_write(dev, regnum, wval);
	
	regnum = TMC5240_GLOBAL_SCALER;
	wval = 0x000000a0;
	tmc5240_write(dev, regnum, wval);
		
	regnum = TMC5240_IHOLD_IRUN;
	wval = 0x04040503;
	tmc5240_write(dev, regnum, wval);	
	
	regnum = TMC5240_TPOWERDOWN;
	wval = 0x0000000a;
	tmc5240_write(dev, regnum, wval);
	
	regnum = TMC5240_TPOWERDOWN;
	wval = 0x00000000;
	tmc5240_write(dev, regnum, wval);
	
	regnum = TMC5240_VSTART;
	wval = 0x00000010;
	tmc5240_write(dev, regnum, wval);
	
	regnum = TMC5240_A1;
	wval = 0x00000010;
	tmc5240_write(dev, regnum, wval);
	
	regnum = TMC5240_V1;
	wval = 0x00000020;
	tmc5240_write(dev, regnum, wval);
	
	regnum = TMC5240_AMAX;
	wval = 0x00000020;
	tmc5240_write(dev, regnum, wval);
	
	regnum = TMC5240_VMAX;
	wval = 0x00000040;
	tmc5240_write(dev, regnum, wval);
	
	regnum = TMC5240_DMAX;
	wval = 0x00000020;
	tmc5240_write(dev, regnum, wval);
	
	regnum = TMC5240_TVMAX;
	wval = 0x00000020;
	tmc5240_write(dev, regnum, wval);	
	
	regnum = TMC5240_D1;
	wval = 0x00000010;
	tmc5240_write(dev, regnum, wval);
	
	regnum = TMC5240_VSTOP;
	wval = 0x00000020;
	tmc5240_write(dev, regnum, wval);
	
	regnum = TMC5240_V2;
	wval = 0x00000020;
	tmc5240_write(dev, regnum, wval);
	
}



void tmc5240_print_regs(uint8_t dev)
{
	xil_printf("\n\rdev = 0x%02x", dev);
	uint32_t rval;
	for (int i=0; i<0x77; i++) {		
		rval = tmc5240_read(dev, i);
		rval = tmc5240_read(dev, i); 
		if (i%4==0) xil_printf("\n\r0x%02x:", i);
		xil_printf("0x%08x ", rval);
	}
	xil_printf("\n\r");
}


// extern void tmc5240_readWriteSPI(uint16_t icID, uint8_t *data, size_t dataLength);
// extern bool tmc5240_readWriteUART(uint16_t icID, uint8_t *data, size_t writeLength, size_t readLength);
// extern TMC5240BusType tmc5240_getBusType(uint16_t icID);
// extern uint8_t tmc5240_getNodeAddress(uint16_t icID);


// TMC5240BusType tmc5240_getBusType(uint16_t icID){
//    TMC5240BusType retval;
// //    xil_printf("TMC5240BusType: icID = 0x%04x\n\r", icID);
//    retval = IC_BUS_SPI;
//    return(retval);
// }

// void tmc5240_readWriteSPI(uint16_t icID, uint8_t *data, size_t dataLength)
// {
	
// }

// bool tmc5240_readWriteUART(uint16_t icID, uint8_t *data, size_t writeLength, size_t readLength)
// {
// 	return(TRUE);
// }

// uint8_t tmc5240_getNodeAddress(uint16_t icID)
// {
// 	return(0);
// }
// void initMotor(uint16_t icID)
// {
//   // Disable motor driver
//   tmc5240_writeRegister(icID, TMC5240_GCONF, 0x10024002); // writing value 0x10020002 = 268582914 = 0.0 to address 0 = 0x00(GCONF)

//   // Enable motor driver
//   tmc5240_writeRegister(icID, TMC5240_GCONF, 0x10020002);  // writing value 0x10020002 = 268566530 = 0.0 to address 0 = 0x00(GCONF)
// }