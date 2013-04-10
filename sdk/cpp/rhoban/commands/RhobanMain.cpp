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
#include "HelpCommand.h"
#include "Commands.h"
#include "CommandsManager.h"
#include "RhobanMain.h"

using namespace std;

namespace Rhoban
{
  RhobanMain::RhobanMain(int argc, char *argv[])
  {
    this->argc = argc-1;
    this->argv = argv+1;
    
    manager.addCommand(new HelpCommand, "basic");
    manager.addCommand(new StatusCommand, "basic");
    manager.addCommand(new EmergencyCommand, "basic");
	
    manager.addCommand(new LoadMoveCommand, "moves");
    manager.addCommand(new KillMoveCommand, "moves");
    manager.addCommand(new StartMoveCommand, "moves");
    manager.addCommand(new StopMoveCommand, "moves");
    manager.addCommand(new UpdateConstantCommand, "moves");
    
    manager.addCommand(new CompliantCommand, "motors");
    manager.addCommand(new HardCommand, "motors");
    manager.addCommand(new SetCommand, "motors");
    manager.addCommand(new ScanCommand, "motors");
    manager.addCommand(new SnapshotCommand, "motors");
    manager.addCommand(new InitCommand, "motors");
    manager.addCommand(new ZeroCommand, "motors");
    manager.addCommand(new MonitorCommand, "motors");
    
    manager.addCommand(new SensorsCommand, "sensors"); 
    
    manager.addCommand(new VisionCommand, "vision"); 
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
