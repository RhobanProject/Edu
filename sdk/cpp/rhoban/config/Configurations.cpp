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
#include "Configurations.h"
#include "LowLevelConfig.h"
#include "MoveSchedulerConfig.h"

using namespace std;

namespace Rhoban
{

  Configurations::Configurations()
  {

  }

  Configurations::~Configurations()
  {

  }

  void Configurations::loadLowLevelConfig(string filename)
  {

  }

  void Configurations::loadMoveSchedulerConfig(string filename)
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
