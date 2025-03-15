#include "AS5048A.h"

using namespace std;

// AS5048A code
static const uint16_t AS5048A_ANGLE_CODE = 0x3FFF;
static const uint16_t AS5048A_DIAG_AGC_CODE = 0x3FFD;

// AS5048A parameter
static const double AS5048A_MAX_VALUE = 8191.0;
static const double PI = 3.141592;

// Constructor
AS5048A::AS5048A(uint8_t cs, bool debug)
{
	this->cs = cs;
	this->debug = debug;
	this->errorFlag = false;
	this->ocfFlag = false;
	this->Pn = 14;
}

void AS5048A::begin()
{
	wiringPiSetup();
	
	this->channel = 0; /* SPI channel */
	int speed = 1000000; /* communication speed 1MHz */
	int mode = 1; /* SPI mode */
	
	if (wiringPiSPISetupMode(this->channel, speed, mode) < 0)
	{
		//cerr << "Can not open SPI device" << endl;
	}

	wiringPiSetupGpio();

	pinMode(this->cs, OUTPUT);
	digitalWrite(this->cs, 1);
}

uint16_t AS5048A::spiCalcEvenParity(uint16_t value)
{
    uint16_t cnt = 0;

    for (uint8_t i=0; i<16; i++)
    {
        if (value & 0x1)
        {
            cnt++;
        }
        value >>= 1;
    }

    return cnt & 0x1;
}

uint16_t AS5048A::read(uint16_t registerAddress)
{
	uint16_t command = 0x4000; // PAR=0 R/W=R
	command = command | registerAddress;

	//Add a parity bit on the the MSB
	command |= static_cast<uint16_t>(spiCalcEvenParity(command) << 0xF);

	unsigned char dataRW_upper = static_cast<unsigned char>((command >> 8) & 0xFF);
	unsigned char dataRW_lower = static_cast<unsigned char>(command & 0xFF);
	unsigned char dataRW[2]    = {dataRW_lower, dataRW_upper};

	if (this->debug)
	{
		/*
		cout << "command:0x" << bitset<16>(command) << endl;
		cout << "dataRW[0]:0x" << bitset<8>(dataRW[0]) << endl;
		cout << "dataRW[1]:0x" << bitset<8>(dataRW[1]) << endl;
		*/
	}

	//Send the command
	digitalWrite(this->cs, LOW);
	if (wiringPiSPIDataRW(this->channel, dataRW, sizeof(dataRW)) == -1)
	{
		//cerr << "SPI communication failed." << endl;
		return 1;
	};
	digitalWrite(this->cs, HIGH);

	uint16_t response_upper = (static_cast<uint16_t>(dataRW[0] << 8));
	uint16_t response_lower = (static_cast<uint16_t>(dataRW[1]));
	uint16_t response       = response_upper | response_lower;

	if (this->debug)
	{
		/*
		cout << "response:0x" << bitset<16>(response) << endl;
		cout << "dataRW[0]:0x" << bitset<8>(dataRW[0]) << endl;
		cout << "dataRW[1]:0x" << bitset<8>(dataRW[1]) << endl;
		*/
	}

	//Check if the error bit is set
	if (response & 0x4000)
	{
		if (this->debug)
		{
			//cerr << "Setting error bit" << endl;
		}
		this->errorFlag = true;
	}
	else
	{
		this->errorFlag = false;
	}

	//Return the data, stripping the parity and error bits
	return response & ~0xC000;
}

int16_t AS5048A::getRawRotation()
{
	return AS5048A::read(AS5048A_ANGLE_CODE);
}

int16_t AS5048A::getRotation()
{
	uint16_t data;
	int16_t rotation;

	data = AS5048A::getRawRotation();

	rotation = static_cast<int16_t>(data);
	if(rotation > AS5048A_MAX_VALUE)
	{
		rotation = -((0x3FFF)-rotation);
	}

	return rotation;
}

double AS5048A::getMechAngleIndeg()
{
	int16_t rotation = getRotation();
	double degrees = 360.0 * (rotation + AS5048A_MAX_VALUE) / (AS5048A_MAX_VALUE * 2.0);
	return degrees;
}

double AS5048A::getMechAngleInrad()
{
	int16_t rotation = getRotation();
	double radian = 2*PI * (rotation + AS5048A_MAX_VALUE) / (AS5048A_MAX_VALUE * 2.0);
	return radian;
}

double AS5048A::getElecAngleInrad()
{
	double angle_mech_rad = getMechAngleInrad();
	double angle_elec_rad = angle_mech_rad * this->Pn/2;

	return angle_elec_rad;
}

double AS5048A::getMechCumulativeAngleIndeg()
{
	int16_t rotation = getRotation();
	int16_t rotation_diff = rotation - rotation_prev;

	// Count round up
	if (rotation_diff > AS5048A_MAX_VALUE)
	{
		round_cnt--;
	}
	else if (rotation_diff < -AS5048A_MAX_VALUE)
	{
		round_cnt++;
	}

	rotation_prev = rotation;

	double degrees_buf = 360.0 * (rotation + AS5048A_MAX_VALUE) / (AS5048A_MAX_VALUE * 2.0);
	double degrees = degrees_buf + 360.0*round_cnt;

	return degrees;
}

void AS5048A::getDiagnostic()
{
	uint16_t data = AS5048A::read(AS5048A_DIAG_AGC_CODE);
	cout << "diagnostic : " << data << endl;
}
