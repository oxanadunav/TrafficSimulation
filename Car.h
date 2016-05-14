#ifndef CAR_H
#define CAR_H

#include <iostream>
#include <windows.h>
#include <cmath>
#include <mmsystem.h>
#include <time.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

#define ID_TIMER 100

using namespace std;

class Car
{
    COLORREF clr;
    public:
    POINT center;
    int xSpeed,ySpeed;
    bool semafor=true;
    int right;

    Car(POINT center,const int &speed);
    bool Accelerate(const int& dSpeedX,const int& dSpeedY);
    bool Collision(const RECT &rect);
    bool Move(const HDC &hdc,const RECT& rect,HBRUSH &hBrush);
    void SetSemafor(bool sem);
    bool Coll(Car &c2);
};

#endif // CAR_H
