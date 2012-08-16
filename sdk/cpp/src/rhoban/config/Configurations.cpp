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
#include <string>
#include <communication/Connection.h>
#include "Configurations.h"

using namespace std;

namespace Rhoban
{

  Configurations::Configurations()
  {

  }

  Configurations::~Configurations()
  {

  }

  void Configurations::load(string fileName)
  {

  }

  Connection *Configurations::getConnection()
  {
    return(new Connection);
  }

  void Configurations::setConnection(Connection *connection)
  {

  }
}
