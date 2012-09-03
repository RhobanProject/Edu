/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
#ifndef MOVES_H_
#define MOVES_H_

#include <cstdlib>
#include <cstdio>
#include <string>
#include <communication/Connection.h>

using namespace std;

namespace Rhoban
{
  class Moves
  {
  public:
    Moves(Connection *connection);

    void loadMove(string path);
    void startMove(string name, ui32 duration, ui32 smooth);
    void pauseMove(string name);
    void stopMove(string name, ui32 smooth = 500);
    void killMove(string name);
    vector<string> getLoadedMoves();
    void updateConstant(string moveName, string constantName, vector<float> values);
    
    void setConnection(Connection *connection);
    Connection *getConnection();
 
  protected:
    Connection *connection;
  };
}
#endif // MOVES_H_
