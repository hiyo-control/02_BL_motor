#include <thread>
#include <chrono>

#include "./include/AS5048A.h"
#include "./include/IHM07M1.h"
#include "./include/data_log.h"

static const uint TICK_F = 7000; // [microseconds]
//static const uint TICK_F = 6000; // [microseconds]
static const uint8_t cs_ch = 24;

using namespace std;

AS5048A angleSensor(cs_ch, true);
IHM07M1 motorInverter(true);
data_log angle_logging(true);

int main(void)
{
    angleSensor.begin();
    motorInverter.begin();
    angle_logging.begin();

    uint counter = 0;
    int16_t value_RawRotation = 0;
    int16_t value_Rotation = 0;
    double value_CompRotation = 0;
    double value_ElecCompRotation = 0;

    double Mech_deg = 0;
    double Mech_Comp_deg = 0;

    cout << "Hello world" << endl;
    this_thread::sleep_for(chrono::milliseconds(1000));
    
    for(int i=0; i<1000; i++)
    {
        value_RawRotation  = angleSensor.getRawRotation();
        value_Rotation = angleSensor.getRotation();
        value_CompRotation = angleSensor.getCompRotation();
        value_ElecCompRotation = angleSensor.getElecCompRotation();

        Mech_deg = angleSensor.getMechAngleIndeg();
        //Mech_Comp_deg = angleSensor.getMechCompAngleIndeg();

        motorInverter.forcedCommutate(TICK_F, counter);

        angle_logging.angle_record(counter, value_RawRotation, value_Rotation, value_CompRotation, value_ElecCompRotation, Mech_deg, Mech_Comp_deg);

        //cout << "degree=" << deg << ", counter=" << counter << endl;

        //this_thread::sleep_for(chrono::milliseconds(7));
        //this_thread::sleep_for(chrono::milliseconds(15));

        counter ++;
    };

    angle_logging.close();

    cout << "End" << endl;

    return 0;
}