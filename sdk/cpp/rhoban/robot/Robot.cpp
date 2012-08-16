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
#include <vector>
#include <communication/Connection.h>
#include <communication/CommandsStore.h>
#include <communication/Message.h>
#include <communication/types.h>
#include <motors/Motors.h>
#include <config/Configurations.h>
#include "Robot.h"

using namespace std;

namespace Rhoban
{
  Robot::Robot(CommandsStore *commandsStore)
  {
    connection = new Connection(commandsStore);
    motors = new Motors(connection);
    configs = new Configurations(connection);
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
    connection->stop();
    motors->stop();
  }

  void Robot::moveMotor(byte motorId, int angle)
  {
    Message *response = new Message;
    response = connection->ServosSetValues_response(1, vector<byte> (1, motorId), 
						    vector<int> (1, angle), 
						    vector<ui32> (1, 1023), 
						    vector<ui32> (1, 1023));

    cout << "moveMotor(" << motorId << ", " << angle << ") : "
	 << response->read_string() << endl;

    delete response;
  }

  void Robot::compliant(byte motorId)
  {
    Message *response = new Message;
    response = connection->ServosSetValues_response(1, vector<byte> (1, motorId), 
						    vector<int> (1, 0), 
						    vector<ui32> (1, 0), 
						    vector<ui32> (1, 0));

    cout << "compliant(" << motorId << ") : " << response->read_string() << endl;

    delete response;
  }

  void Robot::allCompliant()
  {
    motors->allCompliant();
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
