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
#include "CommandsManager.h"

using namespace std;

namespace Rhoban
{
  class Commandpy
  {
  public:
    Commandpy();

    void run(string options, string arguments);

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
    void setArguments(string arguments);
    string getArguments();

  protected:
    CommandsManager *manager;
    string name;
    string description;
    string prototype;
    string options;
    string arguments; ///////////////////////////
  };
}

#endif // COMMAND__H
