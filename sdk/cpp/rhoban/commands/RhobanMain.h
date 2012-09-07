/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
#ifndef RHOBANMAIN_H
#define RHOBANMAIN_H

#include <cstdlib>
#include <cstdio>
#include "CommandsManager.h"

using namespace std;

namespace Rhoban
{
  class RhobanMain
  {
  public:
    RhobanMain(int argc, char *argv[]);
    
    void run();
    
    void setManager(CommandsManager manager);
    CommandsManager getManager();
    void setArgc(int argc);
    int getArgc();
    void setArgv(char **argv);
    char **getArgv();

  protected:
    CommandsManager manager;
    int argc;
    char **argv;
  };
}

#endif // RHOBANMAIN_H
