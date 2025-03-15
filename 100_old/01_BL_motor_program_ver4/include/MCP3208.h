#ifndef mcp3208_h
#define mcp3208_h

#include <vector>
#include <iostream>
#include <stdint.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

using namespace std;

class MCP3208
{
private:
	uint8_t cs;
	double Vref;
	int spi_ch;

	int readADC(int mcp3208_ch);
	double getVoltage(int mcp3208_ch);
	double getShuntCurrent(double voltage);

public:
	/* Constructor */
	MCP3208(uint8_t cs, double Vref);
	void begin();
	double getUCurrent();
	double getVCurrent();
	double getWCurrent();
	vector<double> getUVWCurrent();
};
#endif