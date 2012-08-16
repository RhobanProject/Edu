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

using namespace std;

namespace Rhoban
{
  class Motor
  {
  public:
    Motor(int defaultAngle);
    ~Motor();

    void setDefaultAngle(int angle);
    int getDefaultAngle();
    void setGoalAngle(int angle);
    int getGoalAngle();

  protected:
    int defaultAngle;
    int goalAngle;x
  };
}

#endif // MOTOR_H_
