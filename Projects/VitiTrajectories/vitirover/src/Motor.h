#ifndef MOTOR_H
#define MOTOR_H

#include "Point.h"

class Motor
{
 public:
  Point();
  Point( float power, Point<double> position );
  Point( float power );

  void setPower( float power );
  void setPosition( Point position );

  float getPower();
  float getPosition();


 private:
  float _power;
  Point _position;
  
};

#endif //MOTOR_H
