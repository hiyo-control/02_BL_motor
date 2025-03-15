#include "MCP3208.h"

static const int mcp3208_Uch = 0;
static const int mcp3208_Vch = 1;
static const int mcp3208_Wch = 2;
static const double a_sensor = 1.527778;
static const double b_sensor = 1.58333;
static const double R_shunt = 0.33; // shunt resitance [Ohm]

using namespace std;

/* Constructor */
MCP3208::MCP3208(uint8_t cs, double Vref)
{
	this->cs = cs;
	this->Vref = Vref;
}

void MCP3208::begin()
{
    wiringPiSetup();

    this->spi_ch = 1;
    int speed = 1000000;
    int mode = 0;

    if (wiringPiSPISetupMode(this->spi_ch, speed, mode) < 0)
    {
        //cerr << "Can not open SPI devide" << endl;
    }

    wiringPiSetupGpio();

    pinMode(this->cs,OUTPUT);
    digitalWrite(this->cs, 1);
}

int MCP3208::readADC(int mcp3208_ch)
{
    if (mcp3208_ch < 0 || mcp3208_ch > 7)
    {
        return -1;  // 無効なチャンネルチェック
    }

    unsigned char dataRW[3];
    dataRW[0] = 0x06 | ((mcp3208_ch & 0x07) >> 2);
    dataRW[1] = 0x00 | ((mcp3208_ch & 0x07) << 6);
    dataRW[2] = 0x00;

    digitalWrite(this->cs, LOW);
	if (wiringPiSPIDataRW(this->spi_ch, dataRW, sizeof(dataRW)) == -1)
	{
		//cerr << "SPI communication failed." << endl;
		return 1;
	};
	digitalWrite(this->cs, HIGH);

    // 12ビットの結果を取得
    int result = ((dataRW[1] & 0x0F) << 8) | dataRW[2];
    return result;
}

double MCP3208::getVoltage(int mcp3208_ch)
{
    int adcValue = MCP3208::readADC(mcp3208_ch);
    double voltage = (adcValue * this->Vref) / 4096.0;

    return voltage;
}

double MCP3208::getShuntCurrent(double voltage)
{
    double V_shunt = (voltage-b_sensor) / a_sensor;
    double I_shunt = V_shunt/R_shunt;

    return I_shunt;
}

double MCP3208::getUCurrent()
{
    double voltage = getVoltage(mcp3208_Uch);
    double current = getShuntCurrent(voltage);

    return current;
}

double MCP3208::getVCurrent()
{
    double voltage = getVoltage(mcp3208_Vch);
    double current = getShuntCurrent(voltage);
    
    return current;
}

double MCP3208::getWCurrent()
{
    double voltage = getVoltage(mcp3208_Wch);
    double current = getShuntCurrent(voltage);
    
    return current;
}

vector<double> MCP3208::getUVWCurrent()
{
    vector<double> UVWCurrent;
    UVWCurrent[0] = getUCurrent();
    UVWCurrent[1] = getVCurrent();
    UVWCurrent[2] = getWCurrent();

    return UVWCurrent;
}



