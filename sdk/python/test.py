#!/usr/bin/python
# -*- coding: utf8 -*-

import sys, os
import rhoban.communication as com
import sockets.tcp as tcp

filename = '../common/commands.xml'
store = com.CommandsStore()
store.parseXml(filename)

connection = com.Connection('localhost', 12345)
connection.setStore(store)

arguments = connection.ServerEcho([])

print arguments[0]

#while True:
#    message = connection.getMessage()
#    arguments = store.readData(message)

#    print arguments

#    out  = 'MSG: uid={0}, destination={1}, command={2}, length={3} - '.format(
#            message.uid, message.destination, message.command, len(message.data))
#    raw = message.getRaw()

#    for i in xrange(0, len(raw)):
#        out += '%02X ' % ord(raw[i])

#    print out
