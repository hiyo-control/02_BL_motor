#include "./include/MCP3208.h"

using namespace std;

uint8_t cs_ch = 26;
double Vref = 3.28;
int mcp3208_ch = 0; // 測定するMCP3208のチャンネル

MCP3208 currentSensor(cs_ch, Vref);

int main(void)
{
    currentSensor.begin();
    cout << "Hello world" << endl;

    while(1)
    {
        int adcValue = currentSensor.readADC(mcp3208_ch);
        double voltage = currentSensor.getVoltage(mcp3208_ch);

        std::cout << "ADC Value: " << adcValue << " | Voltage: " << voltage << " V" << std::endl;
        sleep(1); // 1秒待機

    }
    return 0;
}


