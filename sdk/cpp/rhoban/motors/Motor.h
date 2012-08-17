/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
#ifndef MOTOR_H_
#define MOTOR_H_

#include <cstdlib>
#include <cstdio>
#include <utils/types.h>

using namespace std;

namespace Rhoban
{
  class Motor
  {
  public:
    Motor();
    ~Motor();
    bool isDirty();
    void setAngle(int angle);
    void setLoad(double load);
    void setSpeed(double speed);
    int getAngle();
    double getLoad();
    double getSpeed();

    void setId(int id);
    int getId();
    void setName(byte name);
    byte getName();
    void setGoalAngle(int angle);
    int getGoalAngle();
    void setCurrentAngle(int angle);
    int getCurrentAngle();
    void setGoalSpeed(double speed);
    double getGoalSpeed();
    void setCurrentSpeed(double speed);
    double getCurrentSpeed();
    void setGoalLoad(double load);
    double getGoalLoad();
    void setCurrentLoad(double load);
    double getCurrentLoad();
    void setDirty(bool val);
    bool getDirty();

  protected:
    int id;
    byte name;

    int goalAngle;
    int currentAngle;

    double goalSpeed;
    double currentSpeed;

    double goalLoad;
    double currentLoad;

    bool dirty;
  };
}

#endif // MOTOR_H_
