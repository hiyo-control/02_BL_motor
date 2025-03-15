#ifndef as5048_h
#define as5048_h

#include <iostream>
#include <stdint.h>
#include <wiringPiSPI.h>
#include <wiringPi.h>

// AS5048A code
static const uint16_t AS5048A_ANGLE_CODE = 0x3FFF;
static const uint16_t AS5048A_DIAG_AGC_CODE = 0x3FFD;

// AS5048A parameter
static const double AS5048A_MAX_VALUE = 8191.0;
static const double PI = 3.141592;

class AS5048A
{
private:
	uint8_t cs;
	bool errorFlag;
	bool ocfFlag; // Avoid printing OCF flag everytime
	bool debug;
	int channel;
	double Pn;
	int16_t rotation_prev = 0;
	int round_cnt = 0;

	uint16_t spiCalcEvenParity(uint16_t);
	uint16_t read(uint16_t registerAddress);
	int16_t getRawRotation();
	int16_t getRotation();

public:
	/* Constructor */
	AS5048A(uint8_t cs, bool debug);

	/* function */
	void begin();
	double getMechAngleIndeg();
	double getMechAngleInrad();
	double getElecAngleInrad();
	double getMechCumulativeAngleIndeg();
	void getDiagnostic();
};
#endif