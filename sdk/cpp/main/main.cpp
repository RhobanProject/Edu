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

#include <main/Command.h>
#include <commands/RhobanMain.h>

using namespace std;
using namespace Rhoban;

void initiate();
void quit();
void usage();
void help();
void sigint(int signal);

/**
 * Permet d'apeller dxl_terminale en cas de CTRL+C
 */
void sigint(int signal)
{
    quit();
    exit(EXIT_SUCCESS);
}

/**
 * Initialise la connexion avec USB2Dynamixel
 */
void initiate()
{
//    motor_start_dispatching();
}

/**
 * Termine la connexion avec USB2Dynamixel
 */
void quit()
{
//    motor_stop_dispatching();
}

/**
 * Affiche l'utilisation et l'aide
 */
void usage(char *progname)
{
    cerr << "Usage: " << progname << " <command>\n" << endl;

    help();
}

/**
 * Displays help
 */
void help()
{
    Command::help();
}

COMMAND_DEFINE(help, "Displays help about available commands")
{
    help();
}

/**
 * Initializes and try to execute the given command
 */
int oldmain(int argc, char *argv[])
{
    initiate();

    signal(SIGINT, sigint);

    if (argc != 2) {
        usage(argv[0]);
        return EXIT_FAILURE;
    } else {
        try {
            Command::resolve(argc - 1, argv + 1);
        } catch (string exception) {
            cerr << "Error: " << exception << endl;
        }
    }

    quit();

    return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{
        try {
            RhobanMain main(argc, argv);
            main.run();
        } catch (string str) {
            cout << "Error: " << str << endl;
        }
	return EXIT_SUCCESS;
}
