/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
#ifndef SENSORS_H
#define SENSORS_H

#include <cstdlib>
#include <cstdio>
#include <map>
#include <string>
#include <communication/Connection.h>
#include <threading/Thread.h>
#include "Sensor.h"

using namespace std;
 
namespace Rhoban
{  
  // Callback
  void processResponse(Message *response, void *data);
  
 class Sensors : public Thread
  {
  public:
    Sensors(Connection *connection);
    
    size_t len();
    Sensor *operator[](string name);
    Sensor *get(string name);
    bool isRunning();

    void setSensorValues(string name, vector<double> values);
    bool hasSensorNamed(string name);

    void stop();
    void start(int frequency);
    void execute();
    
    void setConnection(Connection *connection);
    Connection *getConnection();
    void setSensors(map<string, Sensor*> sensors);
    map<string, Sensor*> getSensors();
    void setRunning(bool running);
    bool getRunning();
    void setFrequency(int frequency);
    int getFrequency();

  protected:
    Connection *connection;
    map<string, Sensor *> sensors;
    bool running;
    int frequency;
  };
}

#endif // SENSORS_H
