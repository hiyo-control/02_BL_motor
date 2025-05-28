#include "MotorControl.h"

static const double AS508A_ELEC_MAX_VALUE = 2306.0;
static const uint8_t AS5048A_cs_ch = 24;

IHM07M1 motorInverter(true);
AS5048A angleSensor(AS5048A_cs_ch, true);

MotorControl::MotorControl(bool debug)
{
    this->debug = debug;
    
    this->pole_pair = 7.0; // Number of pole pair of BL motor
    this->RawRotation_offset = 6238.5; // U-phase value
    this->elec_Angle_max_value = AS5048A_MAX_VALUE / this->pole_pair;
}

void MotorControl::begin()
{
    motorInverter.begin();
    angleSensor.begin();
}

double MotorControl::getCompRotation()
{
	double value_buf = angleSensor.getRawRotation() - this->RawRotation_offset;
	double value = calc_mod(value_buf, AS5048A_MAX_VALUE);

	return value;
}

double MotorControl::getElecCompRotation()
{
	double value_buf = getCompRotation();
	double value = calc_mod(value_buf, this->elec_Angle_max_value);

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

    // 1. U-pahse -> V-phase
    if(585.1 <= ElecCompRotation && ElecCompRotation < 975.2)
    {
        motorInverter.OutputPWM(1);
    }
    // 2. U-pahse -> V-phase
    else if(975.2 <= ElecCompRotation && ElecCompRotation < 1365.3)
    {
        motorInverter.OutputPWM(2);
    }
    // 3. V-pahse -> W-phase
    else if(1365.3 <= ElecCompRotation && ElecCompRotation < 1755.4)
    {
        motorInverter.OutputPWM(3);
    }
    // 4. V-pahse -> U-phase
    else if(1755.4 <= ElecCompRotation && ElecCompRotation < 2145.5)
    {
        motorInverter.OutputPWM(4);
    }
    // 5. W-pahse -> U-phase
    else if(2145.5 <= ElecCompRotation || ElecCompRotation < 195.0)
    {
        motorInverter.OutputPWM(5);
    }
    // 6. W-pahse -> V-phase
    else if(195.0 <= ElecCompRotation && ElecCompRotation < 585.1)
    {
        motorInverter.OutputPWM(6);
    }
}









