/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
#ifndef MOTORS_H_
#define MOTORS_H_

#include <cstdlib>
#include <cstdio>
#include <map>
#include <string>
#include <threading/Thread.h>
#include <communication/Connection.h>
#include <rhoban/robot/Robot.h>
#include "Motor.h"

using namespace std;

namespace Rhoban
{
  class Motors : public Thread
  {
  public:
    Motors(Connection *connection);
    ~Motors();
    void startDispatcher(int frequency);
    void setConnection(Connection *connection);
    void execute();
    Connection * getConnection();
    void addMotor(string name, Motor *motor);
    Motor * getMotor(string name);
    void removeMotor(string name);
  protected:
    map<string, Motor *> motorlist;
    Connection *connection;
  };
}

#endif // MOTORS_H_
