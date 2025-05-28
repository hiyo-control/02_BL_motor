#ifndef DATA_LOG_H
#define DATA_LOG_H

#include <fstream>
#include <iostream>
#include <vector>
#include "AS5048A.h"

using namespace std;

class DataLog
{
private:
    bool debug;
    string filename;
    ofstream file;
    vector <double> counter_data;
    vector <uint16_t> MechAngleValue_data;

    void makeCSVfile();
    void LogMatrixData();

public:
    DataLog(bool debug);
    void begin();
    void MakeDataMatrix(int counter);
    void outputCSVfile();
};

#endif