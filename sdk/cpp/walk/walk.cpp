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

#include <rhoban/robot/Robots.h>

using namespace std;
using namespace Rhoban;

int main(int argc, char **argv)
{
    try
    {
        Robots robots;

        cout << "Starting " << endl;

        // Charge la configuration et connecte les robots
        robots.loadYaml("config.yml");
        Robot *robot = robots["django"];
        Motors *motors = robot->getMotors();

        cout << "Starting motors " << endl;
        motors->start(30);
        syst_wait_ms(1000);

        while (true) {
            cout << motors->get("Bassin Lat")->getAngle() << endl;
            ms_sleep(100);
        }

        cout << "Bye bye" << endl;
        robots.stop();
    }
    catch(string exc)
    {
        cout << "Received exception " << exc << endl;
        exit(0);
    }

}
