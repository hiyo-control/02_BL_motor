#include "AS5048A.h"

using namespace std;

// Constructor
AS5048A::AS5048A(uint8_t cs, bool debug)
{
	this->cs = cs;
	this->debug = debug;
	this->errorFlag = false;
	this->Pn = 14.0; // Number of Pole of BL motor
}

void AS5048A::begin()
{
	wiringPiSetup();
	
	this->channel = 0; /* SPI channel */
	int speed = 1000000; /* communication speed 1MHz */
	int mode = 1; /* SPI mode */
	
	if (wiringPiSPISetupMode(this->channel, speed, mode) < 0)
	{
		cerr << "Can not open SPI device" << endl;
	}

	wiringPiSetupGpio();

	pinMode(this->cs, OUTPUT);
	digitalWrite(this->cs, 1);

	rotation_initial = getRawRotation();
	rotation_prev = rotation_initial;
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
		cerr << "SPI communication failed." << endl;
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
			cerr << "Setting error bit" << endl;
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

double AS5048A::getMechAngleIndeg()
{
	int16_t rotation_buf = getRawRotation();
	double rotation      = static_cast<double>(rotation_buf);
	double degrees       = AS5048A_resolution_deg * rotation;

	return degrees;
}

double AS5048A::getMechAngleInrad()
{
	int16_t rotation_buf = getRawRotation();
	double rotation      = static_cast<double>(rotation_buf);
	double radian        = AS5048A_resolution_rad * rotation;

	return radian;
}


double AS5048A::getMechCumulativeAngleIndeg()
{
	int16_t rotation = getRawRotation();
	int16_t rotation_diff = rotation - rotation_prev;

	// Count round up
	if (rotation_diff > 0.5*AS5048A_MAX_VALUE)
	{
		round_cnt--;
	}
	else if (rotation_diff < -0.5*AS5048A_MAX_VALUE)
	{
		round_cnt++;
	}

	rotation_prev = rotation;

	double degrees_buf = AS5048A_resolution_deg * (rotation - rotation_initial);
	double degrees     = degrees_buf + 360.0*round_cnt;

	return degrees;
}


double AS5048A::getElecAngleInrad()
{
	double angle_mech_rad = getMechAngleInrad();
	double angle_elec_rad = angle_mech_rad * this->Pn/2.0;

	return angle_elec_rad;
}

void AS5048A::getDiagnostic()
{
	uint16_t data = AS5048A::read(AS5048A_DIAG_CODE);
	cout << "diagnostic : " << data << endl;
}
