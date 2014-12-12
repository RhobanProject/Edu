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
  Motor::Motor(int id, string name, double iniAngle, double zeroAngle)
  {
    this->id = id;
    this->name = name;
    this->iniAngle = iniAngle;
    this->zeroAngle = zeroAngle;
    lastUpdate = 0;
    dirty = 0;

    goalAngleInit = 0;
    currentAngleInit = 0;
    goalSpeedInit = 0;
    currentSpeedInit = 0;
    goalLoadInit = 0;
    currentLoadInit = 0;
    currentGoal = 0;
  }

  bool Motor::isDirty()
  {
    return getDirty();
  }
  
  void Motor::setAngle(double angle)
  {
    setGoalAngle(angle);
    setDirty(1);
    goalAngleInit = 1;
  }

  void Motor::setCurrentGoal(double goal)
  {
      currentGoal = goal;
  }

  double Motor::getCurrentGoal()
  {
      return currentGoal;
  }

  double Motor::getCurrentRelGoal()
  {
      return (currentGoal - iniAngle - zeroAngle);
  }

  void Motor::setRelAngle(double angle)
  {
    setDirty(1);
    setGoalAngle(iniAngle + zeroAngle + angle); 
    goalAngleInit = 1;
  }

  void Motor::setLoad(double load)
  {
    setGoalLoad(load);
    setDirty(1);
    goalLoadInit = 1;
  }

  void Motor::setSpeed(double speed)
  {
    setGoalSpeed(speed);
    setDirty(1);
    goalSpeedInit = 1;
  }
  
  double Motor::getAngle()
  {
    return getCurrentAngle();
  }

  double Motor::getRelAngle()
  {
    return (currentAngle - iniAngle - zeroAngle);
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
    this->id = id;
  }

  int Motor::getId()
  {
    return id;
  }

  void Motor::setName(string name)
  {
    this->name = name;
  }

  string Motor::getName()
  {
    return name;
  }

  void Motor::setLastUpdate(time_t time)
  {
    lastUpdate = time;
  }

  time_t Motor::getLastUpdate()
  {
    return lastUpdate;
  }

  void Motor::setIniAngle(double angle)
  {
    iniAngle = angle;
  }

  double Motor::getIniAngle()
  {
    return iniAngle;
  }

  void Motor::setZeroAngle(double angle)
  {
    zeroAngle = angle;
  }

  double Motor::getZeroAngle()
  {
    return zeroAngle;
  }
  
  void Motor::setGoalAngle(double angle)
  {
    goalAngle = angle;
    goalAngleInit = 1;
  }

  double Motor::getGoalAngle()
  {
    return goalAngle;
  }

  void Motor::setCurrentAngle(double angle)
  {
    currentAngle = angle;
    currentAngleInit = 1;
  }

  double Motor::getCurrentAngle()
  {
    return currentAngle;
  }

  void Motor::setGoalSpeed(double speed)
  {
    goalSpeed = speed;
    goalSpeedInit = 1;
  }

  double Motor::getGoalSpeed()
  {
    return goalSpeed;
  }

  void Motor::setCurrentSpeed(double speed)
  {
    currentSpeed = speed;
    currentSpeedInit = 1;
  }

  double Motor::getCurrentSpeed()
  {
    return currentSpeed;
  }

  void Motor::setGoalLoad(double load)
  {
    goalLoad = load;
    goalLoadInit = 1;
  }

  double Motor::getGoalLoad()
  {
    return goalLoad;
  }

  void Motor::setCurrentLoad(double load)
  {
    currentLoad = load;
    currentLoadInit = 1 ;
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

  bool Motor::getGoalAngleInit()
  {
    return goalAngleInit;
  }

  bool Motor::getCurrentAngleInit()
  {
    return currentAngleInit;
  }

  bool Motor::getGoalSpeedInit()
  {
    return goalSpeedInit;
  }

  bool Motor::getCurrentSpeedInit()
  {
    return currentSpeedInit;
  }  

  bool Motor::getGoalLoadInit()
  {
    return goalLoadInit;
  }

  bool Motor::getCurrentLoadInit()
  {
    return currentLoadInit;
  }
}
