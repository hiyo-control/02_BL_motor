#include <thread>
#include <chrono>
#include <atomic>
#include "./include/MotorControl.h"
#include "./include/DataLog.h"

static const int MotorRotateTime_second  = 10;

using namespace std;

auto start = chrono::high_resolution_clock::now();
atomic<bool> stop_flag(false);

MotorControl MtrCtrl(true);
DataLog getdata(true);

void Sensored120degControl_Loop()
{
    while(!stop_flag)
    {
        this_thread::sleep_for(chrono::microseconds(5));
        MtrCtrl.Sensored120degControl();
    }
}

void VectorControl_loop()
{
    while(!stop_flag)
    {
        this_thread::sleep_for(chrono::microseconds(10));
        MtrCtrl.VectorControl();
    }
}

void datalog_loop()
{
    while(!stop_flag)
    {
        this_thread::sleep_for(chrono::milliseconds(5));
        //this_thread::sleep_for(chrono::microseconds(100));

        auto getdata_time = chrono::high_resolution_clock::now();
        auto duration     = chrono::duration_cast<chrono::milliseconds>(getdata_time - start);
        double time_millisec = duration.count();
        double time_sec = time_millisec / 1000;

        getdata.MakeDataMatrix(time_sec);

        if (time_sec >= MotorRotateTime_second)
        {
            stop_flag = true;
            
            break;
        }
    }
}

int main()
{
    cout << "Start motor rotate" << endl;
    
    MtrCtrl.begin();
    getdata.begin();

    //thread rotateMotorInterrupt(Sensored120degControl_Loop);
    thread rotateMotorInterrupt(VectorControl_loop);
    thread datalogInterrupt(datalog_loop);

    rotateMotorInterrupt.join();
    datalogInterrupt.join();

    getdata.outputCSVfile();

    cout << "End motor rotate" << endl;
}