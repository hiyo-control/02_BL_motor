#include "IHM07M1.h"

// Define motor and driver parameter
//static const int F_PWM = 100000; //100kHz
static const int F_PWM = 95000; //95kHz
//static const int F_PWM = 90000; //90kHz
//static const int F_PWM = 80000; //80kHz

static const int MOD_F = 500000;

static const double Max_voltage = 7.5;
static const double Min_voltage = -7.5;
static const double offset_voltage = 7.5;
static const double supply_voltage = 15.0;

// Define GIO pins
#define EN1 16
#define EN2 20
#define EN3 21

#define IN1 12 // PWM Channel 0
#define IN2 19 // PWM Channel 1
#define IN3 18 // PWM Channel 0

#define SEC1 25
#define GB 0
#define DEB 1

using namespace std;

// Constructor
IHM07M1::IHM07M1(bool debug)
{
    this->debug = debug;
}

void IHM07M1::begin()
{
    /* Initialize GPIO */
    if(gpioInitialise() < 0) {
        cerr << "GPIO initialization is failed" << endl;
        exit(1);
    }
    else {
        cout << "GPIO initialization is OK" << endl;
    }

    /* Set GPIO pins to enable (deblock) signals as output */
    /* Initial values shall be 0 (low) */
    gpioWrite(EN1, GB);
    gpioWrite(EN2, GB);
    gpioWrite(EN3, GB);
    gpioSetMode(EN1, PI_OUTPUT);
    gpioSetMode(EN2, PI_OUTPUT);
    gpioSetMode(EN3, PI_OUTPUT);

    /* Set GPIO pins to input (gate) signals as output */
    /* Initial values shall be 0 (low) */
    gpioWrite(IN1, 0);
    gpioWrite(IN2, 0);
    gpioWrite(IN3, 0);
    gpioSetMode(IN1, PI_OUTPUT);
    gpioSetMode(IN2, PI_OUTPUT);
    gpioSetMode(IN3, PI_OUTPUT);

    /**/
    gpioWrite(EN1, DEB);
    gpioWrite(EN2, DEB);
    gpioWrite(EN3, DEB);

    /* Set GPIO pin for Sector 1 signal */
    gpioWrite(SEC1, 0);
    gpioSetMode(SEC1, PI_OUTPUT);
}

void IHM07M1::produceSignal(uint sector)
{
    /* Produce necessary signals for Sector 1-6 */
    switch(sector) {
        case 1:     /* Sector 1 */
            /* Produce input (gate) signals */
            gpioHardwarePWM(IN1, F_PWM, MOD_F);
            gpioWrite(IN2, 0);
            gpioWrite(IN3, 0);

            /* Produce enable (deblock) signals */
            gpioWrite(EN1, DEB);
            gpioWrite(EN2, GB);
            gpioWrite(EN3, DEB);

            /* Write 1 to Sector 1 signal */
            gpioWrite(SEC1, 1);

            break;

        case 2:     /* Sector 2 */
            /* Produce input (gate) signals */
            gpioWrite(IN1, 0);
            gpioHardwarePWM(IN2, F_PWM, MOD_F);
            gpioWrite(IN3, 0);

            /* Produce enable (deblock) signals */
            gpioWrite(EN1, GB);
            gpioWrite(EN2, DEB);
            gpioWrite(EN3, DEB);

            /* Write 0 to Sector 1 signal */
            gpioWrite(SEC1, 0);

            break;

        case 3:     /* Sector 3 */
            /* Produce input (gate) signals */
            gpioWrite(IN1, 0);
            gpioHardwarePWM(IN2, F_PWM, MOD_F);
            gpioWrite(IN3, 0);

            /* Produce enable (deblock) signals */
            gpioWrite(EN1, DEB);
            gpioWrite(EN2, DEB);
            gpioWrite(EN3, GB);

            /* Write 0 to Sector 1 signal */
            gpioWrite(SEC1, 0);

            break;

        case 4:     /* Sector 4 */
            /* Produce input (gate) signals */
            gpioWrite(IN1, 0);
            gpioWrite(IN2, 0);
            gpioHardwarePWM(IN3, F_PWM, MOD_F);

            /* Produce enable (deblock) signals */
            gpioWrite(EN1, DEB);
            gpioWrite(EN2, GB);
            gpioWrite(EN3, DEB);

            /* Write 0 to Sector 1 signal */
            gpioWrite(SEC1, 0);

            break;

        case 5:     /* Sector 5 */
            /* Produce input (gate) signals */
            gpioWrite(IN1, 0);
            gpioWrite(IN2, 0);
            gpioHardwarePWM(IN3, F_PWM, MOD_F);

            /* Produce enable (deblock) signals */
            gpioWrite(EN1, GB);
            gpioWrite(EN2, DEB);
            gpioWrite(EN3, DEB);

            /* Write 0 to Sector 1 signal */
            gpioWrite(SEC1, 0);

            break;

        case 6:     /* Sector 6 */
            /* Produce input (gate) signals */
            gpioHardwarePWM(IN1, F_PWM, MOD_F);
            gpioWrite(IN2, 0);
            gpioWrite(IN3, 0);

            /* Produce enable (deblock) signals */
            gpioWrite(EN1, DEB);
            gpioWrite(EN2, DEB);
            gpioWrite(EN3, GB);

            /* Write 0 to Sector 1 signal */
            gpioWrite(SEC1, 0);

            break;
    }
}

void IHM07M1::outputUVWVoltage(vector<double> Vuvw)
{
   outputHardwarePWM(IN1, Vuvw[0]);
   outputHardwarePWM(IN2, Vuvw[1]);
   outputHardwarePWM(IN3, Vuvw[2]);
}

void IHM07M1::outputHardwarePWM(int IN_No, double voltage)
{
    double voltage_buf;
    
    if (voltage > Max_voltage)
    {
        voltage_buf = Max_voltage;
        cout << "Max" << endl;
    }
    else if(voltage < Min_voltage)
    {
        voltage_buf = Min_voltage;
        cout << "Min" << endl;
    }
    else
    {
        voltage_buf = voltage;
    }

    double voltage_Inv  = voltage_buf + offset_voltage;

    int MOD_F_value = static_cast<int>(1000000.0 * voltage_Inv/supply_voltage);

    gpioHardwarePWM(IN_No, F_PWM, MOD_F_value);
}

