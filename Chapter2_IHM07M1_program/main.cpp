#include <thread>
#include <chrono>
#include "./include/IHM07M1.h"

using namespace std;

IHM07M1 motorInverter(true);

int counter = 0;
string enter;

int main()
{
    cout << "Start PWM output from Inverter" << endl;

    motorInverter.begin();
    cout << "Push Enter key" << endl;

    while(true)
    {
        getline(cin, enter);

        cout << "Count is " << counter << endl;
        motorInverter.OutputPWM(counter);

        counter ++;
    }
}