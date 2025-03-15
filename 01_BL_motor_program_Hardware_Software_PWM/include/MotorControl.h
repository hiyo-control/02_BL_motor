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
    vector<double> Kp_dq;
    vector<double> Ki_dq;
    vector<double> Idq_ref;

    vector<double> Idq = {0.0, 0.0};
    vector<double> Vuvw = {0.0, 0.0, 0.0};
    vector<double> Vdq = {0.0, 0.0};
    vector<double> Idq_diff = {0.0, 0.0};
    vector<double> Idq_diff_Integral = {0.0, 0.0};

    double getCompRotation();
	double getElecCompRotation();
    vector<double> ParkTransform(vector<double> Iuvw);
    vector<double> InverseParkTransform(vector<double> Vdq);
    vector<double> PIController(vector<double> Idq);
	double calc_mod(double a, double b);

public:
    MotorControl(bool debug);
    void begin();
    void Sensored120degControl();
    void Sensorless120degControl();
    void VectorControl();
};
#endif

