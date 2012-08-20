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
#include <config/MoveSchedulerConfig.h>
#include <robot/Robot.h>
#include "Motor.h"

using namespace std;

namespace Rhoban
{
  class Motors : public Thread
  {
  public:
    Motors(Connection *connection);
    ~Motors();
    void start(int frequency);
    void execute();
    void stop();
    void allCompliant();

    void setConnection(Connection *connection);
    Connection * getConnection();
    void setConfig(MoveSchedulerConfig *config);
    MoveSchedulerConfig *getConfig();
    void addMotor(string name, Motor *motor);
    Motor *getMotor(string name);
    void removeMotor(string name);


  protected:
    Connection *connection;
    MoveSchedulerConfig *configuration;
    map<string, Motor *> motors;

    bool running;
  };
}

#endif // MOTORS_H_
