#ifndef as5048_h
#define as5048_h

//#include <SPI.h>
#include <iostream>
#include <stdint.h>
#include <wiringPiSPI.h>

class AS5048A{
private:
	uint8_t _cs;
	bool errorFlag;
	bool ocfFlag; // Avoid printing OCF flag everytime
	uint16_t position;
	bool debug;
	uint8_t esp32_delay;

public:
	/**
	 *	Constructor
	 */
	AS5048A(uint8_t arg_cs, bool debug = false);

};
#endif