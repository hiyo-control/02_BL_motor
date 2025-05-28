#ifndef mcp3208_h
#define mcp3208_h

#include <iostream>
#include <vector>
#include <cmath>
#include <stdint.h>
#include <wiringPiSPI.h>
#include <wiringPi.h>
#include "AS5048A.h"

//const double PI = 3.141592653589793;

// ADC3208 parameter
static const double MCP3208_max_bit = 4096.0; // 12bit

using namespace std;

class MCP3208
{
private:
	uint8_t cs;
	double Vref;
	int spi_ch;

	// LPF parameter
	double cut_off_freq, LPF_coef;
	int filter_cycle_microsec;
	double UVoltage_LPF     , VVoltage_LPF     , WVoltage_LPF;
	double UVoltage_LPF_prev, VVoltage_LPF_prev, WVoltage_LPF_prev;

	// MCP3208 channel
	int mcp3208_Uch, mcp3208_Vch, mcp3208_Wch;

	// ACS712 parameter
	double a_Uph, a_Vph, a_Wph;
	double b_Uph, b_Vph, b_Wph;

	int readADC(int mcp3208_ch);
	double getRawVoltage(int mcp3208_ch);
	double getLPFVoltage(int mcp3208_ch);
	
public:
	/* Constructor */
	MCP3208(uint8_t cs, double Vref, int filter_cycle_microsec);
	void begin();
	void calcLPF_UVoltage();
	void calcLPF_VVoltage();
	void calcLPF_WVoltage();
	
	double getRaw_UCurrent();
	double getRaw_VCurrent();
	double getRaw_WCurrent();

	double getLPF_UCurrent();
	double getLPF_VCurrent();
	double getLPF_WCurrent();

};
#endif