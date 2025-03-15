#include "MotorControl.h"

static const double AS5048A_RESOLUTION = 16384.0; // 14bit
static const double AS5048A_ANGLE_OFFSET = 1785.5;
static const double AS508A_ELEC_MAX_VALUE = 2306.0;
static const uint8_t AS5048A_cs_ch = 24;
static const uint8_t MCP3208_cs_ch = 26;

static const double PI = 3.141592;

IHM07M1 motorInverter(true);
AS5048A angleSensor(AS5048A_cs_ch, true);
MCP3208 motorCurrent(MCP3208_cs_ch);

MotorControl::MotorControl(bool debug)
{
    this->debug = debug;
    
    this->Kp_dq = {1.5, 1.5};
    this->Ki_dq = {1.0, 1.0};

    this->Idq_ref = {0.0, 4.0};
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
    double UCur = motorCurrent.getUCurrent();
    double VCur = motorCurrent.getVCurrent();
    double WCur = -(UCur + VCur);
    vector<double> Iuvw = {UCur, VCur, WCur};

    vector<double> Idq = ParkTransform(Iuvw);

    vector<double> Vdq = PIController(Idq);

    vector<double> Vuvw = InverseParkTransform(Vdq);

    motorInverter.outputUVWVoltage(Vuvw);
}

vector<double> MotorControl::ParkTransform(vector<double> Iuvw)
{
    double angle_elec_rad = angleSensor.getElecAngleInrad();

    Idq[0] = sqrt(2.0/3.0)*(  cos(angle_elec_rad)*Iuvw[0]+cos(angle_elec_rad-2*PI/3)*Iuvw[1]+cos(angle_elec_rad+2*PI/3)*Iuvw[2] );
    Idq[1] = sqrt(2.0/3.0)*( -sin(angle_elec_rad)*Iuvw[0]-sin(angle_elec_rad-2*PI/3)*Iuvw[1]-sin(angle_elec_rad+2*PI/3)*Iuvw[2] );

    return Idq;
}

vector<double> MotorControl::InverseParkTransform(vector<double> Vdq)
{
    double angle_elec_rad = angleSensor.getElecAngleInrad();
    
    Vuvw[0] = sqrt(2.0/3.0)*( cos(angle_elec_rad           )*Vdq[0]-sin(angle_elec_rad           )*Vdq[1] );
    Vuvw[1] = sqrt(2.0/3.0)*( cos(angle_elec_rad-2.0*PI/3.0)*Vdq[0]-sin(angle_elec_rad-2.0*PI/3.0)*Vdq[1] );
    Vuvw[2] = sqrt(2.0/3.0)*( cos(angle_elec_rad+2.0*PI/3.0)*Vdq[0]-sin(angle_elec_rad+2.0*PI/3.0)*Vdq[1] );

    return Vuvw;
}

vector<double> MotorControl::PIController(vector<double> Idq)
{
    /*
    Idq_diff[0] = this->Idq_ref[0] - Idq[0];
    Idq_diff[1] = this->Idq_ref[1] - Idq[1];

    Idq_diff_Integral[0] = 0;
    Idq_diff_Integral[1] = 0;

    Vdq[0] = this->Kp_dq[0]*Idq_diff[0] + this->Ki_dq[0]*Idq_diff_Integral[0];
    Vdq[1] = this->Kp_dq[1]*Idq_diff[1] + this->Ki_dq[1]*Idq_diff_Integral[1];
    */

    //Vdq = {0.0, 6.0};
    //Vdq = {-0.01, 0.5};
    //Vdq = {-0.02, 1.0};
    Vdq = {-0.02, 3.5};    

    return Vdq;
}


