#!/usr/bin/python
# -*- coding: utf8 -*-

import sys, os, codecs
import rhoban.communication as com

def template_tag(filename, tag, contents):
    template = os.path.join(os.path.dirname(__file__), 'templates/', filename)
    template_contents = unicode(open(template, 'r').read(), 'utf8')

    return template_contents.replace(tag, contents)

def generate_method(specification):
    method = u''

    method += "        Message *message = new Message(getUid(), {0}, {1})\n".format(specification.destinationIndex,
            specification.index)

    argumentNumber = 1

    for argument in specification.parametersPattern:
        method += u"        message.append(arg{0});\n".format(argumentNumber)
        argumentNumber += 1

    method += u"\n        return message;";

    return method

def generate_message_builder(filename, out_h, out_cpp):
    store = com.CommandStore(filename)
    prototypes = u''
    methods = u''

    for name in store:
        if not name:
            continue

        specification = store.get(name)
        prototypes += u'            Message *{0}('.format(name)
        methods += u'    Message *MessageBuilder::Message *{0}('.format(name)
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

    message_builder_h = template_tag('MessageBuilder.h', '<METHODS_PROTOTYPES>', prototypes)
    message_builder_cpp = template_tag('MessageBuilder.cpp', '<METHODS>', methods)

    h = codecs.open(out_h, 'w', 'utf-8')
    h .write(message_builder_h)
    h.close()

    cpp = codecs.open(out_cpp, 'w', 'utf-8')
    cpp.write(message_builder_cpp)
    cpp.close()

