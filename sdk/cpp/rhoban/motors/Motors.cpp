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
#include <communication/Connection.h>
#include <rhoban/robot/Robot.h>
#include "Motor.h"
#include "Motors.h"

using namespace std;

namespace Rhoban
{
  Motors::Motors(Connection *connection)
  {

  }

  Motors::~Motors()
  {

  }

  void Motors::startDispatcher(int frequency)
  {

  }

  void Motors::execute()
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
    return new Motor(0);
  }

  void Motors::removeMotor(string name)
  {

  }

}
