#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include <wiringPiSPI.h>
#include <wiringPi.h>

#include "./include/AS5048A.h"

AS5048A angleSensor(24, true);

int main(void)
{
    std::cout << "Hello world" << std::endl;
    return 0;
}