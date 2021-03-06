#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys, os, re, threading, time
from xml.dom import minidom
import rhoban.communication as com

class Config(object):
    def __init__(self, filename):
        self.config = open(filename, 'rb').read()
        self.parse()

    def getText(self, node, name):
        text = ''
        nodes = node.getElementsByTagName(name)

        if nodes.length == 1:
            for child in nodes[0].childNodes:
                text += child.data

        return text

    def parse(self):
        pass

class ServoConfig(object):
    def __init__(self, id, iniAngle, zeroAngle):
        self.id = id
        self.iniAngle = iniAngle
        self.zeroAngle = zeroAngle

class MoveSchedulerConfig(Config):
    def parse(self):
        self.servos = {}
        dom = minidom.parseString(self.config)

        for servos in dom.getElementsByTagName('ServosConfig'):
            self.processServos(servos)

    def processServos(self, servos):
        for servo in servos.getElementsByTagName('ServoConfig'):
            newServo = ServoConfig(self.getText(servo, 'Id'), self.getText(servo, 'IniAngle'), self.getText(servo, 'ZeroAngle'))
            self.servos[self.getText(servo, 'Name')] = newServo

class LowLevelConfig(Config):
    pass

class Configurations(object):
    def __init__(self, connection):
        self.connection = connection
        self.moveSchedulerConfig = None
        self.lowLevelConfig = None

    def isMoveSchedulerConfigLoaded(self):
        response = None

        while response == None:
            response = self.connection.SchedulerConfigIsLoaded_response()
        return response[0] == 1

    def loadMoveSchedulerConfig(self, config, force = False):
        self.moveSchedulerConfig = MoveSchedulerConfig(config)

        if force or not self.isMoveSchedulerConfigLoaded():
            self.connection.SchedulerLoadConfig(self.moveSchedulerConfig.config)
            self.connection.ServosScan(250, b'Normal')

    def isLowLevelConfigLoaded(self):
        response = None

        while response == None:
            response = self.connection.LowLevelConfigIsLoaded_response()
        return response[0] == 1

    def loadLowLevelConfig(self, config, force = False):
        self.lowLevelConfig = LowLevelConfig(config)

        if force or not self.isLowLevelConfigLoaded():
            self.connection.LowLevelLoadConfig(self.lowLevelConfig.config)

