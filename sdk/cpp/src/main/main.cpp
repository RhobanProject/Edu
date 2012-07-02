#include <signal.h>
#include <iostream>

#include <config.h>
//#include <motor/motor.h>
#include <main/Command.h>

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
int main(int argc, char *argv[])
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
