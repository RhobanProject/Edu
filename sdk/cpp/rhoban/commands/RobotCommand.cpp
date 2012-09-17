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
#include "RobotCommand.h"

using namespace std;

namespace Rhoban
{
  void RobotCommand::run(map<char, string> options, vector<string> arguments)
  {
    if(arguments.empty())
      cout << "You must specify a robot name" << endl;
    else
      {
	string robotName = arguments[0];
	arguments.erase(arguments.begin());

	robotsStore.loadYaml("config.yml");

	if(!robotsStore.getRobots().count(robotName))
	  cout << "Robot " << robotName << " nor found" << endl;
	else
	  execute(robotsStore[robotName], options, arguments);
      }
  }
  
  void RobotCommand::stop()
  {
    robotsStore.stop();
  }
  
  void RobotCommand::execute(Robot *robot, map<char, string> options,
			     vector<string> arguments){}
}
