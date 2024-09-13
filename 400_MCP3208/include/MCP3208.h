#ifndef mcp3208_h
#define mcp3208_h

#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <bitset>
#include <wiringPiSPI.h>
#include <wiringPi.h>

class MCP3208
{
private:
	uint8_t cs;
	double Vref;
	int spi_ch;

public:
	/* Constructor */
	MCP3208(uint8_t cs, double Vref);
	void begin();
    int readADC(int mcp3208_ch);
	double getVoltage(int mcp3208_ch);
};
#endif