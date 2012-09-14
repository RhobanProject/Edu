/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
#ifndef COMMAND__H
#define COMMAND__H

namespace Rhoban
{
  class CommandsManager;
}

#include <cstdlib>
#include <cstdio>
#include <string>
#include <vector>
#include <map>
#include "CommandsManager.h"

using namespace std;

namespace Rhoban
{
  class Commandpy
  {
  public:
    Commandpy();

    virtual void run(map<char, string> options, vector<string> arguments);
    virtual void stop();

    void setManager(CommandsManager *manager);
    CommandsManager *getManager();
    void setName(string name);
    string getName();
    void setDescription(string description);
    string getDescription();
    void setPrototype(string prototype);
    string getPrototype();
    void setOptions(string options);
    string getOptions();
    void setArgumentsLength(int argumentsLength);
    int getArgumentsLength();

  protected:
    CommandsManager *manager;
    string name;
    string description;
    string prototype;
    string options;
    int argumentsLength;
  };
}

#endif // COMMAND__H
