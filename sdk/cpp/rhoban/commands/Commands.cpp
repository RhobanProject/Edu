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
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <robot/Robot.h>
#include <motors/Motors.h>
#include "RobotCommand.h"
#include "RobotsCommand.h"
#include "Commands.h"

using namespace std;

namespace Rhoban
{
  StatusCommand::StatusCommand()
  {
    name = "status";
    prototype = "[-v]";
    description = "Displays the connection's status, -v for detailed informations";
    options = "v";
  }
  
  void StatusCommand::executeFor(Robot *robot, map<char, string> options)
  {
    cout << ". Status for " << robot->getName() << " :" << endl;

    //Connection
    if(robot->isConnected())
      {
      cout << "|- Server version : " << robot->serverVersion() << endl; 
      cout << "|- Connected at " << robot->getHostname() 
	   << ":" << robot->getPort() << endl;
      }
    else
      {
      cout << "|- Disconnected" << endl;
      return;
      }
    
    //Configurations
    cout << "|- Low level config : ";
    if(!robot->getConfigs()->isLowLevelConfigLoaded())
      cout << "not ";
    cout << "loaded" << endl;

    cout << "|- Move scheduler config : ";
    if(!robot->getConfigs()->isMoveSchedulerConfigLoaded())
      cout << "not ";
    cout << "loaded" << endl;

    // Motors
    map<int, Motor*> motors = robot->getMotors()->getIdMotors();
    map<int, Motor*>::iterator it;

    robot->getMotors()->pullValues();

    int val=0;
    for(it=motors.begin(); it!=motors.end(); ++it)
      {
	if(it->second->getLastUpdate())
	  val++;
      }

    cout << "|- " << val << "/" << motors.size() << " motors (";

    bool testval = 0;
    for(it=motors.begin(); it!=motors.end(); ++it)
      {
	if(testval)
	  cout << ", ";
	else
	  testval = 1;
	cout << it->first;
      }

    cout << ")" << endl;
    
    if(options.count('v'))
      {
	string prefix = " \\- ";

	for(it=motors.begin(); it!=motors.end(); ++it)
	  {
	    if(it->second->getLastUpdate())
	      cout << prefix << it->second->getName() 
		   << " (id : " << it->first 
		   << "), angle : " << it->second->getAngle() 
		   << ", speed : " << it->second->getSpeed() 
		   << ", load : " << it->second->getLoad() << endl;
	    else
	      cout << prefix << it->second->getName()
		   << " (id : " << it->second->getId()
		   << ") Not detected" << endl;
	    prefix = " |- ";
	  }
      }
    
    // Moves
    vector<string> moves = robot->getLoadedMoves();
    cout << "|- Loaded moves : " << moves.size() << endl;
    
    if(options.count('v'))
      {
	string prefix = " \\- ";
	for(int i=0; i<moves.size(); ++i)
	  {
	    cout << prefix << moves[i] << endl;
	    prefix = " |- ";
	  }
      }
    
    cout << endl;
  }
  
  
  LoadMoveCommand::LoadMoveCommand()
  {
    name = "loadmove";
    prototype = "<robot> <moveName>";
    argumentsLength = 2;
    description = "Loads the move moveName";
  }

  void LoadMoveCommand::execute(Robot *robot, map<char, string> options, 
				vector<string> arguments)
  {
    robot->loadMove(arguments[0]);
  }
  
 
  KillMoveCommand::KillMoveCommand()
  {
    name = "killmove";
    prototype = "<robot> <moveName>";
    argumentsLength = 2;
    description = "Kills the move moveName (stop and unload it)";
  }

  void KillMoveCommand::execute(Robot *robot, map<char, string> options, 
				vector<string> arguments)
  {
    robot->killMove(arguments[0]);
  }

  
  StartMoveCommand::StartMoveCommand()
  {
    name = "startmove";
    prototype = "<robot> <moveName> [duration=0 [smooth=500]]";
    argumentsLength = 2;
    description = "Starts the move moveName";
  }

  void StartMoveCommand::execute(Robot *robot, map<char, string> options, 
				 vector<string> arguments)
  {
    int duration = 0;
    int smooth = 500;

    if(arguments.size() > 1)
      {
	duration = atoi(arguments[1].c_str());
	if(arguments.size() > 2)
	  smooth = atoi(arguments[2].c_str());
      }
    
    robot->startMove(arguments[0], duration, smooth);
  }

  
  StopMoveCommand::StopMoveCommand()
  {
    name = "stopmove";
    argumentsLength = 2;
    prototype = "<robot> <moveName> [smooth=500]";
    description = "Stops the move moveName";
  }

  void StopMoveCommand::execute(Robot *robot, map<char, string> options, 
				vector<string> arguments)
  {
    int smooth = 500;
    
    if(arguments.size() > 0)
      smooth = atoi(arguments[1].c_str());

    robot->stopMove(arguments[0], smooth);
  }
  
  
  UpdateConstantCommand::UpdateConstantCommand()
  {
    name = "updateconstant";
    argumentsLength = 4;
    prototype = "<robot> <moveName> <constantName> <value>";
    description = "Updates a move constant";
  }

  void UpdateConstantCommand::execute(Robot *robot, map<char, string> options, 
				      vector<string> arguments)
  {
    robot->updateConstant(arguments[0], arguments[1], arguments[2]);
  }  

  
  CompliantCommand::CompliantCommand()
  {
    name = "compliant";
    description = "Release all the servos";
  }
  
  void CompliantCommand::executeFor(Robot *robot, map<char, string> options)
  {
    cout << "Releasing servos for " << robot->getName() << "..." << endl;
    robot->getMotors()->allCompliant();
  } 

  
  HardCommand::HardCommand()
  {
    name = "hard";
    description = "Harden all the servos";
  }

  void HardCommand::executeFor(Robot *robot, map<char, string> options)
  {
    cout << "harden servos for " << robot->getName() << "..." << endl;
    robot->getMotors()->pullValues();
    robot->getMotors()->allHard();
    robot->getMotors()->pushValues();
  }
  
  
  SetCommand::SetCommand()
  {
    name = "set";
    options = "r";
    description = "Set a motor angle (in °), -r for relative angle";
    prototype = "[-r] <robot> <motorName> <angle> [load=1 [speed=1]]";
    argumentsLength = 3;
  }
  
  void SetCommand::execute(Robot *robot, map<char, string> options, 
			   vector<string> arguments)
  {
    Motors *motors = robot->getMotors();
    motors->pullValues();
    Motor *motor = (*motors)[arguments[0]];
    
    if(options.count('r'))
      motor->setRelAngle(atof(arguments[1].c_str()));
    else
      motor->setAngle(atof(arguments[1].c_str()));

    if(arguments.size()>2)
      motor->setLoad(atof(arguments[2].c_str()));
    else
      motor->setLoad(1);

    if(arguments.size()>3)
      motor->setSpeed(atof(arguments[3].c_str()));
    else
      motor->setSpeed(1);

    cout << "Setting motor " << motor->getName() 
	 << " (id = " << motor->getId() 
	 << ") angle = " << motor->getGoalAngle() 
	 << ", load = " << motor->getGoalLoad() 
	 << ", speed = " << motor->getGoalSpeed() << endl;
  
    motors->pushValues();
  }  

  
  InitCommand::InitCommand()
  {
    name = "init";
    description = "Take the initial position for servos";
  }
  
  void InitCommand::executeFor(Robot *robot, map<char, string> options)
  {
    cout << "Taking initial servos position for " << robot->getName() << endl;
    robot->getMotors()->goToInit(5, true);
  }  

  
  ZeroCommand::ZeroCommand()
  {
    name = "zero";
    description = "Take the zero position for servos";
  }

  void ZeroCommand::executeFor(Robot *robot, map<char, string> options)
  {
    cout << "Taking zero servos position for " << robot->getName() << endl;
    robot->getMotors()->goToZero(5, true);
  }  

  
  ScanCommand::ScanCommand()
  {
    name = "scan";
    description = "Run a scan";
  }

  void ScanCommand::executeFor(Robot *robot, map<char, string> options)
  {
    cout << "Running a scan for " << robot->getName() << endl;
    robot->getMotors()->scan();
  }
  

  EmergencyCommand::EmergencyCommand()
  {
    name = "emergency";
    description = "Emergency stop";
  }
  
  void EmergencyCommand::executeFor(Robot *robot, map<char, string> options)
  {
    cout << "Emergency stopping all move and turning compliant for " 
	 << robot->getName() << endl;
    robot->emergency();
  } 

  
  SnapshotCommand::SnapshotCommand()
  {
    name = "snapshot";
    options = "r";
    prototype = "[-r]";
    description = "Snapshots the angles' values  (-r for relative angles)";
  }

  void SnapshotCommand::executeFor(Robot *robot, map<char, string> options)
  {
    cout << "Snapshoting motors for " << robot->getName() << endl;
    robot->getMotors()->pullValues();
    
    map<string, double> snapshot;

    map<string, Motor*> motors = robot->getMotors()->getMotors();
    map<string, Motor*>::iterator it;
    for(it=motors.begin(); it!=motors.end(); ++it)
      {
	if(it->second->getLastUpdate())
	  {
	    if(options.count('r'))
	      snapshot[it->second->getName()] = it->second->getRelAngle();
	    else
	      snapshot[it->second->getName()] = it->second->getAngle();
	  }
      }
    
    map<string, double>::iterator it2;
    for(it2=snapshot.begin(); it2!=snapshot.end(); ++it)
      cout << it2->first << " : " << it2->second << endl;
  } 

  
  MonitorCommand::MonitorCommand()
  {
    name = "monitor";
    prototype = "[-f frequency] [-i] <robotName>";
    options = "f:i";
    argumentsLength = 1;
    description = "Monitors the servos (-i sorts by id)";
  }

  void MonitorCommand::execute(Robot *robot, map<char, string> options, 
			       vector<string> arguments)
  {
    string fmttitle = " | %-10s | %-4s | %-7s | %-12s | %-8s | %-8s |\n";
    string fmtloaded = " | %-10.10s | %-4d | %-7s | %-12.12f | %-7d%% | %-7d%% |\n";
    string fmtnotloaded = " | %-10.10s | %-4d | %-7s | %-12.12f | %-8s | %-8s |\n";

    float frequency;
    if(options.count('f'))
      frequency = atof(options['f'].c_str());
    else
      frequency = 1;
  
    robot->getMotors()->start(3*frequency);

    while(1)
      {
	cout << " Monitoring " << robot->getName() << endl;

	cout << " ";
	for(int i=0; i<((10+4+7+12+8+8)+(5*3)+(2*2)); ++i)
	  cout << "-";
	cout << endl;
	printf(fmttitle.c_str(), 
	       "Name", "Id", "Present", "Angle", "Load", "Speed");
	cout << " ";
	for(int i=0; i<((10+4+7+12+8+8)+(5*3)+(2*2)); ++i)
	  cout << "-";
	cout << endl;
	
	if(options.count('i'))
	  {
	    map<int, Motor*> motors = robot->getMotors()->getIdMotors();
	    map<int, Motor*>::iterator it;

	    for(it=motors.begin(); it!=motors.end(); ++it)
	      {
		if(it->second->getLastUpdate())
		  printf(fmtloaded.c_str(), 
			 it->second->getName().c_str(), 
			 it->second->getId(), 
			 "Yes", 
			 it->second->getAngle(), 
			 (int)(100*it->second->getLoad()), 
			 (int)(100*it->second->getLoad()));
		else
		  printf(fmtnotloaded.c_str(), 
			 it->second->getName().c_str(), 
			 it->second->getId(), 
			 "No", "-", "-", "-");		  
	      }
	  }
	
	else // (!options.count('i'))
	  {
	    map<string, Motor*> motors = robot->getMotors()->getMotors();
	    map<string, Motor*>::iterator it;
	    
	    for(it=motors.begin(); it!=motors.end(); ++it)
	      {
		if(it->second->getLastUpdate())
		  printf(fmtloaded.c_str(), 
			 it->second->getName().c_str(), 
			 it->second->getId(), 
			 "Yes", 
			 it->second->getAngle(), 
			 (int)(100*it->second->getLoad()), 
			 (int)(100*it->second->getLoad()));
		else
		  printf(fmtnotloaded.c_str(), 
			 it->second->getName().c_str(), 
			 it->second->getId(), 
			 "No", "-", "-", "-");
	      }
	  }
	cout << " ";
	for(int i=0; i<((10+4+7+12+8+8)+(5*3)+(2*2)); ++i)
	  cout << "-";
	cout << endl;
	
	usleep(1000000/frequency);	
      }
  }
  

  SensorsCommand::SensorsCommand()
  {
    name = "sensors";
    prototype = "<robotName>";
    argumentsLength = 1;
    description = "Monitor the sensors values";
  }
  
  void SensorsCommand::execute(Robot *robot, map<char, string> options,
			       vector<string> arguments)
  {
    robot->getSensors()->start(5);
    string fmttitle = " | %10s | %10s |\n";
    string fmtloaded = " | %10s | %10d |\n";
    string fmtnotloaded = " | %10s | %10s |\n";
  
    while(1)
      {
	cout << " Monitoring sensors of " << robot->getName() << endl;
	
	cout << " ";
	for(int i=0; i<(10+10+3+2+2); ++i)
	  cout << "-";
	cout << endl;
	printf(fmttitle.c_str(), "Name", "Value");
	cout << " ";
	for(int i=0; i<(10+10+3+2+2); ++i)
	  cout << "-";
	cout << endl;

	map<string, Sensor*> sensors = robot->getSensors()->getSensors();
	map<string, Sensor*>::iterator it;
	for(it=sensors.begin(); it!=sensors.end(); ++it)
	  {
	    if(!it->second->getValues().empty())
	      printf(fmtloaded.c_str(), 
		     it->second->getName().c_str(), it->second->getValue());
	    else
	      printf(fmtnotloaded.c_str(), 
		     it->second->getName().c_str(), "?");
	  }
	
	cout << " ";
	for(int i=0; i<(10+10+3+2+2); ++i)
	  cout << "-";
	cout << endl;
	
	usleep(500000);
      }
  }    
}