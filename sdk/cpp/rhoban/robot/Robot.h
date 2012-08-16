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
#include <communication/Connection.h>
#include <communication/CommandsStore.h>
#include <motors/Motors.h>
#include <config/Configurations.h>

using namespace std;

namespace Rhoban
{
  class Robot
  {
  public:
    Robot(CommandsStore *commandsStore);
    ~Robot();
    void connect(const char *adress, int port);
    int isConnected();
    int testConnection();
    void loadLowLevelConfig(string filename);
    void loadMoveSchedulerConfig(string filename);
    void stop();
    void initialize();
    void moveMotor(string motorId, int angle);
    void compliant(string motorId);
    void allCompliant();

    void setMotors(Motors *motors);
    Motors* getMotors();
    void setConfigs(Configurations *configs);
    Configurations *getConfigs();
    void setConnection(Connection *connection);
    Connection *getConnection();

  protected:
    Motors *motors;
    Configurations *configs;
    Connection *connection;
  };
}

#endif // ROBOT_H_
