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
#include <string>
#include <file_mngt.h>
#include <communication/Connection.h>
#include <config/LowLevelConfig.h>
#include <config/MoveSchedulerConfig.h>
#include "Configurations.h"

using namespace std;

namespace Rhoban
{

  Configurations::Configurations(Connection *connection)
  {
    this->connection = connection;
    lowLevelConfig = NULL;
    moveSchedulerConfig = NULL;
  }

  Configurations::~Configurations()
  {
    delete lowLevelConfig;
    delete moveSchedulerConfig;
  } 

  bool Configurations::isMoveSchedulerConfigLoaded()
  {
    Message *response = NULL;
    while(response == NULL)
      response = connection->SchedulerConfigIsLoaded_response();

    return response->read_uint();
  }

  void Configurations::loadMoveSchedulerConfig(string config, bool force)
  {
    this->moveSchedulerConfig = new MoveSchedulerConfig(config);
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    cout << config << endl;
    //cout << "moveSchedulerConfig->getConfig() = " << moveSchedulerConfig->getConfig() << endl << endl;
    //////////////////////////////////////////////////////////////////////////////////////////////////////////

    if(force || isMoveSchedulerConfigLoaded() == 0)
      {
	//connection->SchedulerLoadConfig(config);
	//connection->SchedulerLoadConfig(moveSchedulerConfig->getConfig());
	string configContents = file_to_string(config);
	connection->SchedulerLoadConfig(configContents);
	connection->ServosScan(250, "Normal");
      }
  }
 
  bool Configurations::isLowLevelConfigLoaded()
  {
    Message *response = NULL;
    while(response == NULL)
      response = connection->LowLevelConfigIsLoaded_response();

    return response->read_uint();
  }

  void Configurations::loadLowLevelConfig(string config, bool force)
  {
    lowLevelConfig = new LowLevelConfig(config);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    cout << endl << config << endl;
    //cout << "lowLevelConfig->getConfig() = " << lowLevelConfig->getConfig() << endl << endl;
    cout << "TESTBLI : " << lowLevelConfig->dynamixel_config->frequency << endl;
    //////////////////////////////////////////////////////////////////////////////////////////////////////////

    if(force || isLowLevelConfigLoaded() == 0)
      {
	//connection->LowLevelLoadConfig(config);
	//connection->LowLevelLoadConfig(lowLevelConfig->getConfig());
	string configContents = file_to_string(config);
	connection->LowLevelLoadConfig(configContents);
      }
  }

  Connection *Configurations::getConnection()
  {
    return connection;
  }

  void Configurations::setConnection(Connection *connection)
  {
    this->connection = connection;
  }

  LowLevelConfig *Configurations::getLowLevelConfig()
  {
    return lowLevelConfig;
  }

  void Configurations::setLowLevelConfig(LowLevelConfig *lowLevelConfig)
  {
    this->lowLevelConfig = lowLevelConfig;
  }

  MoveSchedulerConfig *Configurations::getMoveSchedulerConfig()
  {
    return moveSchedulerConfig;
  }
  
  void Configurations::setMoveSchedulerConfig(MoveSchedulerConfig *moveSchedulerConfig)
  {
    this->moveSchedulerConfig = moveSchedulerConfig;
  }
}
