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
#include <string>
#include <map>
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

  void CommandsManager::run(char *argv[])
  {

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
