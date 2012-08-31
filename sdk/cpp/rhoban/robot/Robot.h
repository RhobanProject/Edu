/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
#ifndef ROBOT_H_
#define ROBOT_H_

namespace Rhoban
{
  class Motors;
}

#include <cstdlib>
#include <cstdio>
#include <string>
#include <communication/Connection.h>
#include <communication/CommandsStore.h>
#include <utils/types.h>
#include <motors/Motors.h>
#include <config/Configurations.h>
#include "Moves.h"
#include "Sensors.h"

using namespace std;

namespace Rhoban
{
  class Robot
  {
  public:
    Robot(CommandsStore *commandsStore);
    ~Robot();
    
    // Environment
    void loadEnvironment(string environment);
    void checkFixEnvironmentPath();
    
    // Connection
    void connect(const char *adress, int port);
    int isConnected();
    ui32 serverVersion();
    int testConnection();
    
    // Configuration
    void loadLowLevelConfig(string filename, bool force = 0);
    void loadMoveSchedulerConfig(string filename, bool force = 0);
    
    // Motors
    void allCompliant();
    
    // Moves
    string moveFileName(string name);
    void loadMove(string name);
    void startMove(string name, ui32 duration = 0, ui32 smooth = 500);
    void pauseMove(string name);
    void stopMove(string name, ui32 smooth = 500);
    void killMove(string name);
    vector<string> getLoadedMoves();
    void updateConstant(string moveName, string constantName, vector<float> values);
    void emergency();
    void stop();

    void setMotors(Motors *motors);
    Motors* getMotors();
    void setConfigs(Configurations *configs);
    Configurations *getConfigs();
    void setConnection(Connection *connection);
    Connection *getConnection();
    void setEnvironment(string path);
    string getEnvironment();
    void setMoves(Moves *moves);
    Moves *getMoves();
    void setSensors(Sensors *sensors);
    Sensors *getSensors();

  protected:
    Motors *motors;
    Configurations *configs;
    Connection *connection;
    string environment;
    Moves *moves;
    Sensors *sensors;
  };
}

#endif // ROBOT_H_
