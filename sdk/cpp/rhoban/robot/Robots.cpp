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
#include <fstream>
#include <iostream>
#include <yaml-cpp/yaml.h>
#include "Robots.h"

using namespace std;

namespace Rhoban
{
  Robots::Robots(){}

  size_t Robots::len()
  {
    return robots.size();
  }

  Robot *Robots::operator[](string name)
  {
    return robots[name];
  }

  void Robots::loadYaml(string filename) //////////////
  {
    ifstream file(filename.c_str());
    YAML::Parser parser(file);
    YAML::Node doc;
    parser.GetNextDocument(doc);
    for(int i=0; i<doc.size(); i++)
      {
	/////////////////////
      }
    /////////////////////:

  }

  void Robots::stop()
  {
    map<string, Robot *>::iterator it;
    for(it = robots.begin(); it != robots.end(); ++it)
      it->second->stop();
  }
  
  void Robots::setRobots(map<string, Robot *> robots)
  {
    this->robots = robots;
  }

  map<string, Robot *> Robots::getRobots()
  {
    return robots;
  }
}
