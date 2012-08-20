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

  }

  Motors::~Motors()
  {

  }

  void Motors::start(int frequency)
  {

  }

  void Motors::execute()
  {

  }

  void Motors::stop()
  {

  }

  void Motors::allCompliant()
  {

  }

  void Motors::setConnection(Connection *connection)
  {

  }

  Connection * Motors::getConnection()
  {
    return NULL;
  }

  void Motors::addMotor(string name, Motor *motor)
  {

  }
 
  Motor * Motors::getMotor(string name)
  {
    return NULL;
  }

  void Motors::removeMotor(string name)
  {

  }

  void Motors::setConfig(MoveSchedulerConfig *config)
  {

  }
  
  MoveSchedulerConfig *Motors::getConfig()
  {
    return NULL;
  }
}
