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
    file << "No, angle [deg]\n";

}

void data_log::angle_record(int counter, double deg)
{
    file << counter << "," << deg << "\n";
}

void data_log::close()
{
    file.close();
}