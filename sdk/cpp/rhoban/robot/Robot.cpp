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
#include <iostream>
#include <string>
#include <communication/Connection.h>
#include <communication/CommandsStore.h>
#include <communication/Message.h>
#include <motors/Motors.h>
#include <config/Configurations.h>
#include "Robot.h"

using namespace std;

namespace Rhoban
{
  Robot::Robot(CommandsStore *commandsStore)
  {
    configs = new Configurations;
    connection = new Connection(commandsStore);
    motors = new Motors(connection);
  }

  Robot::~Robot()
  {
    delete configs;
    delete connection;
    delete motors;
  }

  void Robot::connect(const char *adress, int port)
  {
    connection->connectTo(adress, port);
  }

  int Robot::isConnected()
  {
    return connection->isConnected();
  }

  int Robot::testConnection()
  {
    Message *response = new Message;
    cout << "Testing server version..." << endl;
    response = connection->ServerGetVersion_response();
    cout << "Version : " << response->read_uint() << endl;
    
    cout << "Testing echo\"Hello world\"..." << endl;
    response = connection->ServerEcho_response("Hello world");
    cout << " Echo : " << response->read_string() << endl;

    if(response->read_string().compare("Hello world") == 0)
      cout << "Connection test successful." << endl;
    else
      cout << "Connection test failed." << endl;

    delete response;
  }

  void Robot::loadLowLevelConfig(string filename)
  {
    configs->loadLowLevelConfig(filename);
  }

  void Robot::loadMoveSchedulerConfig(string filename)
  {
    configs->loadMoveSchedulerConfig(filename);
    motors->setConfig(configs->getMoveSchedulerConfig());
  }

  void Robot::stop()
  {

  }

  void Robot::initialize()
  {

  }

  void Robot::moveMotor(string motorId, int angle)
  {

  }

  void Robot::compliant(string motorId)
  {

  }

  void Robot::allCompliant()
  {

  }

  void Robot::setMotors(Motors *motors)
  {
    this->motors = motors;
  }

  Motors* Robot::getMotors()
  {
    return motors;
  }

  void Robot::setConfigs(Configurations *configs)
  {
    this->configs = configs;
  }

  Configurations *Robot::getConfigs()
  {
    return configs;
  }

  void Robot::setConnection(Connection *connection)
  {
    this->connection = connection;
  }

  Connection *Robot::getConnection()
  {
    return connection;
  }

}
