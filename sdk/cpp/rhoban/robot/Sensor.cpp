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
#include <types.h>
#include "Sensor.h"
 
using namespace std;

namespace Rhoban
{
  Sensor::Sensor(string name)
  {
    this->name = name;
  }
    
  double Sensor::getValue()
  {
    if(!values.empty())
      return values[0];
  }

  void Sensor::setName(string name)
  {
    this->name = name;
  }

  string Sensor::getName()
  {
    return name;
  }

  void Sensor::setValues(vector<double> values)
  {
    this->values = values;
  }

  vector<double> Sensor::getValues()
  {
    return values;
  }
}
