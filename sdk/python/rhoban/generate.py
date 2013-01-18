#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys, os, codecs
import rhoban.communication as com

def template_tag(filename, tag, contents, out_dir):
    template = os.path.join(os.path.dirname(__file__), 'templates/', filename)
    template_contents = open(template, 'r', encoding='utf-8').read()

    contents = template_contents.replace(tag, contents)
    
    outFile = codecs.open(os.path.join(out_dir, filename), 'w', 'utf-8')
    outFile.write(contents)
    outFile.close()

def generate_method(specification):
    method = ''

    method += "        Message *message = new Message;\n"
    method += "        message->setUid(getUid());\n"
    method += "        message->setDestination("+str(specification.destination)+");\n"
    method += "        message->setCommand("+str(specification.command)+");\n"


    argumentNumber = 1

    for argument in specification.parametersPattern:
        method += "        message->append(arg{0});\n".format(argumentNumber)
        argumentNumber += 1

    method += "\n        return message;";

    return method

"""
    Generates the MessageBuilder class
"""
def generate_message_builder(store, out_dir):
    prototypes = ''
    methods = ''

    for name in store:
        if not name:
            continue

        specification = store.get(name)
        prototypes += '            Message *{0}('.format(name)
        methods += '    Message *MessageBuilder::{0}('.format(name)
        arguments = []
        argumentNumber = 1

        for argument in specification.parametersPattern:
            arguments += [str(argument.cpp()) + 'arg' + str(argumentNumber)]
            argumentNumber += 1

        prototypes += ', '.join(arguments)
        methods += ', '.join(arguments)

        prototypes += ");\n"
        methods += ")\n    {\n"
        methods += generate_method(specification)
        methods += "\n    }\n\n"

    template_tag('MessageBuilder.h', '<METHODS_PROTOTYPES>', prototypes, out_dir)
    template_tag('MessageBuilder.cpp', '<METHODS>', methods, out_dir)

"""
    Generates the connection class
"""
def generate_connection(store, out_dir):
    prototypes = ''
    methods = ''

    for name in store:
        if not name:
            continue

        specification = store.get(name)
        argumentNumber = 1
        arguments = []
        names = []

        for argument in specification.parametersPattern:
            arguments += [argument.cpp() + 'arg' + str(argumentNumber)]
            names += ['arg' + str(argumentNumber)]
            argumentNumber += 1

        allArgumentNames = ', '.join(names)
        allArguments = ', '.join(arguments)
        allArgumentsCallback = ', '.join(arguments + ['sendCallback callback, void *data'])
        allArgumentsTimeout = ', '.join(arguments + ['ui32 timeout'])
        allArgumentsTimeoutProt = ', '.join(arguments + ['ui32 timeout=1000'])


        # No answer
        prototypes += "                void {0}({1});\n".format(name, allArguments)
        methods += "           void Connection::{0}({1})\n".format(name, allArguments);
        methods += "           {\n"
        methods += "                   Message *message = commandsStore->getBuilder()->{0}({1});\n".format(name, allArgumentNames)
        methods += "                   sendMessage(message);\n"
        methods += "                   delete message;\n"
        methods += "           }\n\n"

        # Direct answer
        prototypes += "                Message *{0}_response({1});\n".format(name, allArgumentsTimeoutProt)
        methods += "           Message *Connection::{0}_response({1})\n".format(name, allArgumentsTimeout)
        methods += "           {\n"
        methods += "                   Message *message = commandsStore->getBuilder()->{0}({1});\n".format(name, allArgumentNames)
        methods += "                   Message * answer = sendMessageReceive(message, timeout);\n"
        methods += "                   delete message;\n"
        methods += "                   return answer;\n"
        methods += "           }\n\n"

        # Callback answer
        prototypes += "                void {0}_callback({1}=NULL);\n".format(name, allArgumentsCallback)
        methods += "           void Connection::{0}_callback({1})\n".format(name, allArgumentsCallback);
        methods += "           {\n"
        methods += "                   Message *message = commandsStore->getBuilder()->{0}({1});\n".format(name, allArgumentNames)
        methods += "                   sendMessageCallback(message, callback, data);\n"
        methods += "                   delete message;\n"
        methods += "           }\n\n"

    template_tag('Connection.h', '<METHODS_PROTOTYPES>', prototypes, out_dir)
    template_tag('Connection.cpp', '<METHODS>', methods, out_dir)

