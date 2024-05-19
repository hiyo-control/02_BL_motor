
#include "./include/AS5048A.h"

using namespace std;

AS5048A angleSensor(24, true);

int main(void)
{
    angleSensor.begin();
    cout << "Hello world" << endl;
    

    while(1)
    {
        uint16_t val = angleSensor.getRawRotation();
        cout << "val(bin)=" << bitset<16>(val) << endl;
        cout << "val(int)=" << static_cast<int>(val) << endl;

        sleep(1);
    };

    return 0;
}