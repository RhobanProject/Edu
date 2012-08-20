#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys, os, re, threading, time, getopt
from datetime import datetime

"""
    Commands manager
"""
class CommandsManager(object):
    def __init__(self, fallbackCommand = 'help'):
        self.commands = {}
        self.families = {}
        self.fallbackCommand = fallbackCommand

    def addCommand(self, command, family = 'default'):
        self.commands[command.name] = command
        
        if family not in self.families:
            self.families[family] = {}

        self.families[family][command.name] = command
        command.setManager(self)

    def run(self, argv):
        command = False

        if len(argv) > 0:
            command = argv[0]

        parameters = argv[1:]

        if not command:
            command = self.fallbackCommand

        if command in self.commands:
            command = self.commands[command]

            try:
                options, arguments = getopt.getopt(parameters, command.options)
                options = dict(options)
            except getopt.GetoptError as e:
                print('Command error: %s' % e)
                return

            if len(arguments) < command.arguments:
                print('Command requires at least %d arguments' % command.arguments)
                print('%s %s: %s' % (command.name, command.prototype, command.description))
                return

            try:
                command.run(options, arguments)
            except (Exception, KeyboardInterrupt, SystemExit):
                print('Command aborted')
                command.stop()
                raise

            command.stop()
        else:
            print('Unknown command: %s' % command)

"""
    A command
"""
class Command(object):
    def __init__(self):
        self.name = ''
        self.description = ''
        self.prototype = ''
        self.options = ''
        self.arguments = 0
        self.define()

    def setManager(self, manager):
        self.manager = manager

    def run(self, options, arguments):
        raise NotImplementedError('Command %s not implemented' % self.name)

    def stop(self):
        pass
