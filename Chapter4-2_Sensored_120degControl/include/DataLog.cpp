#include "DataLog.h"

static const uint8_t AS5048A_cs_ch = 24;
static const uint8_t MCP3208_cs_ch = 26;
static const double Vref = 5.35;
static const int filter_cycle_microsec = 500; // microsecond

AS5048A motorAngle(AS5048A_cs_ch, true);
MCP3208 motorCurrent(MCP3208_cs_ch, Vref, filter_cycle_microsec);

DataLog::DataLog(bool debug)
{
    this->debug = debug;
}

void DataLog::begin()
{
    motorCurrent.begin();
}

void DataLog::makeCSVfile()
{
    this->filename = "data.csv";
    file.open(this->filename);

    if(!file.is_open())
    {
        cerr << "file can not open" << endl;
    }
    char csv_label[] = "time [sec], Angle [deg], Uph-Current [A], Vph-Current [A], Wph-Current [A]";
    file << csv_label << endl;
}

void DataLog::MakeDataMatrix(double time)
{
    this->time_data.push_back(time);

    double MechAngle_buf = motorAngle.getMechCumulativeAngleIndeg();
    this->MechAngle_data.push_back(MechAngle_buf);

    double UCur_buf = motorCurrent.getLPF_UCurrent();
    this->UCurrent.push_back(UCur_buf);

    double VCur_buf = motorCurrent.getLPF_VCurrent();
    this->VCurrent.push_back(VCur_buf);
    
    double WCur_buf = -( UCur_buf + VCur_buf );
    this->WCurrent.push_back(WCur_buf);
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
        file << this->time_data[i] << "," << this->MechAngle_data[i] << ",";
        file << this->UCurrent[i] << "," << this->VCurrent[i] << "," << this->WCurrent[i] << endl;
    }
}

void DataLog::calcLPF()
{
    motorCurrent.calcLPF_UVoltage();
    motorCurrent.calcLPF_VVoltage();
    motorCurrent.calcLPF_WVoltage();

}
