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
  
  COMMAND_DEFINE(testyaml, "Run the Yaml parsing test")
  {
    cout << "Starting test on yaml parsing..." << endl;

    string filename="../rhoban/robot/test/config.yml";

    string name, host, port, environment;
    vector<string> moves;
    
    ifstream cfgfile(filename.c_str());
    YAML::Parser parser(cfgfile);
    YAML::Node doc;
    parser.GetNextDocument(doc);
    
    if(!doc.FindValue("robots"))
      cout << "Config error : no \"robots\" entry" << endl;
    else
      {
	YAML::Iterator it;
	for(it=doc["robots"].begin();it!=doc["robots"].end();++it) 
	  {
	    it.first() >> name;
	    cout << "robots[\"" << name << "\"] = new Robot(new CommandsStore);" << endl;
	    
	  }
      }
    
    cout << "####################################################" << endl;
    cout << "####################################################" << endl;
    cout << "####################################################" << endl;
    cout << doc.size() << endl;

    string sortie;
    doc["robots"]["spider"]["environment"] >> sortie;
    cout << sortie << endl;
    
    cout << doc["robots"].size() << endl;

    doc["robots"]["humanoid"]["environment"] >> sortie;
    cout << sortie << endl;

    int i;
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
	
	it.first() >> name;
	it.second()["host"] >> host;
	//it.second()["port"] >> port;
	it.second()["environment"] >> environment;
	
	cout << "== " << name << " " << host <<" " << port <<" " << environment << endl;

	cout << "size " << it.second().size() << endl;

	cout << "boucle interne" << endl;
	for(YAML::Iterator it2=it.second().begin();it2!=it.second().end();++it2)
	  {
	    cout << "a" << endl;
	    it2.first() >> sortie;
	    cout << sortie << endl;
	  }
	cout << "sortie boucle interne" << endl;
	
	cout << "TEST EXISTENCE PORT " << it.second().FindValue("port") << endl;
      }
    cout << doc.FindValue("commands") << endl;
  }
}
