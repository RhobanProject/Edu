#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys, os, re, struct, threading, time

"""
    Moves manager
"""
class Moves:
    def __init__(self, connection):
        self.connection = connection

    def loadMove(self, filename):
        contents = open(filename, 'rb').read()
        self.connection.SchedulerLoadXMLMove_response(contents)

    def startMove(self, name, duration, smooth = 500):
        self.connection.SchedulerStartMove(name, duration, smooth)

    def pauseMove(self, name):
        self.connection.SchedulerPauseMove(name)

    def stopMove(self, name, smooth = 500):
        self.connection.SchedulerStopMove(name, smooth)

    def killMove(self, name):
        self.connection.SchedulerKillMove(name)

    def getLoadedMoves(self):
        return self.connection.SchedulerGetLoadedMoves_response()[0]

    def updateConstant(self, moveName, constantName, value):
        self.connection.SchedulerUpdateConstant(moveName, constantName, [value])
