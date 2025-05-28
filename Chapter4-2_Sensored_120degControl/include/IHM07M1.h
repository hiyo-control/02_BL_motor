#ifndef ihm07m1
#define ihm07m1

#include <iostream>
#include <wiringPiSPI.h>
#include <wiringPi.h>
#include <pigpio.h>

using namespace std;

// Define motor and driver parameter
static const int F_PWM = 95000; // 95kHz
static const int MOD_F = 500000; // 50%

class IHM07M1
{
private:
    bool debug;
    int counter;
    int Enable1, Enable2, Enable3;
    int PWM_Input1, PWM_Input2, PWM_Input3;
    int GB, DEB;

    void produceSignal(uint sector);

public:
    IHM07M1(bool debug);
    void begin();
    void OutputPWM(uint counter);
};

#endif