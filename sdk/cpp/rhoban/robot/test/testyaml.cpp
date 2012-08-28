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
#include <string>
#include <vector>
#include <yaml-cpp/yaml.h>
#include <main/Command.h>
#include <rhoban/robot/Robot.h>

using namespace std;

namespace Rhoban
{
  struct robottest
  {
    string name, host, port, environment;
    vector<string> moves;
  };
  
  COMMAND_DEFINE(testyaml, "Run the Yaml parsing test")
  {
    cout << "Starting test on yaml parsing..." << endl;

    int i;
    robottest robot;
    string filename="../rhoban/robot/test/config.yml";

    string name, host, port, environment;
    vector<string> moves;
    
    ifstream cfgfile(filename.c_str());
    YAML::Parser parser(cfgfile);
    YAML::Node doc;
    parser.GetNextDocument(doc);
    
    cout << doc.size() << endl;

    string sortie;
    doc["robots"]["spider"]["environment"] >> sortie;
    cout << sortie << endl;
    
    cout << doc["robots"].size() << endl;

    doc["robots"]["humanoid"]["environment"] >> sortie;
    cout << sortie << endl;

    for(i=0; i<doc["robots"].size(); i++)
      {
	cout << i << endl;
	
	
	//doc["robots"][i]["environment"] >> sortie;
	//cout << sortie << endl;
	
      }
    
    cout << endl << endl << endl;
    
    for(YAML::Iterator it=doc["robots"].begin();it!=doc["robots"].end();++it) 
      {
	it.first() >> sortie;
	cout << sortie << endl;
	
	it.first() >> robot.name;
	it.second()["host"] >> robot.host;
	//it.second()["port"] >> robot.port;
	it.second()["environment"] >> robot.environment;
	
	cout << "== " << robot.name << " " << robot.host <<" " << robot.port <<" " << robot.environment << endl;
      }
    
  }
}
