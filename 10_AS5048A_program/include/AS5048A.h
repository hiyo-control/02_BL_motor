#ifndef as5048_h
#define as5048_h

#include <iostream>
#include <stdint.h>
#include <wiringPiSPI.h>
#include <wiringPi.h>

class AS5048A
{
private:
	uint8_t cs;
	bool errorFlag;
	bool ocfFlag; // Avoid printing OCF flag everytime
	bool debug;
	int channel;

	uint16_t spiCalcEvenParity(uint16_t);
	uint16_t read(uint16_t registerAddress);

public:
	/* Constructor */
	AS5048A(uint8_t cs, bool debug);
	void begin();
	int16_t getRawRotation();
	int16_t getRotation();
	double getMechAngleIndeg();
	void getDiagnostic();
};
#endif