#ifndef _WALKING_H
#define _WALKING_H

#include <rhoban/robot/Robots.h>
#include <configfile/ConfigFile.h>

void loadConfig(ConfigFile &config);
void setup(); 
void tick(Robot &robot);
void zero(Robot &robot);

#endif // _WALKING_H
