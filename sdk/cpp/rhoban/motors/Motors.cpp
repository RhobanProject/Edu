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
#include <map>
#include <string>
#include <threading/Thread.h>
#include <config/MoveSchedulerConfig.h>
#include <communication/Connection.h>
#include <robot/Robot.h>
#include "Motor.h"
#include "Motors.h"

using namespace std;

namespace Rhoban
{
  Motors::Motors(Connection *connection) : Thread()
  {
    this->connection = connection;
    running = 0;
  }

  Motors::~Motors()
  {
    delete connection;
    delete configuration;
  }

  size_t Motors::len()
  {
    return motors.size();
  }

  Motor *Motors::operator[](string name)
  {
    return motors[name];
  }

  void Motors::start(int frequency)
  {
    // todo
  }

  void Motors::stop()
  {
    running = 0;
  }

  void Motors::compliant(string name)
  {
    motors[name]->setGoalLoad(0);
  }

  void Motors::allCompliant()
  {
    connection->ServosAllCompliant();
    
    map<string, Motor *>::iterator it;
    for(it = motors.begin(); it != motors.end(); ++it)
      it->second->setGoalLoad(0);
  }

  void Motors::hard(string name)
  {
    motors[name]->setGoalLoad(1023);
  }

  void Motors::allHard(string name)
  {
    map<string, Motor *>::iterator it;
    for(it = motors.begin(); it != motors.end(); ++it)
      it->second->setGoalLoad(1023);    
  }
  
  void Motors::run()
  {
    //todo
  }

  void Motors::execute()
  {
    // ??
  }

  void Motors::setConnection(Connection *connection)
  {
    this->connection = connection;
  }

  Connection * Motors::getConnection()
  {
    return connection;
  }

  void Motors::setConfig(MoveSchedulerConfig *config)
  {
    this->configuration = config;
  }
  
  MoveSchedulerConfig *Motors::getConfig()
  {
    return configuration;
  }

  void Motors::setMotors(map<string, Motor *> motors)
  {
    this->motors = motors;
  }

  map<string, Motor *> Motors::getMotors()
  {
    return motors;
  }

  void Motors::setIdMotors(map<int, Motor *> idMotors)
  {
    this->idMotors = idMotors;
  }

  map<int, Motor *> Motors::getIdMotors()
  {
    return idMotors;
  }
  
  void Motors::setRunning(bool val)
  {
    this->running = val;
  }

  bool Motors::getRunning()
  {
    return running;
  }

  void Motors::setFrequency(int frequency)
  {
    this->frequency = frequency;
  }

  int Motors::getFrequency()
  {
    return frequency;
  }
}
