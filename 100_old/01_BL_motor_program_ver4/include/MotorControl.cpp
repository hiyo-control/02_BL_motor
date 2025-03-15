#include "MotorControl.h"

static const double AS5048A_RESOLUTION = 16384.0; // 14bit
static const double AS5048A_ANGLE_OFFSET = 1785.5;
static const double AS508A_ELEC_MAX_VALUE = 2306.0;
static const uint8_t cs_ch = 24;

static const double PI = 3.141592;

AS5048A angleSensor(cs_ch, true);
IHM07M1 motorInverter(true);
MCP3208 motorCurrent(true, 3.3);

MotorControl::MotorControl(bool debug)
{
    this->debug = debug;
    this->Kp_dq[0] = 0.5;
    this->Kp_dq[1] = 0.5;
    this->Ki_dq[0] = 1.0;
    this->Ki_dq[1] = 1.0;

    this->Iref_dq[0] = 0;
    this->Iref_dq[1] = 0.1;
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


void MotorControl::VectorControl()
{
    double angle_elec_rad = angleSensor.getElecAngleInrad();
    
    /*
    vector<double> I_uvw = motorCurrent.getUVWCurrent();

    vector<double> I_dq, Iref_dq;
    I_dq = ParkTransform(I_uvw, angle_elec_rad);

    vector<double> V_dq = PIController(this->Iref_dq, I_dq);
    */
   /*
    vector<double>V_dq = {-2.0, 6.0};
    
    vector<double> V_uvw = InverseParkTransform(V_dq, angle_elec_rad);

    motorInverter.outputUVWVoltage(V_uvw);
    */
}

vector<double> MotorControl::ParkTransform(vector<double> I_uvw, double angle_elec_rad)
{
    double Id, Iq;

    Id = sqrt(2)/3*( cos(angle_elec_rad)*I_uvw[0]+cos(angle_elec_rad-2*PI/3)*I_uvw[1]+cos(angle_elec_rad+2*PI/3)*I_uvw[2] );
    Iq = sqrt(2)/3*( cos(angle_elec_rad)*I_uvw[0]+cos(angle_elec_rad-2*PI/3)*I_uvw[1]+cos(angle_elec_rad+2*PI/3)*I_uvw[2] );

    return {Id, Iq};
}

vector<double> MotorControl::InverseParkTransform(vector<double> V_dq, double angle_elec_rad)
{
    double Vu, Vv, Vw;

    Vu = sqrt(2)/3*( cos(angle_elec_rad       )*V_dq[0]-sin(angle_elec_rad       )*V_dq[1] );
    Vv = sqrt(2)/3*( cos(angle_elec_rad-2*PI/3)*V_dq[0]-sin(angle_elec_rad-2*PI/3)*V_dq[1] );
    Vw = sqrt(2)/3*( cos(angle_elec_rad+2*PI/3)*V_dq[0]-sin(angle_elec_rad+2*PI/3)*V_dq[1] );

    return {Vu, Vv, Vw};
}

vector<double> MotorControl::PIController(vector<double> Iref_dq, vector<double> I_dq)
{
    double Vd, Vq;
    vector<double> I_dq_diff, I_dq_diff_Integral;

    I_dq_diff[0] = Iref_dq[0] - I_dq[0];
    I_dq_diff[1] = Iref_dq[1] - I_dq[1];

    I_dq_diff_Integral[0] = I_dq_diff[0] + I_dq_diff_Integral[0];
    I_dq_diff_Integral[1] = I_dq_diff[1] + I_dq_diff_Integral[1];

    Vd = this->Kp_dq[0]*I_dq_diff[0] + this->Ki_dq[0]*I_dq_diff_Integral[0];
    Vq = this->Kp_dq[1]*I_dq_diff[1] + this->Ki_dq[1]*I_dq_diff_Integral[1];

    return {Vd, Vq};
}


