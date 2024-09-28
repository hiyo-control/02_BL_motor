#include "data_log.h"

using namespace std;

data_log::data_log(bool debug)
{
    this->debug = debug;
}

void data_log::begin()
{
    this->filename = "data.csv";
    file.open(this->filename);

    if(!file.is_open())
    {
        cerr << "file can not open" << endl;
    }
    file << "No, RawRotation, Rotation, CompRotation, ElecCompRotation, Mech_deg, Mech_Comp_deg\n";

}

void data_log::angle_record(int counter, int16_t RawRotation, int16_t Rotation, double CompRotation, double ElecCompRotation, double Mech_deg, double Mech_Comp_deg)
{
    file << counter << "," << RawRotation << "," << Rotation << "," << CompRotation << "," << ElecCompRotation << "," << Mech_deg << "," << Mech_Comp_deg << "\n";
}

void data_log::close()
{
    file.close();
}