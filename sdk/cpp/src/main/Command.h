#ifndef _COMMAND_H
#define _COMMAND_H

#include <vector>
#include <cstdlib>
#include <cstdio>
#include <string>

using namespace std;

typedef void commandFunction(int argc, char *argv[]);

namespace Rhoban
{
    class Command
    {
        public:
            string name;
            string description;

            Command(string name, string description, commandFunction *_function = NULL);
            
            virtual void execute(int argc, char *argv[]);

            static vector<Command*> *commands;
            static void registerCommand(Command *command);
            static void help();
            static void resolve(int argc, char *argv[]);

        protected:
            commandFunction *function;
    };
}

#define COMMAND_REGISTER_METHOD(_function, _name, _description) \
    __attribute__((constructor)) \
    void _register_command_ ## _name () { \
        Rhoban::Command::registerCommand(new Rhoban::Command(string(#_name), string(_description), _function)); \
    }

#define COMMAND_DEFINE(_name, _description) \
    commandFunction _command_anonymous_ ## _name ; \
    COMMAND_REGISTER_METHOD(_command_anonymous_ ## _name , _name,  _description); \
    void _command_anonymous_ ## _name (int argc, char *argv[]) 

#define COMMAND_REGISTER(_class, _name, _description) \
    __attribute__((constructor)) \
    void _register_command_instance_ ## _name () { \
        Rhoban::Command::registerCommand(new _class (string(#_name), string(_description))); \
    }

#endif // _COMMAND_H
