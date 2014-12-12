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
    Motor(int id, string name, double iniAngle, double zeroAngle);

    bool isDirty();

    void setAngle(double angle);
    void setRelAngle(double angle);
    void setLoad(double load);
    void setSpeed(double speed);

    double getAngle();
    double getRelAngle();
    double getLoad();
    double getSpeed();

    void setId(int id);
    int getId();
    void setName(string name);
    string getName();
    void setLastUpdate(time_t time);
    time_t getLastUpdate();
    void setIniAngle(double angle);
    double getIniAngle();
    void setZeroAngle(double angle);
    double getZeroAngle();
    void setGoalAngle(double angle);
    double getGoalAngle();
    void setCurrentGoal(double goal);
    double getCurrentGoal();
    double getCurrentRelGoal();
    void setCurrentAngle(double angle);
    double getCurrentAngle();
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
    bool getGoalSpeedInit();
    bool getCurrentSpeedInit();
    bool getGoalLoadInit();
    bool getCurrentLoadInit();    

  protected:
    time_t lastUpdate;
    int id;
    string name;
    double iniAngle;
    double zeroAngle;

    double goalAngle;
    double currentAngle;
    double currentGoal;

    double goalSpeed;
    double currentSpeed;

    double goalLoad;
    double currentLoad;

    bool dirty;

    bool goalAngleInit;
    bool currentAngleInit;
    bool goalSpeedInit;
    bool currentSpeedInit;
    bool goalLoadInit;
    bool currentLoadInit;
  };
}

#endif // MOTOR_H_
