/*
 * bldc6p.c
 * A test of driving a blushless DC motor with 6-pulse contol by Raspberry Pi
 * Depends on pigpio.h
 * (c) 2021 @RR_Inyo
 * Released under the MIT lisence
 * https://opensource.org/licenses/mit-license.php
 * exe command : sudo ./build/blcd6p
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pigpio.h>

/* Define motor and drive parameter */
//#define F_PWM   15000   /* [Hz], PWM carrier frequency */
#define F_PWM   30000
#define MOD_F   400000  /* Modulation index for initial forced commutation: Note unity = 1,000,000 */

/* Define GPIO pins connected to P-NUCLEO-IHM001 */
/* Enable (deblock) signals */
#define EN1     16
#define EN2     20
#define EN3     21

/* Input (gate) signals */
/* Assign GPIO pins capable of hardware PWM */
/* PWM0: GPIO12, 18 for U- and W-phases V */
/* PWM1: GPIO13 for V-phase */
#define IN1     12
#define IN2     13
#define IN3     18

/* Sector 1 signal */
#define SEC1    25

/* Constant aliases */
#define GB      0
#define DEB     1

/* Define function protorypes */
void setGPIO();
void forcedCommutate();
void produceSignal(unsigned sector);

/* The main function */
int main(int argc, char *argv[])
{
    /* Set GPIO pins to communicate with P-NUCLEO-IHM001 */
    printf("Setting GPIO pins...\n");
    setGPIO();

    /* Forced commutation to start up */
    printf("Trying forced commutation...\n");

    /* Infinate loop, outer */
    while(1) 
    {
        forcedCommutate();
    }
}

/* Function to set GPIO pins to communicate with P-NUCLEO-IHM001 */
void setGPIO()
{
    /* Initialize GPIO */
    if(gpioInitialise() < 0) {
        printf("GPIO initialization failed.\n");
        exit(1);
    }
    else {
        printf("GPIO initialization OK.\n");
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

/* Function to try to forced-commutatuon to start motor */
void forcedCommutate()
{
    for (int sector = 0; sector < 6; sector++) {
        printf("sector>> %d\n", sector);
        
        produceSignal(sector + 1);
        
        //getchar();
        //usleep(10000);
        usleep(5000);
    }
}

/* Function to produce GPIO signals depending on selected sector */
void produceSignal(unsigned sector)
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
