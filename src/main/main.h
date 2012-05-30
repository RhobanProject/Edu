#ifndef _MAIN_H
#define _MAIN_H

typedef void command_function();

struct main_command {
    char *name;
    char *description;
    command_function *function;
    struct main_command *next_command;
};

void main_register_command(struct main_command *command);

#define REGISTER_COMMAND(_name, _function, _description) \
    __attribute__((constructor)) \
    void _register_command_ ## _name () { \
        struct main_command *command = malloc(sizeof(struct main_command)); \
        command->name = #_name ; \
        command->description = _description ; \
        command->function = (_function); \
        main_register_command(command); \
    }

#endif // _MAIN_H
