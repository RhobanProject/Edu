/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
#ifndef SENSOR_H
#define SENSOR_H

#include <cstdlib>
#include <cstdio>
#include <vector>
#include <string>
#include <types.h>

using namespace std;

namespace Rhoban
{
  class Sensor
  {
  public:
    Sensor(string name);
    
    ui32 getValue();
    
    void setName(string name);
    string getName();
    void setValues(vector<ui32> values);
    vector<ui32> getValues();

  protected:
    string name;
    vector<ui32> values;
  };
}
 
#endif // SENSOR_H
