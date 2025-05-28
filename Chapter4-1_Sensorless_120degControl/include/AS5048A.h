#ifndef as5048_h
#define as5048_h

#include <iostream>
#include <stdint.h>
#include <wiringPiSPI.h>
#include <wiringPi.h>

// AS5048A code
static const uint16_t AS5048A_ANGLE_CODE = 0x3FFF;
static const uint16_t AS5048A_DIAG_CODE  = 0x3FFD;

// AS5048A parameter
static const double PI = 3.141592;
static const double AS5048A_MAX_VALUE      = 16384.0      ; // 2^14
static const double AS5048A_resolution_deg = 0.0219       ; // deg/LSB
static const double AS5048A_resolution_rad = 0.0219*PI/180; // rad/LSB

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
	int16_t rotation_initial = 0;
	int round_cnt = 0;

	uint16_t spiCalcEvenParity(uint16_t);
	uint16_t read(uint16_t registerAddress);
	

public:
	/* Constructor */
	AS5048A(uint8_t cs, bool debug);

	/* function */
	void begin();
	int16_t getRawRotation();
	double getMechAngleIndeg();
	double getMechAngleInrad();
	double getElecAngleInrad();
	double getMechCumulativeAngleIndeg();
	void getDiagnostic();
};
#endif