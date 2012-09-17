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
#include <communication/Message.h>
#include <threading/Thread.h>
#include "Sensors.h"
#include "Sensor.h"

using namespace std;

namespace Rhoban
{
  void processResponse(Message *response, void *data) /////////////
  {
    Sensors *self = (Sensors *)data;
    
    vector<string> names = response->read_string_array();

    vector< vector<ui32> > values = response->read_uint_array_array();
    for(int i=0; i<names.size(); ++i)
      {
	if(self->hasSensorNamed(names[i]))
	  self->setSensorValues(names[i], values[i]);
      }
  }

  Sensors::Sensors(Connection *connection)
  {
    this->connection = connection;
  }
    
  size_t Sensors::len()
  {
    return sensors.size();
  }
  
  Sensor *Sensors::operator[](string name)
  {
    return sensors[name];
  }

  bool Sensors::isRunning()
  {
    return running;
  }

  void Sensors::setSensorValues(string name, vector<ui32> values)
  {
    sensors[name]->setValues(values);
  }

  bool Sensors::hasSensorNamed(string name)
  {
    return sensors.count(name);
  }

  void Sensors::stop()
  {
    running = 0;
  }

  void Sensors::start(int frequency)
  {
    this->frequency = frequency;
    sensors.clear();
    running = 1;

    Message *response = connection->LowLevelGetAnalogicSensors_response();
    if(response == NULL)
      cout << "Unable to get the sensors values" << endl;
    
    else
      {
	vector<string> names = response->read_string_array();
	for(int i=0; i<names.size(); ++i)
	  sensors[names[i]] = new Sensor(names[i]);
	
	Thread::start(NULL);
      }
  }

  void Sensors::execute()
  {
    vector<string> names;
    int queue = 5;

    map<string, Sensor *>::iterator it;
    for(it = sensors.begin(); it != sensors.end(); ++it)
      names.push_back(it->first);
    
    while(running)      
      {
	connection->GetSensorsValues_callback(names, queue, 
					      processResponse, this);
	usleep(1000000/frequency);
      }    
  }

  void Sensors::setConnection(Connection *connection)
  {
    this->connection = connection;
  }

  Connection *Sensors::getConnection()
  {
    return connection;
  }

  void Sensors::setSensors(map<string, Sensor*> sensors)
  {
    this->sensors = sensors;
  }

  map<string, Sensor*> Sensors::getSensors()
  {
    return sensors;
  }

  void Sensors::setRunning(bool running)
  {
    this->running = running;
  }

  bool Sensors::getRunning()
  {
    return running;
  }

  void Sensors::setFrequency(int frequency)
  {
    this->frequency = frequency;
  }

  int Sensors::getFrequency()
  {
    return frequency;
  }
}
 
