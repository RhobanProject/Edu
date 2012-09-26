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
#include "Command.h"
#include "HelpCommand.h"

using namespace std;

namespace Rhoban
{
  HelpCommand::HelpCommand()
  {
    name = "help";
    description = "Displays the available commands";
    prototype = "[family]";
  }

  void HelpCommand::run(map<char, string> options, vector<string> arguments)
  {
    cout << "Rhoban System - http://edu.rhoban-system.fr/sdk/commandes.html"
	 << endl;
    cout << "Available commands :" << endl;
    
    map<string, map<string, Commandpy *> > families = manager->getFamilies();
    
    map<string, map<string, Commandpy *> >::iterator it;
    map<string, Commandpy *>::iterator it2;
    
    for(it=families.begin(); it!=families.end(); ++it)
      {
	if(arguments.empty() || is_in_vector_string(arguments, it->first))
	  {
	    cout << endl << it->first << " :" << endl;
	    for(it2=it->second.begin(); it2!=it->second.end(); ++it2)
	      {
		cout << "  " << it2->first << " " 
		     << it2->second->getPrototype() << " : " 
		     << it2->second->getDescription() << endl;
	      }
	  }
      }
  }
}
