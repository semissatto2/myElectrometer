Basic operation:
- DDC112 indicates data is ready by taking DVALID LO
- When ready to retrieve the data, take DXMIT
LO to enable DOUT. DVALID now returns HI.
-  The DDC112 shifts data out on the falling edge of DCLK, therefore, read
DOUT on the rising edge of DCLK. When finished retrieving the data, return DXMIT HI.
- When not retrieving data, freeze DCLK to reduce digital noise coupling into the front-end integrators.
DCLK can be held either HI or LO when not in use. If held HI, it must be returned LO before DXMIT goes LO. 
If this condition is violated, bit 20 (MSB) will be lost and the first bit out will be bit 19
- You can read the data before, after, or before and after
CONV toggles as described in the last section on multiple
DDC112 systems. Never retrieve data while CONV toggles.
That is, make sure the serial interface is inactive at the
end and beginning of the integration period to prevent digital
noise coupling into the front end.

SPI operation:
At CPOL=0 the base value of the clock is zero, i.e. the idle state is 0 and active state is 1.
With CPOL=0 AND CPHA=0, data are captured on the clock's rising edge (low→high transition) and data is output on a falling edge (high→low clock transition).

https://developer.mbed.org/handbook/SPI
https://en.wikipedia.org/wiki/Serial_Peripheral_Interface_Bus

Firmware:
SPI spi(p5, p6, p7)		//MOSI, MISO, SCLK
DigitalIn DVALID		
DigitalOut DXMIT(p8)		//ChipSelect
DigitalOut Range2
DigitalOut Range1
DigitalOut Range0
unsigned int buffer[5]
unsigned int data[2]
int i = 0

Setup:
leRange() 			//Scan do teclado
setaRange(1 - 7)
	Case range = 1:
		Range0 = 1
		Range1 = 0
		Range2 = 0
	Case range = 2:
		Range0 = 0
		Range1 = 1
		Range2 = 0

lePeriodoIntegracao()		//Scan do teclado
ajudaPeriodoIntegracao()  	//O periodo de integracao deve ser multiplo
			 	//de 1/10MHZ = 100ns
geraSinalConv() 		//Gera PWM 50% Duty cycle. Periodo = 2*periodoIntegracao

DXMIT = 1			//Deixa DXMIT em estado ALTO para não habilitar DOUT
spi.format(8,0)			//Configura SPI modo 8 bits, mode 0 (ver referencia)
spi.frequency(8000000)		//Configura SCLK como 8 MHz (96/12 MHz)

Loop:
if (DVALID == HIGH):
	pass()

if (DVALID == LO):
	DXMIT = 0 		//Enable DOUT
	for (i = 0; i < 5; i++)
		buffer[i] = spi.write(0x00)
	DXMIT = 1		//Disable DOUT
	trataBuffer3()		//Deve-se retornar os 4 bits mais signicativos do Buffer3 e os 4 bits menos significativos do Buffer3
	data[0] = buffer[0]<<16 | buffer[1]<<8 | buffer[3]_MSB
	data[1] = buffer[3]_LSB<<16 | buffer[4] << 8 | buffer[5]
	pc.prinf("CH1 = %d\t CH2 = %d\t\n, data[0],data[1])
	data[1] = buffer[3]
