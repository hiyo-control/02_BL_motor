#ifndef motor_control_h
#define motor_control_h

#include <cmath>
#include <vector>
#include "./AS5048A.h"
#include "./IHM07M1.h"
#include "./MCP3208.h"

using namespace std;

class MotorControl
{
private:
    bool debug;
    int counter;

    double getCompRotation();
	double getElecCompRotation();
    vector<double> InverseParkTransform(vector<double> Vdq, double angle_elec_rad);
	double calc_mod(double a, double b);

public:
    MotorControl(bool debug);
    void begin();
    void Sensored120degControl();
    void Sensorless120degControl();
    void VectorControl();
    double getUVoltage();
    double getVVoltage();
    double getWVoltage();
};
#endif

