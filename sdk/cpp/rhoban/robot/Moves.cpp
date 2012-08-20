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
#include "Moves.h"

using namespace std;

namespace Rhoban
{
  Moves::Moves(Connection *connection)
  {
    this->connection = connection;
  }
  
  Moves::~Moves()
  {
    delete connection;
  }

  void Moves::loadMove(string path)
  {
    
  }

  void Moves::startMove(string name, int duration, int smooth)
  {

  }

  void Moves::pauseMove(string name)
  {

  }

  void Moves::stopMove(string name, int smooth)
  {

  }

  void Moves::killMove(string name)
  {

  }

  vector<string> Moves::getLoadedMoves()
  {
    vector<string> a;
    return a;
  }

  void Moves::updateConstant(string moveName, string constantName, double value)
  {

  }
}
