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
#include <vector>
#include "CommandsManager.h"
#include "Command.h"

using namespace std;

namespace Rhoban
{
  Commandpy::Commandpy()
  {

  }
  
  void Commandpy::run(vector<string> options, vector<string> arguments)
  {

  }
  
  void Commandpy::stop()
  {

  }

  void Commandpy::setManager(CommandsManager *manager)
  {
    this->manager = manager;
  }

  CommandsManager *Commandpy::getManager()
  {
    return manager;
  }

  void Commandpy::setName(string name)
  {
    this->name = name;
  }

  string Commandpy::getName()
  {
    return name;
  }

  void Commandpy::setDescription(string description)
  {
    this->description = description;
  }

  string Commandpy::getDescription()
  {
    return description;
  }

  void Commandpy::setPrototype(string prototype)
  {
    this->prototype = prototype;
  }

  string Commandpy::getPrototype()
  {
    return prototype;
  }

  void Commandpy::setOptions(vector<string> options)
  {
    this->options = options;
  }

  vector<string> Commandpy::getOptions()
  {
    return options;
  }

  void Commandpy::setArguments(int arguments)
  {
    this->arguments = arguments;
  }

  int Commandpy::getArguments()
  {
    return arguments;
  }
}
