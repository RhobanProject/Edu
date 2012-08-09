#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys, os
from rhoban.generate import generate_message_builder

if len(sys.argv) != 3:
    print 'Usage: generate.py <MessageBuilder.h> <MessageBuilder.cpp>'
    exit()

filename = os.path.dirname(__file__) + '/../common/commands.xml'
generate_message_builder(filename, sys.argv[1], sys.argv[2])
