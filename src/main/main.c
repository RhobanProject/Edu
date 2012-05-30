#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>

#include <config.h>
#include <motor/motor.h>
#include <dynamixel/dynamixel.h>
#include <main/main.h>

static void initiate();
static void terminate();
static void usage();
static void help();
static void sigint(int signal);

static struct main_command *commands = NULL;

void main_register_command(struct main_command *command) 
{
    command->next_command = commands;
    commands = command;
}

/**
 * Permet d'apeller dxl_terminale en cas de CTRL+C
 */
static void sigint(int signal)
{
    terminate();
    exit(EXIT_SUCCESS);
}

/**
 * Initialise la connexion avec USB2Dynamixel
 */
static void initiate()
{
    if (dxl_initialize(DEVICE_INDEX, BAUDRATE_NUM) == 0) {
        fprintf(stderr, "Error while initializing USB2Dynamixel\n");
        exit(EXIT_FAILURE);
    }

    motor_start_dispatching();
}

/**
 * Termine la connexion avec USB2Dynamixel
 */
static void terminate()
{
    motor_stop_dispatching();
    
    dxl_terminate();
}

/**
 * Affiche l'utilisation et l'aide
 */
static void usage(char *progname)
{
    fprintf(stderr, "Usage: %s <commande>\n", progname);

    help();
}

/**
 * Affiche l'aide
 */
static void help()
{
    struct main_command *command;

    fprintf(stderr, "\n");
    fprintf(stderr, "Commandes disponibles :\n");

    for (command = commands; command != NULL; command = command->next_command) {
        fprintf(stderr, "%s\t: %s\n", command->name, command->description);
    }
    
    fprintf(stderr, "\n");
}

REGISTER_COMMAND(help, help, "Affiche l'aide sur les commandes disponibles");

/**
 * Initialise la communication Dynamixel, tente d'éxecuter la commande
 * fournie en premier paramètre et termine
 */
int main(int argc, char *argv[])
{
    initiate();

    signal(SIGINT, sigint);

    if (argc != 2) {
        usage(argv[0]);
        return EXIT_FAILURE;
    } else {
        char found = 0;
        struct main_command *command;

        for (command = commands; command != NULL; command = command->next_command) {
            if (strcmp(command->name, argv[1]) == 0) {
                command->function();
                found = 1;
                break;
            }
        }

        if (!found) {
            fprintf(stderr, "Command inconnue: %s\n", argv[1]);
            fprintf(stderr, "Utilisez la commande \"help\" pour obtenir de l'aide\n");
            return EXIT_FAILURE;
        }
    }

    terminate();

    return EXIT_SUCCESS;
}
