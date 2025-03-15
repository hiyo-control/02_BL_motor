#ifndef motor_control_h
#define motor_control_h

#include <cmath>
#include "./AS5048A.h"
#include "./IHM07M1.h"

class MotorControl
{
private:
    bool debug;
    int counter;

    double getCompRotation();
	double getElecCompRotation();
	double calc_mod(double a, double b);

public:
    MotorControl(bool debug);
    void begin();
    void Sensored120degControl();
    void Sensorless120degControl();
    void VectorControl();
};
#endif

