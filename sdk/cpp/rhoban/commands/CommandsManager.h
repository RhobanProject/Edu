/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
#ifndef COMMANDS_MANAGER_H
#define COMMANDS_MANAGER_H

namespace Rhoban
{
  class Commandpy;
}

#include <cstdlib>
#include <cstdio>
#include <string>
#include <map>
#include "Command.h"

using namespace std;

namespace Rhoban
{
  class CommandsManager
  {
  public:
    CommandsManager(string fallbackCommand = "help");

    void addCommand(Commandpy *command, string family = "default");
    void run(int argc, char *argv[]);

    void setCommands(map<string, Commandpy *> commands);
    map<string, Commandpy *> getCommands();
    void setFamilies(map<string, map<string, Commandpy *> > families);
    map<string, map<string, Commandpy *> > getFamilies();
    void setFallbackCommand(string fallbackCommand);
    string getFallbackCommand();

  protected:
    map<string, Commandpy *> commands;
    map<string, map<string, Commandpy *> > families;
    string fallbackCommand;
  };
}

#endif // COMMANDS_MANAGER_H
