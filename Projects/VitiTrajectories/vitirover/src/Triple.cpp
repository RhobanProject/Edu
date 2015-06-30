#include "Triple.h"

Triple::Triple(){
    _x = 0;
    _y = 0;
    _z = 0;
}

Triple::Triple(float x, float y, float z)
{
    _x = x;
    _y = y;
    _z = z;
}


void Triple::setX(float x)
{
    _x = x;
}

void Triple::setY(float y)
{
    _y = y;
}

void Triple::setZ(float z)
{
    _z = z;
}


float Triple::getX()
{
    return _x;
}

float Triple::getY()
{
    return _y;
}

float Triple::getZ()
{
    return _z;
}
