#include <thread>
#include <chrono>

#include "./include/AS5048A.h"
#include "./include/IHM07M1.h"

static const uint TICK_F = 7000; // [microseconds]

using namespace std;

AS5048A angleSensor(24, true);
IHM07M1 motorInverter(true);

int main(void)
{
    angleSensor.begin();
    motorInverter.begin();

    uint counter = 0;
    double deg = 0;

    cout << "Hello world" << endl;
    this_thread::sleep_for(chrono::milliseconds(1000));
    
    while(1)
    {
        deg = angleSensor.getRotationInDegrees();
        motorInverter.forcedCommutate(TICK_F, counter);

        cout << "degree=" << deg << ", counter=" << counter << endl;

        counter ++;
    };

    return 0;
}