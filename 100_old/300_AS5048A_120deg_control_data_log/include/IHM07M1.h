#ifndef ihm07m1
#define ihm07m1

#include <iostream>
#include <wiringPiSPI.h>
#include <wiringPi.h>
#include <pigpio.h>

class IHM07M1
{
private:
    bool debug;

public:
    IHM07M1(bool debug);
    void begin();
    void forcedCommutate(uint tick_f, uint counter);
    void produceSignal(uint sector);
};

#endif