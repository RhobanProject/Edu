/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
#ifndef HELPCOMMAND_H
#define HELPCOMMAND_H

#include <cstdlib>
#include <cstdio>
#include <string>
#include <map>
#include <vector>
#include "Command.h"

using namespace std;

namespace Rhoban
{
  // List availables commands
  class HelpCommand : public Commandpy
  {
    void define();
    void run(map<char, string> options, vector<string> arguments);
  };
}

#endif // HELPCOMMAND_H
