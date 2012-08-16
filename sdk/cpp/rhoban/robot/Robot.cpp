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
#include "Robot.h"
#include <communication/Connection.h>
#include <rhoban/motors/Motors.h>
#include <rhoban/config/Configurations.h>

using namespace std;

namespace Rhoban
{
  Robot::Robot()
  {

  }

  Robot::~Robot()
  {

  }

  void Robot::connect(string adress)
  {

  }

  void Robot::setMotors(Motors *motors)
  {

  }

  Motors* Robot::getMotors()
  {
    return new Motors(NULL);
  }

  void Robot::setConfigs(Configurations *configs)
  {

  }

  Configurations *Robot::getConfigs()
  {
    return new Configurations;
  }

  void Robot::setConnection(Connection *connection)
  {

  }

  Connection *Robot::getConnection()
  {
    return NULL;
  }

}
