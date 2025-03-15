#ifndef as5048_h
#define as5048_h

#include <iostream>
#include <cmath>
#include <stdint.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

class AS5048A
{
private:
	uint8_t cs;
	bool errorFlag;
	bool ocfFlag; // Avoid printing OCF flag everytime
	bool debug;
	int channel;
	int Pn;
	int16_t rotation_prev = 0;
	int round_cnt = 0;

	uint16_t spiCalcEvenParity(uint16_t);
	uint16_t read(uint16_t registerAddress);

public:
	/* Constructor */
	AS5048A(uint8_t cs, bool debug);
	void begin();
	int16_t getRawRotation();
	int16_t getRotation();
	double getMechAngleIndeg();
	double getMechAngleInrad();
	double getElecAngleInrad();
	double getMechCumulativeAngleIndeg();
	void getDiagnostic();
};
#endif