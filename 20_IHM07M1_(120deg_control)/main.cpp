#include <thread>
#include <chrono>

#include "./include/AS5048A.h"
#include "./include/IHM07M1.h"

static const uint TICK_F = 7000; // [microseconds]
static const uint8_t cs_ch = 24;

using namespace std;

AS5048A angleSensor(cs_ch, true);
IHM07M1 motorInverter(true);

void getMotorAngle()
{
    while(true)
    {
        this_thread::sleep_for(chrono::seconds(1));

        double MotorAngle = angleSensor.getMechAngleIndeg();
        cout << "Motor Angle = " << MotorAngle << " [deg]" << endl;
    }
}

void Sensored120degControl_Loop()
{
    while(true)
    {
        //this_thread::sleep_for(chrono::microseconds(7000));
        //this_thread::sleep_for(chrono::milliseconds(6));
        this_thread::sleep_for(chrono::milliseconds(6));
        motorInverter.Sensorless120degControl();
    }
}

int main()
{
    cout << "Start motor rotate" << endl;

    angleSensor.begin();
    motorInverter.begin();

    thread rotateMotorInterrupt(Sensored120degControl_Loop);
    thread MotorAngleInterrupt(getMotorAngle);

    rotateMotorInterrupt.join();
    MotorAngleInterrupt.join();





    /*
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
    */
}