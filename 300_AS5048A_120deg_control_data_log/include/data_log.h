#ifndef DATA_LOG_H
#define DATA_LOG_H

#include <fstream>
#include <iostream>

class data_log
{
private:
    bool debug;
    std::string filename;
    std::ofstream file;

public:
    data_log(bool debug);
    void begin();
    void angle_record(int counter, int16_t RawRotation, int16_t Rotation, double CompRotation, double ElecCompRotation, double Mech_deg, double Mech_Comp_deg);
    void close();
};

#endif