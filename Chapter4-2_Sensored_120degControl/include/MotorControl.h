#ifndef motor_control_h
#define motor_control_h

#include <cmath>
//#include <vector>
#include "AS5048A.h"
#include "IHM07M1.h"

using namespace std;

class MotorControl
{
private:
    bool debug;
    double pole_pair;
    double RawRotation_offset;
    double elec_Angle_max_value;

    double getCompRotation();
	double getElecCompRotation();
	double calc_mod(double a, double b);

public:
    MotorControl(bool debug);
    void begin();
    void Sensored120degControl();

};
#endif

