/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
#ifndef COMMANDS_H
#define COMMANDS_H

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <robot/Robot.h>
#include "RobotCommand.h"
#include "RobotsCommand.h"

using namespace std;

namespace Rhoban
{
  // Commande permettant de visualiser le statut de la connexion
  class StatusCommand : public RobotsCommand
  {
  public:
    StatusCommand();
    void executeFor(Robot *robot, map<char, string> options);
  };

  // Commande de chargement de mouvement
  class LoadMoveCommand : public RobotCommand
  {
  public:
    LoadMoveCommand();
    void execute(Robot *robot, map<char, string> options, 
		 vector<string> arguments);
  };

  // Commande pour tuer un mouvement
  class KillMoveCommand : public RobotCommand
  {
  public:
    KillMoveCommand();
    void execute(Robot *robot, map<char, string> options, 
		 vector<string> arguments);
  };

  // Commande de lancement de mouvement
  class StartMoveCommand : public RobotCommand
  {
  public:
    StartMoveCommand();
    void execute(Robot *robot, map<char, string> options, 
		 vector<string> arguments);
  };

  // Commande de stoppage de mouvement
  class StopMoveCommand : public RobotCommand
  {
  public:
    StopMoveCommand();
    void execute(Robot *robot, map<char, string> options, 
		 vector<string> arguments);
  };

  // Commande de mise à jour d'une constante
  class UpdateConstantCommand : public RobotCommand
  {
  public:
    UpdateConstantCommand();
    void execute(Robot *robot, map<char, string> options, 
		 vector<string> arguments);
  };

  // Relâche tous les moteurs
  class CompliantCommand : public RobotsCommand
  {
  public:
    CompliantCommand();
    void executeFor(Robot *robot, map<char, string> options);
  };

  // Bloque tous les moteurs
  class HardCommand : public RobotsCommand
  {
  public:
    HardCommand();
    void executeFor(Robot *robot, map<char, string> options);
  };

  // Définit la valeur d'un angle
  class SetCommand : public RobotCommand
  {
  public:
    SetCommand();
    void execute(Robot *robot, map<char, string> options, 
		 vector<string> arguments);
  };

  // Fait prendre à un ou plusieurs robots leur positions initiales
  class InitCommand : public RobotsCommand
  {
  public:
    InitCommand();
    void executeFor(Robot *robot, map<char, string> options);
  };

  // Fait prendre à un ou plusieurs robots la position zero
  class ZeroCommand : public RobotsCommand
  {
  public:
    ZeroCommand();
    void executeFor(Robot *robot, map<char, string> options);
  };

  // Lance un scan
  class ScanCommand : public RobotsCommand
  {
  public:
    ScanCommand();
    void executeFor(Robot *robot, map<char, string> options);
  };

  // Arrêt d'urgence
  class EmergencyCommand : public RobotsCommand
  {
  public:
    EmergencyCommand();
    void executeFor(Robot *robot, map<char, string> options);
  };

  // Capture des valeurs des moteurs
  class SnapshotCommand : public RobotsCommand
  {
  public:
    SnapshotCommand();
    void executeFor(Robot *robot, map<char, string> options);
  };

  // Monitorer les moteurs d'un robot
  class MonitorCommand : public RobotCommand
  {
  public:
    MonitorCommand();
    void execute(Robot *robot, map<char, string> options, 
		 vector<string> arguments);
  };

  // Monitorer les capteurs d'un robot
  class SensorsCommand : public RobotCommand
  {
  public:
    SensorsCommand();
    void execute(Robot *robot, map<char, string> options, 
		 vector<string> arguments);
  };  
}

#endif // COMMANDS_H