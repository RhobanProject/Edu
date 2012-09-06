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
#include <vector>
#include <map>
#include "CommandsManager.h"
#include "Command.h"

using namespace std;

namespace Rhoban
{
  Commandpy::Commandpy()
  {
    arguments = 0;
    define();
  }
  
  void Commandpy::run(map<char, string> options, vector<string> arguments)
  {
    cout << "Command " << name << " nor implemented" << endl;
  }
  
  void Commandpy::stop(){}
  
  void Commandpy::define(){}

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

  void Commandpy::setOptions(string options)
  {
    this->options = options;
  }

  string Commandpy::getOptions()
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
