/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
#ifndef ROBOTSCOMMAND_H
#define ROBOTSCOMMAND_H

#include <cstdlib>
#include <cstdio>
#include <map>
#include <vector>
#include <string>
#include <robot/Robots.h>
#include <robot/Robot.h>
#include "Command.h"

using namespace std;

namespace Rhoban
{
  // Command abstraite qui charge la configuration
  class RobotsCommand : public Commandpy
  {
  public:
    void run(map<char, string> options, vector<string> arguments);
    virtual void executeFor(Robot *robot, map<char, string> options);
    void stop();
    
  protected:
    Robots robotsStore;
  };
}

#endif // ROBOTSCOMMAND_H
