#include "MotorControl.h"

static const double AS5048A_RESOLUTION = 16384.0; // 14bit
static const double AS5048A_ANGLE_OFFSET = 1785.5;
static const double AS508A_ELEC_MAX_VALUE = 2306.0;
static const uint8_t cs_ch = 24;

AS5048A angleSensor(cs_ch, true);
IHM07M1 motorInverter(true);

MotorControl::MotorControl(bool debug)
{
    this->debug = debug;
}

void MotorControl::begin()
{
    this->counter = 0;
    angleSensor.begin();
    motorInverter.begin();
}

double MotorControl::getCompRotation()
{
	double value_buf = angleSensor.getRawRotation() - AS5048A_ANGLE_OFFSET;
	double value = calc_mod(value_buf, AS5048A_RESOLUTION);

	return value;
}

double MotorControl::getElecCompRotation()
{
	double value_buf = getCompRotation();
	double value = calc_mod(value_buf, AS508A_ELEC_MAX_VALUE);

	return value;
}

double MotorControl::calc_mod(double a, double b)
{
	double result = fmod(a, b);
	if(result < 0)
	{
		result += abs(b);
	}
	return result;
}

void MotorControl::Sensored120degControl()
{
    double ElecCompRotation = getElecCompRotation();
    if(768.7 <= ElecCompRotation && ElecCompRotation < 1153)
    {
        motorInverter.produceSignal(1);
    }
    else if(1153 <= ElecCompRotation && ElecCompRotation < 1537.3)
    {
        motorInverter.produceSignal(2);
    }
    else if(1537.3 <= ElecCompRotation && ElecCompRotation < 1921.7)
    {
        motorInverter.produceSignal(3);
    }
    else if(1921.7 <= ElecCompRotation && ElecCompRotation < 2306)
    {
        motorInverter.produceSignal(4);
    }
    else if(0 <= ElecCompRotation && ElecCompRotation < 384.3)
    {
        motorInverter.produceSignal(5);
    }
    else if(384.3 <= ElecCompRotation && ElecCompRotation < 768.7)
    {
        motorInverter.produceSignal(6);
    }
}

void MotorControl::Sensorless120degControl()
{
    uint sector = this->counter % 6 + 1;
    
    motorInverter.produceSignal(sector);
}