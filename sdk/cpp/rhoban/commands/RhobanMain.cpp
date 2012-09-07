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
#include "CommandsManager.h"
#include "RhobanMain.h"

using namespace std;

namespace Rhoban
{
  RhobanMain::RhobanMain(int argc, char *argv[])
  {
    this->argc = argc-1;
    this->argv = argv+1;
    
    ///////////////////: COMMANDS
    
  }

  void RhobanMain::run()
  {
    manager.run(argc, argv);
  }

  void RhobanMain::setManager(CommandsManager manager)
  {
    this->manager = manager;
  }

  CommandsManager RhobanMain::getManager()
  {
    return manager;
  }

  void RhobanMain::setArgc(int argc)
  {
    this->argc = argc;
  }

  int RhobanMain::getArgc()
  {
    return argc;
  }

  void RhobanMain::setArgv(char **argv)
  {
    this->argv = argv;
  }
  
  char **RhobanMain::getArgv()
  {
    return argv;
  }    
}
