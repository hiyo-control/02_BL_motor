#ifndef ihm07m1
#define ihm07m1

#include <iostream>
#include <vector>
//#include <wiringPi.h>
//#include <wiringPiSPI.h>
#include <pigpio.h>

using namespace std;

class IHM07M1
{
private:
    bool debug;
    
public:
    IHM07M1(bool debug);
    void begin();
    void forcedCommutate(uint tick_f, uint counter);
    void produceSignal(uint sector);
    void outputUVWVoltage(vector<double> Vuvw);
    void outputPWM(int IN_No, double voltage);
};

#endif