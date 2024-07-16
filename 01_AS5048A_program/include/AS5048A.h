#ifndef as5048_h
#define as5048_h

#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <bitset>
#include <wiringPiSPI.h>
#include <wiringPi.h>

class AS5048A
{
private:
	uint8_t cs;
	bool errorFlag;
	bool ocfFlag; // Avoid printing OCF flag everytime
	uint16_t position;
	bool debug;
	uint8_t esp32_delay;
	int channel;

	uint16_t spiCalcEvenParity(uint16_t);
	uint16_t read(uint16_t registerAddress);

public:
	/*
	static AS5048A& Instance()
    {
        static AS5048A _instance;
        return _instance;
    }
	*/

	/* Constructor */
	AS5048A(uint8_t cs, bool debug);
	void begin();
	int16_t getRawRotation();
	int16_t getRotation();
	double getRotationInDegrees();

	uint16_t getState();
	void getDiagnostic();
};
#endif