#!/usr/bin/python
# -*- coding: utf8 -*-

import sys, os
from rhoban.generate import generate_message_builder

if len(sys.argv) != 3:
    print 'Usage: generate.py <MessageBuilder.h> <MessageBuilder.cpp>'
    exit()

filename = '../common/commands.xml'
generate_message_builder(filename, sys.argv[1], sys.argv[2])
