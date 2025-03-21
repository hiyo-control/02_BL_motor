#include <thread>
#include <chrono>
#include "./include/AS5048A.h"

static const uint8_t cs_ch = 24;

using namespace std;

AS5048A angleSensor(cs_ch, true);

void getMotorAngle()
{
    while(true)
    {
        this_thread::sleep_for(chrono::milliseconds(100));

        double MotorAngle = angleSensor.getMechAngleIndeg();
        //double MotorAngle = angleSensor.getMechCumulativeAngleIndeg();
        cout << "Motor Angle = " << MotorAngle << " [deg]" << endl;
    }
}

int main()
{
    angleSensor.begin();

    thread MotorAngleInterrupt(getMotorAngle);
    MotorAngleInterrupt.join();
}