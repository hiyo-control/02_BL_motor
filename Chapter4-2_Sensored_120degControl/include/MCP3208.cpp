#include "MCP3208.h"

using namespace std;

/* Constructor */
MCP3208::MCP3208(uint8_t cs, double Vref, int filter_cycle_microsec)
{
	this->cs = cs;
	this->Vref = Vref;

    // LPF parameter
    this->filter_cycle_microsec = filter_cycle_microsec;
    this->cut_off_freq = 100.0; // cut off frequency [Hz]

    // MCP3208 channel
    this->mcp3208_Uch = 0;
    this->mcp3208_Vch = 1;
    this->mcp3208_Wch = 2;

    // ACS712 parameter
    this->a_Uph = -0.00109; // [V/mA]
    this->b_Uph =  2.57   ; // [V]
    this->a_Vph = -0.00204; // [V/mA]
    this->b_Vph =  2.56   ; // [V]
    this->a_Wph =  1.0    ; // [V/mA]
    this->b_Wph =  0.0    ; // [V]
}

void MCP3208::begin()
{
    // Initialize SPI communication
    wiringPiSetup();

    this->spi_ch = 1;
    int speed = 1000000; //1MHz
    int mode = 0;

    if (wiringPiSPISetupMode(this->spi_ch, speed, mode) < 0)
    {
        cerr << "Can not open SPI device" << endl;
    }

    wiringPiSetupGpio();

    pinMode(this->cs,OUTPUT);
    digitalWrite(this->cs, 1);

    // Initialize LPF
    double tau           = 1/(2*PI*this->cut_off_freq);
    double sampling_time = static_cast<double>(this->filter_cycle_microsec) / 1000.0 / 1000.0;

    LPF_coef = tau / (tau+sampling_time);

    UVoltage_LPF_prev = getRawVoltage(mcp3208_Uch);
    VVoltage_LPF_prev = getRawVoltage(mcp3208_Vch);
    WVoltage_LPF_prev = getRawVoltage(mcp3208_Wch);
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
		cerr << "SPI communication failed." << endl;
		return 1;
	};
	digitalWrite(this->cs, HIGH);

    // 12ビットの結果を取得
    int result = ((dataRW[1] & 0x0F) << 8) | dataRW[2];
    return result;
}

double MCP3208::getRawVoltage(int mcp3208_ch)
{
    int adcValue_buf = MCP3208::readADC(mcp3208_ch);
    double adcValue  = static_cast<double>(adcValue_buf);
    double voltage   = (adcValue * this->Vref) / MCP3208_max_bit;

    return voltage;
}

void MCP3208::calcLPF_UVoltage()
{
    double voltage_Raw = getRawVoltage(mcp3208_Uch);
    UVoltage_LPF       = LPF_coef*UVoltage_LPF_prev + (1-LPF_coef)*voltage_Raw;
    UVoltage_LPF_prev  = UVoltage_LPF;
}

void MCP3208::calcLPF_VVoltage()
{
    double voltage_Raw = getRawVoltage(mcp3208_Vch);
    VVoltage_LPF       = LPF_coef*VVoltage_LPF_prev + (1-LPF_coef)*voltage_Raw;
    VVoltage_LPF_prev  = VVoltage_LPF;
}

void MCP3208::calcLPF_WVoltage()
{
    double voltage_Raw = getRawVoltage(mcp3208_Wch);
    WVoltage_LPF       = LPF_coef*WVoltage_LPF_prev + (1-LPF_coef)*voltage_Raw;
    WVoltage_LPF_prev  = WVoltage_LPF;
}

double MCP3208::getRaw_UCurrent()
{
    double voltage   = getRawVoltage(this->mcp3208_Uch);
    double I_Uph_buf = (voltage-this->b_Uph) / this->a_Uph; // [mA]
    double I_Uph      = I_Uph_buf / 1000.0; // [A]

    return I_Uph;
}

double MCP3208::getRaw_VCurrent()
{
    double voltage = getRawVoltage(this->mcp3208_Vch);
    double I_Vph_buf = (voltage-this->b_Vph) / this->a_Vph; // [mA]
    double I_Vph = I_Vph_buf / 1000.0; // [A]

    return I_Vph;
}

double MCP3208::getRaw_WCurrent()
{
    double voltage   = getRawVoltage(this->mcp3208_Wch);
    double I_Wph_buf = (voltage-this->b_Wph) / this->a_Wph; // [mA]
    double I_Wph     = I_Wph_buf / 1000.0; // [A]

    return I_Wph;
}

double MCP3208::getLPF_UCurrent()
{
    double voltage   = UVoltage_LPF;
    double I_Uph_buf = (voltage-this->b_Uph) / this->a_Uph; // [mA]
    double I_Uph     = I_Uph_buf / 1000.0; // [A]

    return I_Uph;
}

double MCP3208::getLPF_VCurrent()
{
    double voltage   = VVoltage_LPF;
    double I_Vph_buf = (voltage-this->b_Vph) / this->a_Vph; // [mA]
    double I_Vph     = I_Vph_buf / 1000.0; // [A]

    return I_Vph;
}

double MCP3208::getLPF_WCurrent()
{
    double voltage   = WVoltage_LPF;
    double I_Wph_buf = (voltage-this->b_Wph) / this->a_Wph; // [mA]
    double I_Wph     = I_Wph_buf / 1000.0; // [A]
    
    return I_Wph;
}



