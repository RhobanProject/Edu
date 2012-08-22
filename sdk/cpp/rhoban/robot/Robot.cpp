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
#include <utils/types.h>
#include <motors/Motors.h>
#include <config/Configurations.h>
#include "Robot.h"
#include "Moves.h"

using namespace std;

namespace Rhoban
{
  Robot::Robot(CommandsStore *commandsStore)
  {
    connection = new Connection(commandsStore);
    motors = new Motors(connection);
    configs = new Configurations(connection);
    moves = new Moves(connection);
  }

  Robot::~Robot()
  {
    delete configs;
    delete connection;
    delete motors;
    delete moves;
  }

  void Robot::loadEnvironment(string environment)
  {
    this->setEnvironment(environment);
    checkFixEnvironmentPath();
   
    string env1 = this->environment;
    string env2 = this->environment;
    env1.append("ConfigFiles/LowLevelConfig.xml");
    env2.append("ConfigFiles/MoveSchedulerConfig.xml");
    this->loadLowLevelConfig(env1);
    this->loadMoveSchedulerConfig(env2);  
  }

  void Robot::checkFixEnvironmentPath()
  {
    string::iterator it;
    it = environment.end();
    it--;
    if(*it != '/')
      environment.push_back('/');
  }
  
  void Robot::connect(const char *adress, int port)
  {
    connection->connectTo(adress, port);
  }

  int Robot::isConnected()
  {
    return connection->isConnected();
  }

  ui32 Robot::serverVersion()
  {
    return connection->ServerGetVersion_response()->read_uint();
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

  void Robot::loadLowLevelConfig(string filename, bool force)
  {
    configs->loadLowLevelConfig(filename, force);
  }

  void Robot::loadMoveSchedulerConfig(string filename, bool force)
  {
    configs->loadMoveSchedulerConfig(filename, force);
    motors->setConfig(configs->getMoveSchedulerConfig());
  }

  void Robot::allCompliant()
  {
    motors->allCompliant();
  }

  string Robot::moveFileName(string name)
  {
    string retval = getEnvironment();
    retval.append("Moves/");
    retval.append(name);
    retval.append(".xml");
    return retval;
  }

  void Robot::loadMove(string name)
  {
    string filename = name;
    filename.append(".graphics");
    filename = moveFileName(filename);
    
    ifstream ifile(filename.c_str());
    bool fileExists = (bool)ifile;
    ifile.close();
    
    if(fileExists)
      moves->loadMove(filename);
    else
      moves->loadMove(moveFileName(name));
  }

  void Robot::startMove(string name, ui32 duration, ui32 smooth)
  {
    moves->startMove(name, duration, smooth);
  }

  void Robot::pauseMove(string name)
  {
    moves->pauseMove(name);
  }

  void Robot::stopMove(string name, ui32 smooth)
  {
    moves->stopMove(name, smooth);
  }

  void Robot::killMove(string name)
  {
    moves->killMove(name);
  }

  vector<string> Robot::getLoadedMoves()
  {
    return moves->getLoadedMoves();
  }

  void Robot::updateConstant(string moveName, string constantName, vector<float> values)
  {
    moves->updateConstant(moveName, constantName, values);
  }

  void Robot::emergency()
  {
    connection->SchedulerEmergencyStop();
  }

  void Robot::stop()
  {
    connection->stop();
    motors->stop();
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

  void Robot::setEnvironment(string path)
  {
    environment = path;
    checkFixEnvironmentPath();
  }
  
  string Robot::getEnvironment()
  {
    checkFixEnvironmentPath();
    return environment;
  }
  void Robot::setMoves(Moves *moves)
  {
    this->moves = moves;
  }
  
  Moves *Robot::getMoves()
  {
    return moves;
  }
}
