/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
#include <cstdlib>
#include <cstdio>
#include "Motor.h"

using namespace std;

namespace Rhoban
{
  Motor::Motor(int id, string name)
  {
    this->id = id;
    this->name = name;
    lastUpdate = 0;
  }

  bool Motor::isDirty()
  {
    return getDirty();
  }
  
  void Motor::setAngle(int angle)
  {
    setGoalAngle(angle);
    setDirty(1);
  }

  void Motor::setLoad(double load)
  {
    setGoalLoad(load);
    setDirty(1);
  }

  void Motor::setSpeed(double speed)
  {
    setGoalSpeed(speed);
    setDirty(1);
  }

  int Motor::getAngle()
  {
    return getCurrentAngle();
  }

  double Motor::getSpeed()
  {
    return getCurrentSpeed();
  }

  double Motor::getLoad()
  {
    return getCurrentLoad();
  }

  void Motor::setId(int id)
  {
    this->id=id;
  }

  int Motor::getId()
  {
    return id;
  }

  void Motor::setName(string name)
  {
    this->name=name;
  }

  string Motor::getName()
  {
    return name;
  }

  void Motor::setLastUpdate(time_t time)
  {
    lastUpdate=time;
  }

  time_t Motor::getLastUpdate()
  {
    return lastUpdate;
  }
  
  void Motor::setGoalAngle(int angle)
  {
    goalAngle=angle;
  }

  int Motor::getGoalAngle()
  {
    return goalAngle;
  }

  void Motor::setCurrentAngle(int angle)
  {
    currentAngle=angle;
  }

  int Motor::getCurrentAngle()
  {
    return currentAngle;
  }

  void Motor::setGoalSpeed(double speed)
  {
    goalSpeed=speed;
  }

  double Motor::getGoalSpeed()
  {
    return goalSpeed;
  }

  void Motor::setCurrentSpeed(double speed)
  {
    currentSpeed=speed;
  }

  double Motor::getCurrentSpeed()
  {
    return currentSpeed;
  }

  void Motor::setGoalLoad(double load)
  {
    goalLoad=load;
  }

  double Motor::getGoalLoad()
  {
    return goalLoad;
  }

  void Motor::setCurrentLoad(double load)
  {
    currentLoad=load;
  }

  double Motor::getCurrentLoad()
  {
    return currentLoad;
  }

  void Motor::setDirty(bool val)
  {
    dirty=val;
  }

  bool Motor::getDirty()
  {
    return dirty;
  } 
}
