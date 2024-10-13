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
    int counter;

    void produceSignal(uint sector);

public:
    IHM07M1(bool debug);
    void begin();
    void Sensorless120degControl();
};

#endif