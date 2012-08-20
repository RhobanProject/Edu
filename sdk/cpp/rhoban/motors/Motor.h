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
#include <ctime>
#include <string>
#include <utils/types.h>

using namespace std;

namespace Rhoban
{
  class Motor
  {
  public:
    Motor(int id, string name);

    bool isDirty();

    void setAngle(int angle);
    void setLoad(double load);
    void setSpeed(double speed);

    int getAngle();
    double getLoad();
    double getSpeed();

    void setId(int id);
    int getId();
    void setName(string name);
    string getName();
    void setLastUpdate(time_t time);
    time_t getLastUpdate();
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
    bool getGoalAngleInit();
    bool getCurrentAngleInit();
    bool getCurrentSpeedInit();

  protected:
    int id;
    string name;
    time_t lastUpdate;

    int goalAngle;
    int currentAngle;

    double goalSpeed;
    double currentSpeed;

    double goalLoad;
    double currentLoad;

    bool dirty;

    bool goalAngleInit;
    bool currentAngleInit;
    bool currentSpeedInit;
  };
}

#endif // MOTOR_H_
