#include "Field.h"

Field::Field()
{
    _viscosity = 0.0;
    _obstacle = 0;
    _grass_high = 0.0;

}

Field::Field(Point<float>p, float viscosity, bool obstacle, float grass_high)
{
    _position = p;
    _viscosity = viscosity;
    _obstacle = obstacle;
    _grass_high = grass_high;

}

Point<float> Field::getPosition()
{
    return _position;
}

void Field::setPosition(Point<float> position)
{
    _position = position;
}

float Field::getViscosity()
{
    return _viscosity;
}

void Field::setViscosity(float viscosity)
{
    _viscosity = viscosity;
}

bool Field::isObstacle()
{
    return _obstacle;
}

void Field::setIsObstacle(bool obstacle)
{
    _obstacle = obstacle;
}

bool Field::isStake()
{
    return _stake;
}

void Field::setIsStake(bool stake)
{
    _stake = stake;
    if(_stake || _vine)
        _obstacle = true;
}

bool Field::isVine()
{
    return _vine;
}

void Field::setIsVine(bool vine)
{
    _vine = vine;
    if(_stake || _vine)
        _obstacle = true;
}

float Field::getGrassHigh()
{
    return _grass_high;
}

void Field::setGrassHigh(float grass_high)
{
    _grass_high = grass_high;
}

void Field::growGrass(float high)
{
    _grass_high += high;
}
