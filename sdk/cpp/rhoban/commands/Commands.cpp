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
#include "ticks.h"

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
    
    if(arguments.size() > 1)
      smooth = atoi(arguments[1].c_str());

    robot->stopMove(arguments[0], smooth);
  }
  
  
  UpdateConstantCommand::UpdateConstantCommand()
  {
    name = "updateconstant";
    argumentsLength = 4;
    prototype = "<robot> <moveName> <constantName> -- <value>";
    description = "Updates a move constant (-- before value for negative values)";
  }

  void UpdateConstantCommand::execute(Robot *robot, map<char, string> options, 
				      vector<string> arguments)
  {
    robot->updateConstant(arguments[0], arguments[1], atof(arguments[2].c_str()));
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
    description = "Set a motor angle (in °), -r for relative angle, -- befor angle for negative values";
    prototype = "[-r] <robot> <motorName> -- <angle> [load=1 [speed=1]]";
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
    options = "rp";
    prototype = "[-r] [-p]";
    description = "Snapshots the angles' values  (-r for relative angles, p for python format)";
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
    if(options.count('p'))
      {
	cout << "{";
	bool testval=0;
	for(it2=snapshot.begin(); it2!=snapshot.end(); ++it2)
	  {
	    if(testval)
	      cout << ", ";
	    else
	      testval=1;
	    cout << "'" << it2->first << "': " << it2->second;
	  }	
	cout << "}" << endl;
      }
    else
      {
	for(it2=snapshot.begin(); it2!=snapshot.end(); ++it2)
	  cout << it2->first << " : " << it2->second << endl;
      }
  } 

  
  MonitorCommand::MonitorCommand()
  {
    name = "monitor";
    prototype = "[-f frequency] [-i] <robotName>";
    options = "f:ic";
    argumentsLength = 1;
    description = "Monitors the servos (-i sorts by id)";
  }

  void MonitorCommand::execute(Robot *robot, map<char, string> options, 
			       vector<string> arguments)
  {
    string fmttitle = " | %-10s | %-4s | %-7s | %-12s | %-8s | %-8s |\n";
    string fmtloaded = " | %-10.10s | %-4d | %-7s | %-12f | %-12f | %-7d%% |\n";
    string fmtnotloaded = " | %-10.10s | %-4d | %-7s | %-12c | %-8c | %-8c |\n";

    bool csv = options.count('c') > 0;

    float frequency;
    if(options.count('f'))
      frequency = atof(options['f'].c_str());
    else
      frequency = 1;
  
    robot->getMotors()->start(3*frequency);

    map<int, Motor*> idmotors;
    map<int, Motor*>::iterator idit;
    
    map<string, Motor*> motors;
    map<string, Motor*>::iterator it;
	    
    while(1)
      {
          if (csv) {
            motors = robot->getMotors()->getMotors();
                    
            for(it=motors.begin(); it!=motors.end(); ++it)
              {
                if(it->second->getLastUpdate()) {
                    printf("%g/%g ",
                         it->second->getRelAngle(),
                         it->second->getCurrentRelGoal());
                } else {
                    printf("0 ");
                }
              }
            printf("\n");
          } else {
            cout << " Monitoring " << robot->getName() << endl;

            cout << " ";
            for(int i=0; i<((10+4+7+12+8+8)+(5*3)+(2*2)); ++i)
              cout << "-";
            cout << endl;
            printf(fmttitle.c_str(), 
                   "Name", "Id", "Present", "Angle", "Goal", "Speed");
            cout << " ";
            for(int i=0; i<((10+4+7+12+8+8)+(5*3)+(2*2)); ++i)
              cout << "-";
            cout << endl;
            
            if(options.count('i'))
              {
                idmotors = robot->getMotors()->getIdMotors();
                
                for(idit=idmotors.begin(); idit!=idmotors.end(); ++idit)
                  {
                    if(idit->second->getLastUpdate())
                      printf(fmtloaded.c_str(), 
                             idit->second->getName().c_str(), 
                             idit->second->getId(), 
                             "Yes", 
                             idit->second->getRelAngle(),
                             idit->second->getCurrentRelGoal(),
                             (int)(100*idit->second->getLoad()));
                    else
                      {
                        printf(fmtnotloaded.c_str(), 
                               idit->second->getName().c_str(), 
                               idit->second->getId(), 
                               "No", '-', '-', '-');
                      }		  
                  }
              }
            
            else // (!options.count('i'))
              {
                motors = robot->getMotors()->getMotors();
                        
                for(it=motors.begin(); it!=motors.end(); ++it)
                  {
                    if(it->second->getLastUpdate())
                      printf(fmtloaded.c_str(), 
                             it->second->getName().c_str(), 
                             it->second->getId(), 
                             "Yes", 
                             it->second->getRelAngle(), 
                             it->second->getCurrentRelGoal(),
                             (int)(100*it->second->getLoad()));
                    else
                      printf(fmtnotloaded.c_str(), 
                             it->second->getName().c_str(), 
                             it->second->getId(), 
                             "No", '-', '-', '-');
                  }
              }

            cout << " ";
            for(int i=0; i<((10+4+7+12+8+8)+(5*3)+(2*2)); ++i)
              cout << "-";
            cout << endl;
          }
	
	syst_wait_ms(1000/frequency);
      }
  }
  

  SensorsCommand::SensorsCommand()
  {
    name = "sensors";
    prototype = "[-f frequency] [-c] <robotName>";
    options = "f:c";
    argumentsLength = 1;
    description = "Monitor the sensors values";
  }
  
  void SensorsCommand::execute(Robot *robot, map<char, string> options,
			       vector<string> arguments)
  {
    string fmttitle = " | %10s | %10s |\n";
    string fmtloaded = " | %10s | %10f |\n";
    string fmtnotloaded = " | %10s | %10s |\n";
    
    bool csv = options.count('c');
    float frequency;
    if(options.count('f'))
      frequency = atof(options['f'].c_str());
    else
      frequency = 5;
    
    robot->getSensors()->start(frequency);
  
    while(1)
      {
          if (csv) {
            map<string, Sensor*> sensors = robot->getSensors()->getSensors();
            map<string, Sensor*>::iterator it;
            for(it=sensors.begin(); it!=sensors.end(); ++it)
              {
                if(!it->second->getValues().empty()) {
                    printf("%f ", it->second->getValue());
                } else {
                  printf("0 "); 
                }
              }
            printf("\n");

          } else {
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
          }
	
	syst_wait_ms(1000/frequency);
      }
  }    

  DumpCommand::DumpCommand()
  {
    name = "dump";
    prototype = "[-f frequency] [-c] <robotName>";
    options = "f:c";
    argumentsLength = 1;
    description = "Dump into CSV all motors and sensors read values";
  }
  
  void DumpCommand::execute(Robot *robot, map<char, string> options,
			       vector<string> arguments)
  {
      float frequency;
      if(options.count('f'))
          frequency = atof(options['f'].c_str());
      else
          frequency = 5;

      robot->getSensors()->start(frequency);
      robot->getMotors()->start(frequency);
      //robot->getMotors()->start(3*frequency);
      map<string, Sensor*> sensors;
      map<string, Sensor*>::iterator itS;
      map<string, Motor*> motors;
      map<string, Motor*>::iterator itM;

      printf("###\n");
      printf("### Dump %s\n", robot->getName().c_str());
      printf("###\n");
      int index = 1;
      sensors = robot->getSensors()->getSensors();
      for(itS=sensors.begin(); itS!=sensors.end(); ++itS)
      {
          printf("### %d -> [sensor] %s\n", index, itS->second->getName().c_str());
          index++;
      }
      motors = robot->getMotors()->getMotors();
      for(itM=motors.begin(); itM!=motors.end(); ++itM)
      {
          printf("### %d -> [position] %s\n", index, itM->second->getName().c_str());
          index++;
      }
      for(itM=motors.begin(); itM!=motors.end(); ++itM)
      {
          printf("### %d -> [goal] %s\n", index, itM->second->getName().c_str());
          index++;
      }
      printf("###\n");
      /*
      printf("#");
      for(itS=sensors.begin(); itS!=sensors.end(); ++itS)
      {
          printf("%s ", itS->second->getName().c_str());
      }
      for(itM=motors.begin(); itM!=motors.end(); ++itM)
      {
          printf("%s ", itM->second->getName().c_str());
      }
      printf("\n");
      */
      while (1) {
          sensors = robot->getSensors()->getSensors();
          for(itS=sensors.begin(); itS!=sensors.end(); ++itS)
          {
              if(!itS->second->getValues().empty()) {
                  printf("%f ", itS->second->getValue());
              } else {
                  printf("0 "); 
              }
          }
            
          motors = robot->getMotors()->getMotors();
          for(itM=motors.begin(); itM!=motors.end(); ++itM)
          {
              if(itM->second->getLastUpdate()) {
                  printf("%f ", itM->second->getRelAngle());
              } else {
                  printf("0 ");
              }
          }
          for(itM=motors.begin(); itM!=motors.end(); ++itM)
          {
              if(itM->second->getLastUpdate()) {
                  printf("%f ", itM->second->getCurrentRelGoal());
              } else {
                  printf("0 ");
              }
          }

          printf("\n");
          syst_wait_ms(1000/frequency);
      }
  }

  FramesDisplayCommand::FramesDisplayCommand()
  {
    name = "framesdisplay";
    prototype = "[-w width=300] [-h height=200] [-i src,ball,...] <robotName>";
    options = "w:h:i:";
    argumentsLength = 1;
    description = "Display frames vision camera";
  }
  
  void FramesDisplayCommand::execute(Robot *robot, map<char, string> options,
    vector<string> arguments)
  {
    unsigned int width = 300;
    unsigned int height = 200;
    vector<string> imgNames;

    if (options.count('w')) {
        width = atoi(options['w'].c_str());
    }
    if (options.count('h')) {
        height = atoi(options['h'].c_str());
    }
    if (options.count('i')) {
        string names = options['i'];
        size_t pos1 = 0;
        size_t pos2 = 0;
        while ((pos2 = names.find_first_of(",", pos1)) != string::npos) {
            imgNames.push_back(names.substr(pos1, pos2-pos1));
            pos1 = pos2+1;
        }
        imgNames.push_back(names.substr(pos1));
    } else {
        imgNames.push_back("src");
    }
    cout << "Displaying vision module " << width << "x" << height << endl;
    #ifdef WITH_OPENCV
        while (1) {
            robot->getVision()->grabAndDisplayFrames(imgNames, width, height);
        }
    #else
        cout << "Error: Compilation without OPENCV" << endl;
    #endif
  }
  
  BallInfoCommand::BallInfoCommand()
  {
    name = "ballinfo";
    prototype = "[-c] <robotName>";
    options = "c";
    argumentsLength = 1;
    description = "Monitor Ball info from vision. -c enable clipping";
  }
  
  void BallInfoCommand::execute(Robot *robot, map<char, string> options,
    vector<string> arguments)
  {
      bool clipping = false;
      if (options.count('c')) {
          clipping = true;
      }

      cout << "Reading Ball infos" << endl;
      cout << "Clipping ";
      if (clipping) {
          cout << "On" << endl;
      } else {
          cout << "Off" << endl;
      }
      while (1) {
          robot->getVision()->grabBallInfo(clipping);
          if (robot->getVision()->getIsBallDetected()) {
              cout << "posX=" << robot->getVision()->getBallRelPosX() << " ";
              cout << "posY=" << robot->getVision()->getBallRelPosY() << " ";
              cout << "radius=" << robot->getVision()->getBallRelRadius() << endl;
          } else {
              cout << "No ball detected" << endl;
          }
      }
  }
  
  GoalInfoCommand::GoalInfoCommand()
  {
    name = "goalinfo";
    prototype = "<robotName>";
    argumentsLength = 1;
    description = "Monitor Goal info from vision";
  }
  
  void GoalInfoCommand::execute(Robot *robot, map<char, string> options,
    vector<string> arguments)
  {
      cout << "Reading Goal infos" << endl;
      while (1) {
          robot->getVision()->grabGoalInfo();
          if (robot->getVision()->getIsGoalDetected()) {
              cout << "posX=" << robot->getVision()->getGoalRelPosX() << " ";
              cout << "posY=" << robot->getVision()->getGoalRelPosY() << " ";
              cout << "width=" << robot->getVision()->getGoalRelWidth() << " ";
              cout << "height=" << robot->getVision()->getGoalRelHeight() << endl;
          } else {
              cout << "No goal detected" << endl;
          }
      }
  }
  
  BallCalibrationCommand::BallCalibrationCommand()
  {
    name = "ballcalibration";
    prototype = "<robotName>";
    argumentsLength = 1;
    description = "Calibrate the ball detection process";
  }
  
  void BallCalibrationCommand::execute(Robot *robot, map<char, string> options,
    vector<string> arguments)
  {
    cout << "Calibrating ball detection process" << endl;
    #ifdef WITH_OPENCV
      robot->getVision()->ballCalibration();
    #else
      cout << "Error: Compilation without OPENCV" << endl;
    #endif
  }
}
