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
#include <robot/Robots.h>
#include <robot/Robot.h>
#include "Command.h"
#include "RobotsCommand.h"

using namespace std;

namespace Rhoban
{
  void RobotsCommand::run(map<char, string> options, vector<string> arguments)
  {
    robotsStore.loadYaml("config.yml");
    map<string, Robot *> robots = robotsStore.getRobots();
 
    if(arguments.empty())
      {
	map<string, Robot *>::iterator it;
	for(it=robots.begin(); it!=robots.end(); ++it)
	  executeFor(it->second, options);
      }
    else
      {
	for(int i=0; i<arguments.size(); ++i)
	  {
	    if(robots.count(arguments[i]))
	      executeFor(robots[arguments[i]], options);
	    else
	      {
		cout << "Robot " << arguments[i] << " not found" << endl;
		return;
	      }
	  }
      }
  }
  
  void RobotsCommand::stop()
  {
    robotsStore.stop();
  }
  
  void RobotsCommand::executeFor(Robot *robot, map<char, string> options){}
}
