/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
#ifndef CONFIGURATIONS_H_
#define CONFIGURATIONS_H_

#include <cstdlib>
#include <cstdio>
#include <string>
#include <communication/Connection.h>
#include <config/LowLevelConfig.h>
#include <config/MoveSchedulerConfig.h>

using namespace std;

namespace Rhoban
{
  class Configurations
  {
  public:
    Configurations(Connection *connection);
    ~Configurations();

    bool isMoveSchedulerConfigLoaded();
    void loadMoveSchedulerConfig(string config, bool force = 0);
    bool isLowLevelConfigLoaded();
    void loadLowLevelConfig(string config, bool force = 0);
    
    Connection *getConnection();
    void setConnection(Connection *connection);
    LowLevelConfig *getLowLevelConfig();
    void setLowLevelConfig(LowLevelConfig *lowLevelConfig);
    MoveSchedulerConfig *getMoveSchedulerConfig();
    void setMoveSchedulerConfig(MoveSchedulerConfig *moveSchedulerConfig);
   
  protected:
    Connection *connection;
    LowLevelConfig *lowLevelConfig;
    MoveSchedulerConfig *moveSchedulerConfig;
  };
}

#endif // CONFIGURATIONS_H_
