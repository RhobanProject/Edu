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
    
    size_t len();
    Motor *operator[](string name);
    void start(int frequency);
    void stop();
    void compliant(string name);
    void allCompliant();
    void hard(string name);
    void allHard(string name);
    void run();

    void execute();

    void setConnection(Connection *connection);
    Connection * getConnection();
    void setConfig(MoveSchedulerConfig *config);
    MoveSchedulerConfig *getConfig();
    void setMotors(map<string, Motor *> motors);
    map<string, Motor *> getMotors();
    void setIdMotors(map<int, Motor *> idMotors);
    map<int, Motor *> getIdMotors();
    void setRunning(bool val);
    bool getRunning();
    void setFrequency(int frequency);
    int getFrequency();

  protected:
    Connection *connection;
    MoveSchedulerConfig *configuration;
    map<string, Motor *> motors;
    map<int, Motor *> idMotors;
    bool running;
    int frequency;
  };
}

#endif // MOTORS_H_
