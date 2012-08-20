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
#include <map>
#include <string>
#include <threading/Thread.h>
#include <config/MoveSchedulerConfig.h>
#include <communication/Connection.h>
#include <robot/Robot.h>
#include <utils/types.h>
#include "Motor.h"
#include "Motors.h"

using namespace std;

namespace Rhoban
{
  Motors::Motors(Connection *connection) : Thread()
  {
    this->connection = connection;
    this->configuration = NULL;
    running = 0;
  }

  Motors::~Motors()
  {
    delete connection;
    delete configuration;
  }

  size_t Motors::len()
  {
    return motors.size();
  }

  Motor *Motors::operator[](string name)
  {
    return motors[name];
  }
  
  void Motors::start(int frequency)
  {
    this->frequency = frequency;
    
    if(configuration == NULL)
      cout << "No suitable motors configuration" << endl;
    else
      {
	vector<ServoConfig>::iterator it;
	for(it = configuration->servos_config.servos.begin(); 
	    it != configuration->servos_config.servos.end(); ++it)
	  {	    
	    motors[it->Name] = new Motor(it->Id, it->Name);
	    idMotors[it->Id] = motors[it->Name];
	  }
	running = 1;
	Thread::start(NULL);
      }
  }

  void Motors::stop()
  {
    running = 0;
  }

  void Motors::compliant(string name)
  {
    motors[name]->setGoalLoad(0);
  }

  void Motors::allCompliant()
  {
    connection->ServosAllCompliant();
    
    map<string, Motor *>::iterator it;
    for(it = motors.begin(); it != motors.end(); ++it)
      it->second->setGoalLoad(0);
  }

  void Motors::hard(string name)
  {
    motors[name]->setGoalLoad(1);
  }

  void Motors::allHard(string name)
  {
    map<string, Motor *>::iterator it;
    for(it = motors.begin(); it != motors.end(); ++it)
      it->second->setGoalLoad(1);    
  }
  
  void Motors::run()
  {
    map<string, Motor *>::iterator it;
    vector<byte> ids;
    vector<int> angles;
    vector<ui32> speeds;
    vector<ui32> loads;

    //todo definir le callback motorsValues

    while(running)
      {
	//	connection->ServosGetValues_callback(1, motorsValues);

	ids.clear();
	angles.clear();
	speeds.clear();
	loads.clear();
 
	for(it = motors.begin(); it != motors.end(); ++it)
	  {
	    if(it->second->isDirty()
	       && it->second->getGoalAngleInit()
	       && it->second->getCurrentAngleInit()   
	       && it->second->getCurrentSpeedInit())
	      {
		ids.push_back(it->second->getId());
		angles.push_back(it->second->getGoalAngle()*1000);
		speeds.push_back(it->second->getGoalSpeed()*1023);
		loads.push_back(it->second->getGoalLoad()*1023);
	      }
	  }

	if(ids.size())
	  connection->ServosSetValues(1, ids, angles, speeds, loads);
	
	usleep(1000000/frequency);
      }
  }

  void Motors::motorsValue(Message *values)
  {
    vector<byte> ids = values->read_array();
    vector<float> angles= values->read_float_array();
    vector<float> speeds= values->read_float_array();
    vector<float> loads= values->read_float_array();
    
        
  }
  
  void Motors::execute()
  {
    // ??
  }

  void Motors::setConnection(Connection *connection)
  {
    this->connection = connection;
  }

  Connection * Motors::getConnection()
  {
    return connection;
  }

  void Motors::setConfig(MoveSchedulerConfig *config)
  {
    this->configuration = config;
  }
  
  MoveSchedulerConfig *Motors::getConfig()
  {
    return configuration;
  }

  void Motors::setMotors(map<string, Motor *> motors)
  {
    this->motors = motors;
  }

  map<string, Motor *> Motors::getMotors()
  {
    return motors;
  }

  void Motors::setIdMotors(map<int, Motor *> idMotors)
  {
    this->idMotors = idMotors;
  }

  map<int, Motor *> Motors::getIdMotors()
  {
    return idMotors;
  }
  
  void Motors::setRunning(bool val)
  {
    this->running = val;
  }

  bool Motors::getRunning()
  {
    return running;
  }

  void Motors::setFrequency(int frequency)
  {
    this->frequency = frequency;
  }

  int Motors::getFrequency()
  {
    return frequency;
  }
}
