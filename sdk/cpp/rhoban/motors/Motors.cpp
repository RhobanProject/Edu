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
#include <types.h>
#include "Motor.h"
#include "Motors.h"

using namespace std;

namespace Rhoban
{
  void motorsValues(Message *values, void *data)
  { 
    Motors *self = (Motors *)data;
    self->processValues(values);
  }

  Motors::Motors(Connection *connection) : Thread()
  {
    this->connection = connection;
    this->configuration = NULL;
    running = 0;
  }

  Motors::~Motors()
  {
    delete configuration;
  }

  void Motors::setConfig(MoveSchedulerConfig *config)
  {
    this->configuration = config;

    vector<ServoConfig>::iterator it;
    for(it = configuration->servos_config.servos.begin(); 
	it != configuration->servos_config.servos.end(); ++it)
      {	    
	motors[it->Name] = new Motor(it->Id, it->Name, 
				     it->IniAngle, it->ZeroAngle);
	idMotors[it->Id] = motors[it->Name];
      }
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
    motors[name]->setLoad(0);
  }

  void Motors::allCompliant()
  {
    connection->ServosAllCompliant();
    
    map<string, Motor *>::iterator it;
    for(it = motors.begin(); it != motors.end(); ++it)
      it->second->setLoad(0);
  }

  void Motors::hard(string name)
  {
    motors[name]->setLoad(1);
  }

  void Motors::allHard(string name)
  {
    map<string, Motor *>::iterator it;
    for(it = motors.begin(); it != motors.end(); ++it)
      it->second->setLoad(1);    
  }

  void Motors::pullValues()
  {
    Message *values = connection->ServosGetValues_response(1);
    processValues(values);
  }

  void Motors::pushValues()
  {
    map<string, Motor *>::iterator it;

    vector<byte> ids;
    vector<int> angles;
    vector<ui32> speeds;
    vector<ui32> loads;
    
    for(it = motors.begin(); it != motors.end(); ++it)
      {
	if(it->second->isDirty()
	   && it->second->getGoalAngleInit()
	   && it->second->getCurrentAngleInit()   
	   && it->second->getCurrentSpeedInit())
	  {
	    ids.push_back   ((byte) (it->second->getId())             );
	    angles.push_back((int)  (it->second->getGoalAngle()*1000) );
	    speeds.push_back((ui32) (it->second->getGoalSpeed()*1023) );
	    loads.push_back ((ui32) (it->second->getGoalLoad()*1023)  );
	  }
      }

    if(ids.size())
      connection->ServosSetValues(1, ids, angles, speeds, loads);
  }

  void Motors::processValues(Message *values)
  {
    vector<byte> ids = values->read_array();
    vector<float> angles= values->read_float_array();
    vector<float> speeds= values->read_float_array();
    vector<float> loads= values->read_float_array();
    
    for(int i=0; i < ids.size(); ++i)
      {
	if(idMotors.count(ids[i]))
	  {
	    idMotors[ids[i]]->setLastUpdate(time(NULL));
	    
	    idMotors[ids[i]]->setCurrentAngle((double)angles[i]);
	    if(idMotors[ids[i]]->getGoalAngleInit() == 0)
	      idMotors[ids[i]]->setGoalAngle(idMotors[ids[i]]->getAngle());
	    
	    idMotors[ids[i]]->setCurrentSpeed((double)speeds[i]/1023);
	    if(idMotors[ids[i]]->getGoalSpeedInit() == 0)
	      {
		if(idMotors[ids[i]]->getSpeed() > 0)
		  idMotors[ids[i]]->setGoalSpeed(idMotors[ids[i]]->getSpeed());
		else
		  idMotors[ids[i]]->setGoalSpeed(0);
	      }
	  }
	idMotors[ids[i]]->setCurrentLoad((double)loads[i]/1023);
	if(idMotors[ids[i]]->getGoalLoadInit() == 0)
	  {
	    if(idMotors[ids[i]]->getLoad() > 0)
	      idMotors[ids[i]]->setGoalLoad(idMotors[ids[i]]->getLoad());
	    else
	      idMotors[ids[i]]->setGoalLoad(0);
          }
      }   
  }
  
  void Motors::goToZero(int duration, bool verbose)
  {
    pullValues();
    
    map<string, Motor *>::iterator it;
    for(it = motors.begin(); it != motors.end(); ++it)
      it->second->setAngle(it->second->getZeroAngle());
    
    raiseLoad(duration, verbose);
  }

  void Motors::goToInit(int duration, bool verbose)
  {
    pullValues();
    
    map<string, Motor *>::iterator it;
    for(it = motors.begin(); it != motors.end(); ++it)
      it->second->setRelAngle(0);
    
    raiseLoad(duration, verbose);
  }

  void Motors::raiseLoad(int duration, bool verbose)
  {
    map<string, Motor *>::iterator it;
    int cs = duration * 100;

    for(int i=0; i < cs; i++)
      {
	if(verbose)
	  printf("\rLoad: %3d%%", ((i+1)*100)/cs);
	     
	for(it = motors.begin(); it != motors.end(); ++it)
	  it->second->setLoad(((double)i+1)/(double)cs);
	
	usleep(10000);
	pushValues();	  
      }
    if(verbose)
      cout << "" << endl;
  }
    
  void Motors::execute()
  {
    while(running)
      {
	connection->ServosGetValues_callback(1, motorsValues, this);
	pushValues();
	usleep(1000000/frequency);
      } 
  }

  void Motors::scan()
  {
    map<string, Motor *>::iterator it;
    for(it = motors.begin(); it != motors.end(); ++it)
      it->second->setLastUpdate(0);
    connection->ServosScan(250, "Normal");
  }

  void Motors::setConnection(Connection *connection)
  {
    this->connection = connection;
  }

  Connection * Motors::getConnection()
  {
    return connection;
  }

  void Motors::setConfiguration(MoveSchedulerConfig *config)
  {
    this->configuration = config;
  }
  
  MoveSchedulerConfig *Motors::getConfiguration()
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
