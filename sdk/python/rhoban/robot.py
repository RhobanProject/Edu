#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys, os, re, threading, time
import configurations as config
import communication as com

class Robot(object):
    def __init__(self):
        self.store = com.CommandsStore()
        self.store.parseXml('../common/commands.xml')

        self.connection = com.Connection()
        self.connection.setStore(self.store)

        self.configs = config.Configurations(self.connection)

    def connect(self, hostname = 'localhost', port = 12345):
        self.connection.connectTo(hostname, port)

    def testConnection(self):
        print 'Testing server version...'
        response = self.connection.ServerGetVersion_response()
        print('~> %s' % response[0])

        print 'Testing echo "Hello world"...'
        response = self.connection.ServerEcho_response('Hello world')
        print('~> %s' % response[0])

        if response[0] == 'Hello world':
            print "\n"+'Connection test passed'

    def stop(self):
        self.connection.stop()

    def initialize(self):
        self.connection.ServosInit()

    def moveMotor(self, motorId, angle):
        print self.connection.ServosSetValues_response(1, [motorId], [angle], [1023], [1023])
    
    def compliant(self, motorId):
        print self.connection.ServosSetValues_response(1, [motorId], [0], [0], [0])

    def allCompliant(self):
        self.connection.ServosAllCompliant()
