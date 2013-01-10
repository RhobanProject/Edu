#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys, os
import rhoban.communication as com
from rhoban.generate import generate_message_builder, generate_connection

if len(sys.argv) != 2:
    print('Usage: generate.py <Output directory>')
    exit()
    

filename = os.path.dirname(__file__) + '/../common/commands.xml'
store = com.CommandsStore()
store.parseXml(filename)

generate_message_builder(store, sys.argv[1])
generate_connection(store, sys.argv[1])
