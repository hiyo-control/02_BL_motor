#include <thread>
#include <chrono>
#include <atomic>
#include "./include/MotorControl.h"
#include "./include/DataLog.h"

static const int MotorRotateTime_second  = 10; // 10sec
static const int filter_cycle_microsec = 500; // microsec

using namespace std;

atomic<bool> stop_flag(false);
auto start = chrono::high_resolution_clock::now();

MotorControl motorControl(true);
DataLog getdata(true);

double gettime()
{
    auto getdata_time = chrono::high_resolution_clock::now();
    auto duration     = chrono::duration_cast<chrono::milliseconds>(getdata_time - start);
    
    double time_millisec = duration.count();
    double time_sec      = time_millisec / 1000.0; // millisec -> sec

    return time_sec;
}

void calcLPF_loop()
{
    while(!stop_flag)
    {
        this_thread::sleep_for(chrono::microseconds(filter_cycle_microsec));

        getdata.calcLPF();
    }
}

void datalog_loop()
{
    while(!stop_flag)
    {
        this_thread::sleep_for(chrono::milliseconds(20));

        double time_sec = gettime();
        getdata.MakeDataMatrix(time_sec);

        if (time_sec >= MotorRotateTime_second)
        {
            stop_flag = true;
            break;
        } 
    }
    
}

void Sensored120degControl_loop()
{
    while(!stop_flag)
    {
        this_thread::sleep_for(chrono::milliseconds(10));
        motorControl.Sensored120degControl();
    }
}

int main()
{
    motorControl.begin();
    getdata.begin();

    cout << "Start Motor rotate" << endl;

    thread MotorRotateInterrupt(Sensored120degControl_loop);
    thread DataLogInterrupt(datalog_loop);
    thread calcLPFInterrupt(calcLPF_loop);

    MotorRotateInterrupt.join();
    DataLogInterrupt.join();
    calcLPFInterrupt.join();

    cout << "End motor rotate" << endl;
    cout << "Make Data log" << endl;

    getdata.outputCSVfile();

    cout << "Completed" << endl;
    
}