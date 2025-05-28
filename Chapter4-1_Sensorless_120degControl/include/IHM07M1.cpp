#include "IHM07M1.h"

// Constructor
IHM07M1::IHM07M1(bool debug)
{
    this->debug = debug;
    // define Enable Port
    this->Enable1 = 16; // U-phase
    this->Enable2 = 20; // V-phase
    this->Enable3 = 21; // W-phase

    // define PWM Input Port
    this->PWM_Input1 = 12; // U-phase
    this->PWM_Input2 = 19; // V-phase
    this->PWM_Input3 = 18; // V-phase

    // define ON/OFF parameter
    this->GB  = 0;
    this->DEB = 1;
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
    gpioWrite(this->Enable1, this->GB);
    gpioWrite(this->Enable2, this->GB);
    gpioWrite(this->Enable3, this->GB);
    gpioSetMode(this->Enable1, PI_OUTPUT);
    gpioSetMode(this->Enable2, PI_OUTPUT);
    gpioSetMode(this->Enable3, PI_OUTPUT);

    /* Set GPIO pins to input (gate) signals as output */
    /* Initial values shall be 0 (low) */
    gpioWrite(this->PWM_Input1, this->GB);
    gpioWrite(this->PWM_Input2, this->GB);
    gpioWrite(this->PWM_Input3, this->GB);
    gpioSetMode(this->PWM_Input1, PI_OUTPUT);
    gpioSetMode(this->PWM_Input2, PI_OUTPUT);
    gpioSetMode(this->PWM_Input3, PI_OUTPUT);

}

void IHM07M1::OutputPWM(uint counter)
{
    uint sector = counter % 6 + 1;

    //cout << "Sector is " << sector << endl;
    produceSignal(sector);
}

void IHM07M1::produceSignal(uint sector)
{
    /* Produce necessary signals for Sector 1-6 */
    switch(sector) {
        case 1:
            /* Produce input (gate) signals */
            gpioHardwarePWM(this->PWM_Input1, F_PWM, MOD_F);
            gpioWrite(this->PWM_Input2, 0);
            gpioWrite(this->PWM_Input3, 0);

            /* Produce enable (deblock) signals */
            gpioWrite(this->Enable1, this->DEB);
            gpioWrite(this->Enable2, this->GB);
            gpioWrite(this->Enable3, this->DEB);

            break;

        case 2:
            /* Produce input (gate) signals */
            gpioHardwarePWM(this->PWM_Input1, F_PWM, MOD_F);
            gpioWrite(this->PWM_Input2, 0);
            gpioWrite(this->PWM_Input3, 0);

            /* Produce enable (deblock) signals */
            gpioWrite(this->Enable1, this->DEB);
            gpioWrite(this->Enable2, this->DEB);
            gpioWrite(this->Enable3, this->GB);

            break;

        case 3:
            /* Produce input (gate) signals */
            gpioWrite(this->PWM_Input1, 0);
            gpioWrite(this->PWM_Input2, 0);
            gpioHardwarePWM(this->PWM_Input3, F_PWM, MOD_F);

            /* Produce enable (deblock) signals */
            gpioWrite(this->Enable1, this->GB);
            gpioWrite(this->Enable2, this->DEB);
            gpioWrite(this->Enable3, this->DEB);

            break;

        case 4:
            /* Produce input (gate) signals */
            gpioWrite(this->PWM_Input1, 0);
            gpioWrite(this->PWM_Input2, 0);
            gpioHardwarePWM(this->PWM_Input3, F_PWM, MOD_F);

            /* Produce enable (deblock) signals */
            gpioWrite(this->Enable1, this->DEB);
            gpioWrite(this->Enable2, this->GB);
            gpioWrite(this->Enable3, this->DEB);

            break;

        case 5:
            /* Produce input (gate) signals */
            gpioWrite(this->PWM_Input1, 0);
            gpioHardwarePWM(this->PWM_Input2, F_PWM, MOD_F);
            gpioWrite(this->PWM_Input3, 0);

            /* Produce enable (deblock) signals */
            gpioWrite(this->Enable1, this->DEB);
            gpioWrite(this->Enable2, this->DEB);
            gpioWrite(this->Enable3, this->GB);

            break;

        case 6:
            /* Produce input (gate) signals */
            gpioWrite(this->PWM_Input1, 0);
            gpioHardwarePWM(this->PWM_Input2, F_PWM, MOD_F);
            gpioWrite(this->PWM_Input3, 0);

            /* Produce enable (deblock) signals */
            gpioWrite(this->Enable1, this->GB);
            gpioWrite(this->Enable2, this->DEB);
            gpioWrite(this->Enable3, this->DEB);

            break;
    }
}
