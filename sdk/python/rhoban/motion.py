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
        contents = file(filename, 'r').read()

    def startMove(self, name, duration, smooth = 500):
        self.connection.SchedulerStartMove(name, duration, smooth)

    def pauseMove(self, name):
        self.connection.SchedulerPauseMove(name)

    def stopMove(self, name, smooth = 500):
        self.connection.SchedulerStopMove(name, smooth)

    def killMove(self, name):
        self.connection.SchedulerKillMove(name)
