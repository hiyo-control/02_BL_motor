#ifndef DATA_LOG_H
#define DATA_LOG_H

#include <fstream>
#include <iostream>
#include <vector>

#include "AS5048A.h"
#include "MCP3208.h"

using namespace std;

class DataLog
{
private:
    bool debug;
    string filename;
    ofstream file;
    vector <double> time_data;
    vector <double> angle;
    vector <double> UCurrent;
    vector <double> VCurrent;
    vector <double> WCurrent;
    vector <double> Id;
    vector <double> Iq;

    void makeCSVfile();
    void LogMatrixData();
    double ParkTransform_Id(vector<double> Iuvw);
    double ParkTransform_Iq(vector<double> Iuvw);
    
public:
    DataLog(bool debug);
    void begin();
    void MakeDataMatrix(double time);
    void outputCSVfile();
};

#endif