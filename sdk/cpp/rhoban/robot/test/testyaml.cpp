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
#include <util.h>

using namespace std;

namespace Rhoban
{
  
  COMMAND_DEFINE(testyaml, "Run the Yaml parsing test")
  {
    cout << "Starting test on yaml parsing..." << endl << endl;

    string filename="../rhoban/robot/test/config.yml";

    ifstream cfgfile(filename.c_str());
    YAML::Parser parser(cfgfile);
    YAML::Node doc;
    parser.GetNextDocument(doc);
    
    if(!doc.FindValue("robots"))
      cout << "Config error : no \"robots\" entry" << endl;
    else
      {
	YAML::Iterator it;
	int i;

	string name, host, environment, move;
	int port;
	vector<string> loadedMoves;
	
	for(it=doc["robots"].begin();it!=doc["robots"].end();++it) 
	  {
	    loadedMoves.clear();
	    loadedMoves.push_back("Fly");
	    
	    // Name
	    it.first() >> name;
	    cout << "robots[\"" << name << "\"] = new Robot(new CommandsStore);" << endl;
	    
	    // Host & port
	    if(it.second().FindValue("host"))
	      {
		it.second()["host"] >> host;
		
		if(it.second().FindValue("port"))
		  it.second()["port"] >> port;		  
		else
		  port = 12345;
		
		cout << "robots[\"" << name << "\"]->connect(" << host << ", " 
		     << port << ");" << endl;
	      }
	    
	    // Environment
	    if(it.second().FindValue("environment"))
	      {
		it.second()["environment"] >> environment;
		
		cout << "robots[\"" << name << "\"]->loadEnvironment(" 
		     << environment << ");" << endl;
	      }

	    // LoadMoves
	    if(it.second().FindValue("loadMoves"))
	      {
		cout << "loadedMoves = robots[\"" << name 
		     << "\"]->getLoadedMoves();" << endl;
		
		for(i=0; i<it.second()["loadMoves"].size(); ++i)
		  { 
		    it.second()["loadMoves"][i] >> move;
		    if(!is_in_vector_string(loadedMoves, move))
		      {
			loadedMoves.push_back(move);
			cout << "robots[\"" << name << "\"]->loadMove(" 
			     << move << ");" << endl;
		      }
		  }
	      }   
	    cout << endl;
	  }
      }
  }
}
