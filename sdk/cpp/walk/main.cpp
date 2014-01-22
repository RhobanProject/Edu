/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
#include <signal.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <main/Command.h>
#include <timing/sleep.h>
#include <ticks.h>
#include <configfile/ConfigFile.h>
#include "walking.h"

#include <rhoban/robot/Robots.h>

using namespace std;
using namespace Rhoban;

Robot *robot;

void stop(int x)
{
    zero(*robot);
    ms_sleep(50);
    exit(0);
}

int main(int argc, char **argv)
{
    signal(SIGINT, stop);

    ConfigFile walk("walk.yml");
    walk.useCommandArgs(argc, argv);
    loadConfig(walk);
    walk.help();

    try
    {
        Robots robots;
        setup();

        cout << "Starting " << endl;

        // Charge la configuration et connecte les robots
        robots.loadYaml("config.yml");
        robot = robots["django"];
        Motors *motors = robot->getMotors();

        cout << "Starting motors " << endl;
        motors->start(100);
        ms_sleep(100);

        // motors->get("Coude D")->setAngle(-10);

        while (true) {
            tick(*robot);
            ms_sleep(10);
        }

        ms_sleep(25);
        cout << "Bye bye" << endl;
        robots.stop();
    }
    catch(string exc)
    {
        cout << "Received exception " << exc << endl;
        exit(0);
    }

}
