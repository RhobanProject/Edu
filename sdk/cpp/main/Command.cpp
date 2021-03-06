/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <vector>

#include "Command.h"

using namespace std;

namespace Rhoban
{
    vector<Command*> *Command::commands = NULL;

    Command::Command(string _name, string _description, commandFunction *_function)
    {
        name = _name;
        description = _description;
        function = _function;
    }

    /**
     * Executes the command
     */
    void Command::execute(int argc, char *argv[])
    {
        function(argc, argv);
    }

    /**
     * Registers a new command
     */
    void Command::registerCommand(Command *command)
    {
        if (commands == NULL) {
            commands = new vector<Command*>;
        }

        commands->push_back(command);
    }

    /**
     * Help
     */
    void Command::help()
    {
        for (vector<Command*>::iterator i = commands->begin(); i != commands->end(); i++) {
            Command *command = *i;

            cerr << command->name << ": " << command->description << endl;
        }
    }

    /**
     * Try to find & execute a command
     */
    void Command::resolve(int argc, char *argv[])
    {
        char found = 0;
        string commandName = string(argv[0]);

        for (vector<Command*>::iterator i = commands->begin(); i != commands->end(); i++) {
            Command *command = *i;

            if (command->name == commandName) {
                command->execute(argc - 1, argv + 1);

                found = 1;
            }
        }

        if (!found) {
            ostringstream stream;
            stream << "Command " << commandName << " not found";

            throw stream.str();
        }
    }
}
