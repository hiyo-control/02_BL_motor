#include <thread>
#include <chrono>
#include <atomic>
#include "./include/IHM07M1.h"
#include "./include/DataLog.h"

int counter = 0;
atomic<bool> stop_flag(false);
static const int MotorRotateTime_second  = 120; // 2min
auto start = chrono::high_resolution_clock::now();

using namespace std;

IHM07M1 motorInverter(true);
DataLog getdata(true);

double gettime()
{
    auto getdata_time = chrono::high_resolution_clock::now();
    auto duration     = chrono::duration_cast<chrono::milliseconds>(getdata_time - start);
    
    double time_millisec = duration.count();
    double time_sec      = time_millisec / 1000.0; // millisec -> sec

    return time_sec;
}

void motorRotate()
{
    while(!stop_flag)
    {
        this_thread::sleep_for(chrono::seconds(1));

        double time_sec = gettime();

        motorInverter.OutputPWM(counter);
        getdata.MakeDataMatrix(counter);

        if (time_sec >= MotorRotateTime_second)
        {
            stop_flag = true;
            
            break;
        }

        counter++;
    }
}

int main()
{
    motorInverter.begin();
    getdata.begin();

    cout << "Start PWM output from Inverter" << endl;

    thread MotorRotateInterrupt(motorRotate);
    MotorRotateInterrupt.join();

    getdata.outputCSVfile();

    cout << "End motor rotate" << endl;
}