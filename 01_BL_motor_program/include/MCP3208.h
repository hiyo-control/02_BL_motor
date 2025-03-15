#ifndef mcp3208_h
#define mcp3208_h

#include <iostream>
#include <stdint.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

class MCP3208
{
private:
	uint8_t cs;
	double Vref;
	int spi_ch;

	double a_Uph;
	double b_Uph;
	double a_Vph;
	double b_Vph;

	int readADC(int mcp3208_ch);
	double getVoltage(int mcp3208_ch);
	double getShuntCurrent(double voltage);

public:
	/* Constructor */
	MCP3208(uint8_t cs);
	void begin();
	double getUCurrent();
	double getVCurrent();
	double getWCurrent();
};
#endif