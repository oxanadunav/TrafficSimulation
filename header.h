#include <windows.h>
#define ID_TIMER1 1

using namespace std;

class traffic_lights
{
public:
    POINT position;
    bool tls;

    void setTL(POINT position, bool tls);
    bool SwitchTL(const HDC &hdc,HBRUSH &hBrush);
};
