/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
#ifndef ROBOTCOMMAND_H
#define ROBOTCOMMAND_H

#include <cstdlib>
#include <cstdio>
#include <vector>
#include <map>
#include <string>
#include <robot/Robots.h>
#include <robot/Robot.h>
#include "Command.h"

using namespace std;

namespace Rhoban
{
  // Commande abstraite qui s'applique sur un seul robot
  class RobotCommand : public Commandpy
  {
  public:
    void run(map<char, string> options, vector<string> arguments);
    virtual void execute(Robot *robot, map<char, string> options, 
		 vector<string> arguments);
    void stop();
  protected:
    Robots robotsStore;
  };
}

#endif // ROBOTCOMMAND_H
