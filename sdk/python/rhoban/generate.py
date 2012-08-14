#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys, os, codecs
import rhoban.communication as com

def template_tag(filename, tag, contents, out_dir):
    template = os.path.join(os.path.dirname(__file__), 'templates/', filename)
    template_contents = unicode(open(template, 'r').read(), 'utf8')

    contents = template_contents.replace(tag, contents)
    
    outFile = codecs.open(os.path.join(out_dir, filename), 'w', 'utf-8')
    outFile.write(contents)
    outFile.close()

def generate_method(specification):
    method = u''

    method += "        Message *message = new Message;\n"
    method += "        message->setUid(getUid());\n"
    method += "        message->setDestination("+str(specification.destination)+");\n"
    method += "        message->setCommand("+str(specification.command)+");\n"


    argumentNumber = 1

    for argument in specification.parametersPattern:
        method += u"        message->append(arg{0});\n".format(argumentNumber)
        argumentNumber += 1

    method += u"\n        return message;";

    return method

"""
    Generates the MessageBuilder class
"""
def generate_message_builder(store, out_dir):
    prototypes = u''
    methods = u''

    for name in store:
        if not name:
            continue

        specification = store.get(name)
        prototypes += u'            Message *{0}('.format(name)
        methods += u'    Message *MessageBuilder::{0}('.format(name)
        arguments = []
        argumentNumber = 1

        for argument in specification.parametersPattern:
            arguments += [unicode(argument.cpp() + 'arg' + unicode(argumentNumber))]
            argumentNumber += 1

        prototypes += u', '.join(arguments)
        methods += u', '.join(arguments)

        prototypes += u");\n"
        methods += u")\n    {\n"
        methods += generate_method(specification)
        methods += "\n    }\n\n"

    template_tag('MessageBuilder.h', '<METHODS_PROTOTYPES>', prototypes, out_dir)
    template_tag('MessageBuilder.cpp', '<METHODS>', methods, out_dir)

"""
    Generates the connection class
"""
def generate_connection(store, out_dir):
    prototypes = u''
    methods = u''

    for name in store:
        if not name:
            continue

        specification = store.get(name)
        argumentNumber = 1
        arguments = []
        names = []

        for argument in specification.parametersPattern:
            arguments += [unicode(argument.cpp() + 'arg' + unicode(argumentNumber))]
            names += ['arg' + unicode(argumentNumber)]
            argumentNumber += 1

        allArgumentNames = u', '.join(names)
        allArguments = u', '.join(arguments)
        allArgumentsCallback = u', '.join(arguments + ['sendCallback callback'])

        # No answer
        prototypes += u"                void {0}({1});\n".format(name, allArguments)
        methods += u"           void Connection::{0}({1})\n".format(name, allArguments);
        methods += u"           {\n"
        methods += u"                   Message *message = commandsStore->getBuilder()->{0}({1});\n".format(name, allArgumentNames)
        methods += u"                   sendMessage(message);\n"
        methods += u"           }\n\n"

        # Direct answer
        prototypes += u"                Message *{0}_response({1});\n".format(name, allArguments)
        methods += u"           Message *Connection::{0}_response({1})\n".format(name, allArguments);
        methods += u"           {\n"
        methods += u"                   Message *message = commandsStore->getBuilder()->{0}({1});\n".format(name, allArgumentNames)
        methods += u"                   return sendMessageRecieve(message);\n"
        methods += u"           }\n\n"

        # Callback answer
        prototypes += u"                void {0}_callback({1});\n".format(name, allArgumentsCallback)
        methods += u"           void Connection::{0}_callback({1})\n".format(name, allArgumentsCallback);
        methods += u"           {\n"
        methods += u"                   Message *message = commandsStore->getBuilder()->{0}({1});\n".format(name, allArgumentNames)
        methods += u"                   sendMessageCallback(message, callback);\n"
        methods += u"           }\n\n"

    template_tag('Connection.h', '<METHODS_PROTOTYPES>', prototypes, out_dir)
    template_tag('Connection.cpp', '<METHODS>', methods, out_dir)

