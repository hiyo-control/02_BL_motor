#include <thread>
#include <chrono>

#include "./include/AS5048A.h"
#include "./include/IHM07M1.h"
#include "./include/data_log.h"

static const uint TICK_F = 7000; // [microseconds]
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
    double deg = 0;

    cout << "Hello world" << endl;
    this_thread::sleep_for(chrono::milliseconds(1000));
    
    for(int i=0; i<500; i++)
    {
        deg = angleSensor.getRotationInDegrees();
        motorInverter.forcedCommutate(TICK_F, counter);

        angle_logging.angle_record(counter, deg);

        cout << "degree=" << deg << ", counter=" << counter << endl;

        counter ++;
    };

    angle_logging.close();

    return 0;
}