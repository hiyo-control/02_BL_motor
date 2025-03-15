#include "DataLog.h"

using namespace std;

static const uint8_t AS5048A_cs_ch = 24;
static const uint8_t MCP3208_cs_ch = 26;
static const double MCP3208_Vref = 3.28;

AS5048A motorAngle(AS5048A_cs_ch, true);
MCP3208 getCurrent(MCP3208_cs_ch, MCP3208_Vref);

DataLog::DataLog(bool debug)
{
    this->debug = debug;
}

void DataLog::begin()
{
    getCurrent.begin();

    double time_buf = 0.0;
    MakeDataMatrix(time_buf);
}

void DataLog::makeCSVfile()
{
    this->filename = "data.csv";
    file.open(this->filename);

    if(!file.is_open())
    {
        //cerr << "file can not open" << endl;
    }
    char csv_label[] = "time, angle [deg], U Currrent [A], V Current [A], W Current [A]";
    file << csv_label << endl;
}

void DataLog::MakeDataMatrix(double time)
{
    this->time_data.push_back(time);

    //double angle_buf = motorAngle.getMechAngleIndeg();
    double angle_buf = motorAngle.getMechCumulativeAngleIndeg();
    this->angle.push_back(angle_buf);

    double UCurrent_buf = getCurrent.getUCurrent();
    this->UCurrent.push_back(UCurrent_buf);

    double VCurrent_buf = getCurrent.getVCurrent();
    this->VCurrent.push_back(VCurrent_buf);

    double WCurrent_buf = getCurrent.getWCurrent();
    this->WCurrent.push_back(WCurrent_buf);
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
        file << this->time_data[i] << "," << this->angle[i] << ",";
        file << this->UCurrent[i] << "," << this->VCurrent[i] << "," << this->WCurrent[i] << "\n";
    }
}

