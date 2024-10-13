#include "DataLog.h"

using namespace std;

static const uint8_t cs_ch = 24;
AS5048A motorAngle(cs_ch, true);

DataLog::DataLog(bool debug)
{
    this->debug = debug;
}

void DataLog::begin()
{
    this->time_data.push_back(0);

    //double angle_buf = motorAngle.getMechAngleIndeg();
    double angle_buf = motorAngle.getMechCumulativeAngleIndeg();
    this->angle.push_back(angle_buf);
}

void DataLog::makeCSVfile()
{
    this->filename = "data.csv";
    file.open(this->filename);

    if(!file.is_open())
    {
        cerr << "file can not open" << endl;
    }
    char csv_label[] = "time, angle [deg]";
    file << csv_label << endl;
}

void DataLog::MakeDataMatrix(double time)
{
   this->time_data.push_back(time);

    double angle_buf = motorAngle.getMechCumulativeAngleIndeg();
    this->angle.push_back(angle_buf);
}

void DataLog::outputCSVfile()
{
    makeCSVfile();
    LogMatrixData();
    file.close();
}

void DataLog::LogMatrixData()
{
    int data_num = this->time_data.size();
    for(int i=0; i<data_num; i++)
    {
        file << this->time_data[i] << "," << this->angle[i] << "\n";
    }
}

