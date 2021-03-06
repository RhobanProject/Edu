#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys, os, re, threading, time
import yaml
import rhoban.configurations as config
import rhoban.communication as com
import rhoban.motors as motors
import rhoban.motion as motion
import rhoban.sensors as sensors

"""
    Représente un groupe de robots
"""
class Robots(object):
    def __init__(self):
        self.robots = {}

    def loadYaml(self, filename):
        config = yaml.load(open(filename, 'rb').read())
        storeFileName = '../common/commands.xml'

        if 'commands' in config:
            storeFileName = config['commands']

        if 'robots' not in config or not config['robots']:
            raise Exception('Config error: no "robots" entry')

        for robotName, robotConfig in config['robots'].items():
            if not robotConfig.get('enabled', True):
                continue
            
            robot = Robot(storeFileName)
            self.robots[robotName] = robot
            robot.name = robotName
            
            if 'host' in robotConfig:
                robot.connect(robotConfig['host'], robotConfig.get('port', 12345))

            if 'environment' in robotConfig:
                robot.loadEnvironment(robotConfig['environment'])

            if 'loadMoves' in robotConfig:
                loadedMoves = robot.getLoadedMoves()

                for move in robotConfig['loadMoves']:
                    if move not in loadedMoves:
                        loadedMoves.append(move)
                        robot.loadMove(move)

    def stop(self):
        for name, robot in self.robots.items():
            robot.stop()

    def __len__(self):
        return len(self.robots)

    def __iter__(self):
        return iter(self.robots)

    def __getitem__(self, name):
        return self.robots[name]

"""
    Ensemble de fonctions et raccourcis pour communiquer avec un robot
"""
class Robot(object):
    def __init__(self, storeFileName = '../common/commands.xml'):
        self.store = com.CommandsStore()
        self.store.parseXml(storeFileName)

        self.connection = com.Connection()
        self.connection.setStore(self.store)

        self.moves = motion.Moves(self.connection)

        self.motors = motors.Motors(self.connection)
        self.sensors = sensors.Sensors(self.connection)
        self.configs = config.Configurations(self.connection)

        self.environment = ''

    # Environment

    def loadEnvironment(self, environment):
        self.setEnvironment(environment)
        if os.path.exists(os.path.join(environment, 'LowLevelConfig.xml')):
            self.loadLowLevelConfig(os.path.join(environment, 'LowLevelConfig.xml'))
        else:
            self.loadLowLevelConfig(os.path.join(environment, 'ConfigFiles', 'LowLevelConfig.xml'))

        if os.path.exists(os.path.join(environment, 'MoveSchedulerConfig.xml')):
            self.loadMoveSchedulerConfig(os.path.join(environment, 'MoveSchedulerConfig.xml'))
        else:
            self.loadMoveSchedulerConfig(os.path.join(environment, 'ConfigFiles', 'MoveSchedulerConfig.xml'))

    def setEnvironment(self, environment):
        self.environment = environment

    # Connection

    def connect(self, hostname = 'localhost', port = 12345):
        self.connection.connectTo(hostname, port)

    def isConnected(self):
        return self.connection.connected

    def serverVersion(self):
        response = self.connection.ServerGetVersion_response()
        return response[0]

    def testConnection(self):
        print('Testing server version...')
        response = self.connection.ServerGetVersion_response()
        print('~> %s' % response[0])

        print('Testing echo "Hello world"...')
        response = self.connection.ServerEcho_response(b'Hello world')
        print('~> %s' % response[0])

        if response[0] == 'Hello world':
            print("\n"+'Connection test passed')

    # Configuration

    def loadLowLevelConfig(self, config, force = False):
        self.configs.loadLowLevelConfig(config, force)

    def loadMoveSchedulerConfig(self, config, force = False):
        self.configs.loadMoveSchedulerConfig(config, force)
        self.motors.setConfig(self.configs.moveSchedulerConfig)

    # Motors

    def allCompliant(self):
        self.motors.allCompliant()

    # Moves

    def moveFileName(self, name):
        return os.path.join(self.environment, 'Moves', name + '.xml')

    def loadMove(self, name):
        filename = self.moveFileName(name + '.graphics')

        if os.path.exists(filename):
            self.moves.loadMove(filename)
        else:
            self.moves.loadMove(self.moveFileName(name))

    def startMove(self, name, duration = 0, smooth = 500):
        self.moves.startMove(name, duration, smooth)

    def pauseMove(self, name):
        self.moves.pauseMove(name)

    def stopMove(self, name, smooth = 500):
        self.moves.stopMove(name, smooth)

    def killMove(self, name):
        self.moves.killMove(name)

    def getLoadedMoves(self):
        return self.moves.getLoadedMoves()

    def updateConstant(self, moveName, constantName, value):
        self.moves.updateConstant(moveName, constantName, float(value))

    def emergency(self):
        self.connection.SchedulerEmergencyStop()
        self.motors.allCompliant()

    def stop(self):
        self.connection.stop()
        self.motors.stop()
