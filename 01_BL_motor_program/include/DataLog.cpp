#include "DataLog.h"

using namespace std;

static const uint8_t AS5048A_cs_ch = 24;
static const uint8_t MCP3208_cs_ch = 26;

static const double PI = 3.141592;

AS5048A motorAngle(AS5048A_cs_ch, true);
MCP3208 getCurrent(MCP3208_cs_ch);

DataLog::DataLog(bool debug)
{
    this->debug = debug;
}

void DataLog::begin()
{
    getCurrent.begin();

    //double time_buf = 0.0;
    //MakeDataMatrix(time_buf);
}

void DataLog::makeCSVfile()
{
    this->filename = "data.csv";
    file.open(this->filename);

    if(!file.is_open())
    {
        cerr << "file can not open" << endl;
    }
    char csv_label[] = "time, angle [deg], U Currrent [A], V Current [A], W Current [A] , Id[A], Iq[A]";
    file << csv_label << endl;
}

void DataLog::MakeDataMatrix(double time)
{
    
    this->time_data.push_back(time);

    //double angle_buf = motorAngle.getMechAngleIndeg();
    double angle_buf = motorAngle.getMechCumulativeAngleIndeg();
    this->angle.push_back(angle_buf);

    double UCur_buf = getCurrent.getUCurrent();
    this->UCurrent.push_back(UCur_buf);

    double VCur_buf = getCurrent.getVCurrent();
    this->VCurrent.push_back(VCur_buf);

    double WCur_buf = -(UCur_buf + VCur_buf);
    this->WCurrent.push_back(WCur_buf);

    vector<double> Iuvw = {UCur_buf, VCur_buf, WCur_buf};
    double Id_buf = ParkTransform_Id(Iuvw);
    double Iq_buf = ParkTransform_Iq(Iuvw);
    this->Id.push_back(Id_buf);
    this->Iq.push_back(Iq_buf);
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
        file << this->UCurrent[i] << "," << this->VCurrent[i] << "," << this->WCurrent[i] << ",";
        file << this->Id[i] << "," << this->Iq[i] << endl;
    }
}

double DataLog::ParkTransform_Id(vector<double> Iuvw)
{
    double angle_elec_rad = motorAngle.getElecAngleInrad();

    double Id = sqrt(2.0/3.0)*(  cos(angle_elec_rad)*Iuvw[0]+cos(angle_elec_rad-2*PI/3)*Iuvw[1]+cos(angle_elec_rad+2*PI/3)*Iuvw[2] );

    return Id;
}

double DataLog::ParkTransform_Iq(vector<double> Iuvw)
{
    double angle_elec_rad = motorAngle.getElecAngleInrad();

    double Iq = sqrt(2.0/3.0)*( -sin(angle_elec_rad)*Iuvw[0]-sin(angle_elec_rad-2*PI/3)*Iuvw[1]-sin(angle_elec_rad+2*PI/3)*Iuvw[2] );

    return Iq;
}