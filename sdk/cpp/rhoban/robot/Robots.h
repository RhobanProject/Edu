/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
#ifndef ROBOTS_H_
#define ROBOTS_H_

#include <cstdlib>
#include <cstdio>
#include <string>
#include <map>
#include "Robot.h"

using namespace std;

namespace Rhoban
{
  class Robots
  {
  public:
    Robots();

    size_t len();
    Robot *operator[](string name);
    
    void loadYaml(string filename);
    void stop();
    
    void setRobots(map<string, Robot *> robots);
    map<string, Robot *> getRobots();
  protected:
    map<string, Robot *> robots;
  };
}

#endif // ROBOTS_H_
