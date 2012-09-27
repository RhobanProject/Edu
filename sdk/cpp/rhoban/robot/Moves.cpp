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
#include <communication/Connection.h>
#include <file_mngt.h>
#include "Moves.h"

using namespace std;

namespace Rhoban
{
  Moves::Moves(Connection *connection)
  {
    this->connection = connection;
  }

  void Moves::loadMove(string path)
  {
    connection->SchedulerLoadXMLMove_response(file_to_string(path));
  }

  void Moves::startMove(string name, ui32 duration, ui32 smooth)
  {
    connection->SchedulerStartMove(name, duration, smooth);
  }

  void Moves::pauseMove(string name)
  {
    connection->SchedulerPauseMove(name);
  }

  void Moves::stopMove(string name, ui32 smooth)
  {
    connection->SchedulerStopMove(name, smooth);
  }

  void Moves::killMove(string name)
  {
    connection->SchedulerKillMove(name);
  }

  vector<string> Moves::getLoadedMoves()
  {
    return connection->SchedulerGetLoadedMoves_response()->read_string_array();
  }

  void Moves::updateConstant(string moveName, string constantName, float value)
  {
    vector<float> values;
    values.push_back(value);
    connection->SchedulerUpdateConstant(moveName, constantName, values);
  }

  void Moves::setConnection(Connection *connection)
  {
    this->connection = connection;
  }

  Connection *Moves::getConnection()
  {
    return connection;
  }
}
