/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
#ifndef CONFIGURATIONS_H_
#define CONFIGURATIONS_H_

#include <cstdlib>
#include <cstdio>
#include <string>
#include <communication/Connection.h>

using namespace std;

namespace Rhoban
{
  class Configurations
  {
  public:
    Configurations();
    ~Configurations();
    void load(string fileName);
    Connection *getConnection();
    void setConnection(Connection *connection);
  protected:
    Connection *connection;
  };
}

#endif // CONFIGURATIONS_H_