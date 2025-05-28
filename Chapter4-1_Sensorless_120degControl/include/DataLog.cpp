#include "DataLog.h"

static const uint8_t AS5048A_cs_ch = 24;

AS5048A motorAngle(AS5048A_cs_ch, true);

DataLog::DataLog(bool debug)
{
    this->debug = debug;
}

void DataLog::begin()
{
    motorAngle.begin();
}

void DataLog::makeCSVfile()
{
    this->filename = "data.csv";
    file.open(this->filename);

    if(!file.is_open())
    {
        cerr << "file can not open" << endl;
    }
    char csv_label[] = "Counter [-], Mech Angle value [-]";
    file << csv_label << endl;
}

void DataLog::MakeDataMatrix(int counter)
{
    
    this->counter_data.push_back(counter);

    uint16_t MechAngleValue_buf = motorAngle.getRawRotation();
    this->MechAngleValue_data.push_back(MechAngleValue_buf);
}

void DataLog::outputCSVfile()
{
    makeCSVfile();
    LogMatrixData();
    file.close();
}

void DataLog::LogMatrixData()
{
    int data_num = this->counter_data.size();
    for(int i=0; i<data_num; i++)
    {
        file << this->counter_data[i] << "," << this->MechAngleValue_data[i] << endl;
    }
}

