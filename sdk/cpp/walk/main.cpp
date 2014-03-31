/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <main/Command.h>
#include <timing/sleep.h>
#include <ticks.h>
#include <signal.h>
#include <configfile/ConfigFile.h>
#include "SigmabanWalk.h"

#include <rhoban/robot/Robots.h>

using namespace std;
using namespace Rhoban;

SigmabanWalk walk(8.6, 13.73, 3);

void stop(int x)
{
    walk.zero();
    ms_sleep(50);
    exit(0);
}

int main(int argc, char **argv)
{
    signal(SIGINT, stop);

    ConfigFile config("walk.yml");
    config.useCommandArgs(argc, argv);
    walk.loadConfig(config);
    config.help();

    try {
        Robots robots;

        cout << "Starting " << endl;

        robots.loadYaml("config.yml");
        Robot *robot = robots["local"];
        walk.setRobot(robot);
        Motors *motors = robot->getMotors();
        Sensors *sensors = robot->getSensors();

        cout << "Starting motors " << endl;
        motors->start(100);
        sensors->start(100);
        ms_sleep(100);

        while (true) {
            walk.tick(0.01);
            ms_sleep(10);
        }

        ms_sleep(25);
        cout << "Bye bye" << endl;
        robots.stop();
    } catch(string exc) {
        cout << "Received exception " << exc << endl;
        exit(0);
    }

}
