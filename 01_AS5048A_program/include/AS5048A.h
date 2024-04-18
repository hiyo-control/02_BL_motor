#ifndef AS5048A_H
#define AS5048A_H

// #include <SPI.h>

typedef struct AS5048A
{
    uint8_t _cs;
    bool errorFlag;
    bool ocfFlag;
    uint16_t position;
    bool debug;
    uint8_t esp32_delay;

} AS5048A;
#endif