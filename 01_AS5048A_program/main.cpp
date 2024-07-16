
#include "./include/AS5048A.h"

using namespace std;

AS5048A angleSensor(24, true);

int main(void)
{
    angleSensor.begin();
    cout << "Hello world" << endl;
    
    while(1)
    {
        double deg = angleSensor.getRotationInDegrees();
        cout << "degree=" << deg << endl;

        sleep(1);
    };

    return 0;
}