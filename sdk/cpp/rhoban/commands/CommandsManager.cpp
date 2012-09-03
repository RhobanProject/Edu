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

    vector<string> parameters;
    for(int i=1; i<argc; ++i)
      parameters[i-1] = argv[i];
    
    if(commands.count(command))
      {
	// getopt -> on recoit options et arguments (vector ?)
	vector<string> arguments;
	vector<string> options;
	// getopt

	if(arguments.size() < commands[command]->getArguments())
	  {
	    cout << "Command requires at least " 
		 << commands[command]->getArguments() 
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
