#include <thread>
#include <chrono>
#include "./include/MCP3208.h"

using namespace std;

uint8_t cs_ch = 26;
double Vref = 5.35;
int filter_cycle_microsec = 10; // microsecond
int count = 1;

MCP3208 getCurrent(cs_ch, Vref, filter_cycle_microsec);

void calcLPF()
{
    while(true)
    {
        this_thread::sleep_for(chrono::microseconds(filter_cycle_microsec));
        
        getCurrent.calcLPF_UVoltage();
        getCurrent.calcLPF_VVoltage();
        getCurrent.calcLPF_WVoltage();
    }
}

void getUVWCurrent()
{
    while(true)
    {
        this_thread::sleep_for(chrono::seconds(1));

        double UCurrent =getCurrent.getLPF_UCurrent();
        double VCurrent =getCurrent.getLPF_VCurrent();
        double WCurrent =getCurrent.getLPF_WCurrent();

        cout << UCurrent << "," << VCurrent << "," << WCurrent << "," << endl;

        count ++;
    }
}

int main()
{
    getCurrent.begin();

    thread getCurrentInterrupt(getUVWCurrent);
    thread calcAverageInterrupt(calcLPF);

    getCurrentInterrupt.join();
    calcAverageInterrupt.join();
}


