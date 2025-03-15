#ifndef motor_control_h
#define motor_control_h

#include <cmath>
#include "AS5048A.h"
#include "IHM07M1.h"
#include "MCP3208.h"

class MotorControl
{
private:
    bool debug;
    int counter;
    vector<double> Kp_dq;
    vector<double> Ki_dq;
    vector<double> Iref_dq;
    
    double getCompRotation();
	double getElecCompRotation();
    vector<double> ParkTransform(vector<double> i_uvw, double angle_elec_rad);
    vector<double> InverseParkTransform(vector<double> V_dq, double angle_elec_rad);
    vector<double> PIController(vector<double> Iref_dq, vector<double> I_dq);
	double calc_mod(double a, double b);

public:
    MotorControl(bool debug);
    void begin();
    void Sensored120degControl();
    void Sensorless120degControl();
    void VectorControl();
};
#endif

