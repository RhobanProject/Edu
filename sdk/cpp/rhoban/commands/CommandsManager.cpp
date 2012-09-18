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
#include <string>
#include <map>
#include <vector>
#include <util.h>
#include "CommandsManager.h"
#include "Command.h"

using namespace std;

namespace Rhoban
{
  CommandsManager::CommandsManager(string fallbackCommand)
  {
    this->fallbackCommand = fallbackCommand;
  }

  void CommandsManager::addCommand(Commandpy *command, string family)
  {
    commands[command->getName()] = command;
    families[family][command->getName()] = command;
    command->setManager(this);
  }

  void CommandsManager::run(int argc, char *argv[])
  {
    string command = fallbackCommand;
    if(argc>0)
      command = argv[0];

    if(commands.count(command))
      {
	map<char, string> options;
	vector<string> arguments;
	int opt;
	while((opt = getopt(argc, argv, 
			    commands[command]->getOptions().c_str())) != -1)
	  {
	    switch(opt)
	      {
		if(opt == '?' || opt == ':')
		  {
		  cout << "Command error" << endl;
		  return;
		  }
		else
		  {
		    if(optarg)
		      options[opt] = optarg;
		    else
		      options[opt];
		  }
	      }
	  }
	
	for(int i=optind; i<argc; ++i)
	  arguments.push_back(argv[i]);	

	if(arguments.size() < commands[command]->getArgumentsLength())
	  {
	    cout << "Command requires at least " 
		 << commands[command]->getArgumentsLength() 
		 << " arguments" << endl;
	    cout << commands[command]->getName() << " "
		 << commands[command]->getPrototype() << " : "
		 << commands[command]->getDescription() << endl;
	  }
	else
	  {
	    commands[command]->run(options, arguments);
	    commands[command]->stop();
	  }
      }
    else
      cout << "Unknown command : " << command << endl;
  }

  void CommandsManager::setCommands(map<string, Commandpy *> commands)
  {
    this->commands = commands;
  }

  map<string, Commandpy *> CommandsManager::getCommands()
  {
    return commands;
  }

  
  void CommandsManager::setFamilies(map<string, map<string, Commandpy *> > families)
  {
    this->families = families;
  }

  
  map<string, map<string, Commandpy *> > CommandsManager::getFamilies()
  {
    return families;
  }

  void CommandsManager::setFallbackCommand(string fallbackCommand)
  {
    this->fallbackCommand = fallbackCommand;
  }

  string CommandsManager::getFallbackCommand()
  {
    return fallbackCommand;
  }
}
