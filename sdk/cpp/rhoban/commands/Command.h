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
#include "CommandsManager.h"

using namespace std;

namespace Rhoban
{
  class Commandpy
  {
  public:
    Commandpy();

    void run(vector<string> options, vector<string> arguments); // PROTOYPE ?
    void stop();

    void setManager(CommandsManager *manager);
    CommandsManager *getManager();
    void setName(string name);
    string getName();
    void setDescription(string description);
    string getDescription();
    void setPrototype(string prototype);
    string getPrototype();
    void setOptions(vector<string> options);
    vector<string> getOptions();
    void setArguments(int arguments);
    int getArguments();

  protected:
    CommandsManager *manager;
    string name;
    string description;
    string prototype;
    vector<string> options;
    int arguments;
  };
}

#endif // COMMAND__H
