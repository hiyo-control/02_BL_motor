#include "MotorControl.h"

static const double AS5048A_RESOLUTION = 16384.0; // 14bit
static const double AS5048A_ANGLE_OFFSET = 1785.5;
static const double AS508A_ELEC_MAX_VALUE = 2306.0;
static const uint8_t AS5048A_cs_ch = 24;
static const uint8_t MCP3208_cs_ch = 26;

IHM07M1 motorInverter(true);
AS5048A angleSensor(AS5048A_cs_ch, true);
MCP3208 motorCurrent(MCP3208_cs_ch);

MotorControl::MotorControl(bool debug)
{
    this->debug = debug;
}

void MotorControl::begin()
{
    this->counter = 0;
    
    motorInverter.begin();
    angleSensor.begin();
    motorCurrent.begin();
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

void MotorControl::VectorControl()
{
    double angle_elec_rad = angleSensor.getElecAngleInrad();

    //double UCur = motorCurrent.getUCurrent();
    //double VCur = motorCurrent.getVCurrent();
    //double WCur = motorCurrent.getWCurrent();


    //vector<double> Vdq = {-2.0, 8.0};
    //vector<double> Vdq = {-1.0, 4.0};

    //vector<double> Vdq = {-1.0, 6.0};
    vector<double> Vdq = {-2.0, 6.0};
    
    vector<double> Vuvw = InverseParkTransform(Vdq, angle_elec_rad);

    motorInverter.outputUVWVoltage(Vuvw);
}

vector<double> MotorControl::InverseParkTransform(vector<double> Vdq, double angle_elec_rad)
{
    double Vu, Vv, Vw;

    Vu = sqrt(2.0/3.0)*( cos(angle_elec_rad         )*Vdq[0]-sin(angle_elec_rad         )*Vdq[1] );
    Vv = sqrt(2.0/3.0)*( cos(angle_elec_rad-2*M_PI/3)*Vdq[0]-sin(angle_elec_rad-2*M_PI/3)*Vdq[1] );
    Vw = sqrt(2.0/3.0)*( cos(angle_elec_rad+2*M_PI/3)*Vdq[0]-sin(angle_elec_rad+2*M_PI/3)*Vdq[1] );

    return {Vu, Vv, Vw};
}

