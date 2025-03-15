#include "IHM07M1.h"

// Define motor and driver parameter
static const int F_PWM = 15000;
//static const int F_PWM = 1500;
static const int MOD_F = 400000;

// Define GIO pins
#define EN1 16
#define EN2 20
#define EN3 21

#define IN1 12
#define IN2 13
#define IN3 18

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

    /* Set GPIO pin for Sector 1 signal */
    gpioWrite(SEC1, 0);
    gpioSetMode(SEC1, PI_OUTPUT);
}

void IHM07M1::forcedCommutate(uint tick_f, uint counter)
{
    uint sector = counter % 6 + 1;
    uint tick = gpioTick();
    
    produceSignal(sector);
    
    while(gpioTick()-tick < tick_f)
    {
    }
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
