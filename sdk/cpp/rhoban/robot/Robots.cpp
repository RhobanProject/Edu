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
    string name, host, port, environment;
    vector<string> moves;
    
    ifstream cfgfile(filename.c_str());
    YAML::Parser parser(cfgfile);
    YAML::Node doc;
    parser.GetNextDocument(doc);

    for(int i=0; i<doc.size(); i++)
      {
	Robot *robot;
	// cfg le robot en question

	//robots[
      }
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
