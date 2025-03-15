#ifndef ihm07m1
#define ihm07m1

#include <iostream>
#include <vector>
#include <pigpio.h>

using namespace std;

class IHM07M1
{
private:
    bool debug;

    /*
    static const double offset_voltage = 7.5;
    static const double supply_voltage = 15.0;
    */
    
public:
    IHM07M1(bool debug);
    void begin();
    void forcedCommutate(uint tick_f, uint counter);
    void produceSignal(uint sector);
    void outputUVWVoltage(vector<double> Vuvw);
    void outputHardwarePWM(int IN_No, double voltage);
    void outputSoftwarePWM(int IN_No, double voltage);
};

#endif