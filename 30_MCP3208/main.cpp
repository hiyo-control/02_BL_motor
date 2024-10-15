#include <thread>
#include <chrono>
#include "./include/MCP3208.h"

using namespace std;

uint8_t cs_ch = 26;
double Vref = 3.28;

MCP3208 getCurrent(cs_ch, Vref);

void getUVWCurrent()
{
    while(true)
    {
        this_thread::sleep_for(chrono::seconds(1));

        double UCurrent = getCurrent.getUCurrent();
        double VCurrent = getCurrent.getVCurrent();
        double WCurrent = getCurrent.getWCurrent();

        cout << "U-phase current : " << UCurrent << endl;
        cout << "V-phase current : " << VCurrent << endl;
        cout << "W-phase current : " << WCurrent << endl;
        
    }
}

int main()
{
    cout << "Hello world" << endl;

    getCurrent.begin();

    thread getCurrentInterrupt(getUVWCurrent);
    getCurrentInterrupt.join();
}


