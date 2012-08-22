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
    delete connection;
    delete lowLevelConfig;
    delete moveSchedulerConfig;
  }

  void Configurations::loadLowLevelConfig(string filename, bool force)
  {

  }

  void Configurations::loadMoveSchedulerConfig(string filename, bool force)
  {

  }

  Connection *Configurations::getConnection()
  {
    return NULL;
  }

  void Configurations::setConnection(Connection *connection)
  {

  }

  LowLevelConfig *Configurations::getLowLevelConfig()
  {
    return NULL;
  }

  void Configurations::setLowLevelConfig(LowLevelConfig *lowLevelConfig)
  {

  }

  MoveSchedulerConfig *Configurations::getMoveSchedulerConfig()
  {
    return NULL;
  }
  
  void Configurations::setMoveSchedulerConfig(MoveSchedulerConfig *moveSchedulerConfig)
  {

  }
}
