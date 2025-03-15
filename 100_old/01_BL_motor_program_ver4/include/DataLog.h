#ifndef DATA_LOG_H
#define DATA_LOG_H

#include <fstream>
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

    void makeCSVfile();
    void LogMatrixData();
    
public:
    DataLog(bool debug);
    void begin();
    void MakeDataMatrix(double time);
    void outputCSVfile();
};

#endif