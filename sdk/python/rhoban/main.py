#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys, os, re, threading, time
import commands
from robot import Robots

"""
    Classe pouvant être exécutée pour présenter les commandes
    du SDK à l'utilisateur
"""
class RhobanMain(object):
    def __init__(self):
        self.manager = commands.CommandsManager()
        myCommands = {
                'basic': [HelpCommand(), StatusCommand()],
                'moves': [LoadMoveCommand(), KillMoveCommand(),
                    StartMoveCommand(), StopMoveCommand()],
                'motors': [CompliantCommand(), HardCommand(),
                    SetCommand()]
                }

        for family, familyCommands in myCommands.items():
            for command in familyCommands:
                self.manager.addCommand(command, family)

    def run(self):
        self.manager.run(sys.argv[1:])
"""
    Commande permettant de lister les commandes disponibles
"""
class HelpCommand(commands.Command):
    def define(self):
        self.name = 'help'
        self.description = 'Displays the available commands'
        self.prototype = '[family]'

    def run(self, options, arguments):
        print('Available commands:')

        families = sorted(self.manager.families.items())

        for family, commands in families:
            if not arguments or family in arguments:
                print('')
                print('%s:' % family)
                commands = sorted(commands.items())

                for name, command in commands:
                    print('  %s %s: %s' % (command.name, command.prototype, command.description))

"""
    Commande abstraite qui charge la configuration
"""
class RobotsCommand(commands.Command):
    def run(self, options, arguments):
        self.robots = Robots()
        self.robots.loadYaml('config.yml')

        if not arguments:
            for name, robot in self.robots.robots.items():
                robot.name = name
                self.executeFor(robot, options)
        else:
            for name in arguments:
                if name in self.robots.robots:
                    self.executeFor(robot, options)
                else:
                    print('Robot %s not found' % name)
                    return

    def executeFor(self, robot, options):
        pass

    def stop(self):
        if self.robots:
            self.robots.stop()


"""
    Commands abstraire qui s'applique à un seul robot
"""
class RobotCommand(commands.Command):
    def run(self, options, arguments):
        if not arguments:
            print('You must specify a robot name')
            return

        robotName = arguments[0]
        arguments = arguments[1:]

        self.robots = Robots()
        self.robots.loadYaml('config.yml')

        if robotName not in self.robots:
            print('Robot %s not found' % robotName)
            return

        self.execute(self.robots[robotName], options, arguments)

    def execute(self, robot, options, arguments):
        pass

    def stop(self):
        if self.robots:
            self.robots.stop()

"""
    Commande permettant de visualiser le status de la connexion
"""
class StatusCommand(RobotsCommand):
    def define(self):
        self.name = 'status'
        self.prototype = '[-v]'
        self.description = 'Displays the status of the connection'
        self.options = 'v'

    def executeFor(self, robot, options):
        loadedLabel = {True: 'loaded', False: 'not loaded'}

        print('. Status for ' + robot.name + ':')

        # Connection
        if robot.isConnected():
            print('|- Connected at %s:%d' % (robot.connection.hostname, robot.connection.port))

        # Configurations
        loaded = robot.configs.isLowLevelConfigLoaded()
        print('|- Low level config: %s' % (loadedLabel[loaded]))

        loaded = robot.configs.isMoveSchedulerConfigLoaded()
        print('|- Move scheduler config: %s' % (loadedLabel[loaded]))

        # Motors
        robot.motors.pullValues()
        print('|- %d motors (%s)' % (len(robot.motors.motors), ','.join(map(str, robot.motors.idMotors.keys()))))
    
        if '-v' in options:
            prefix = ' \-'
            for id, motor in robot.motors.motors.items():
                print('%s %s (id: %d), angle: %g, speed: %g, load: %g' % (prefix, motor.name, motor.id, motor.getAngle(), motor.getSpeed(), motor.getLoad()))
                prefix = ' |-'

        # Moves
        moves = robot.getLoadedMoves()
        print('|- Loaded move(s): %d' % (len(moves)))

        if '-v' in options:
            prefix = ' \-'
            if moves:
                for move in moves:
                    print('%s %s' % (prefix, move))
                    prefix = ' |-'

        print('')

"""
    Commande de chargement de mouvement
"""
class LoadMoveCommand(RobotCommand):
    def define(self):
        self.name = 'loadmove'
        self.prototype = '<robot> <moveName>'
        self.arguments = 2
        self.description = 'Loads the move moveName'

    def execute(self, robot, options, arguments):
        robot.loadMove(arguments[0])

"""
    Commande pour tuer un mouvement
"""
class KillMoveCommand(RobotCommand):
    def define(self):
        self.name = 'killmove'
        self.prototype = '<robot> <moveName>'
        self.arguments = 2
        self.description = 'Kills the move moveName (stop and unload it)'

    def execute(self, robot, options, arguments):
        robot.killMove(arguments[0])

"""
    Commande de lancement de mouvement
"""
class StartMoveCommand(RobotCommand):
    def define(self):
        self.name = 'startmove'
        self.prototype = '<robot> <moveName> [duration=0 [smooth=500]]'
        self.arguments = 2
        self.description = 'Starts the moveName move'

    def execute(self, robot, options, arguments):
        duration = 0
        smooth = 500

        if len(arguments) >= 2:
            duration = arguments[1]
        if len(arguments) >= 3:
            smooth = arguments[2]

        robot.startMove(arguments[0], duration, smooth)

"""
    Commande de stoppage de mouvement
"""
class StopMoveCommand(RobotCommand):
    def define(self):
        self.name = 'stopmove'
        self.arguments = 2
        self.prototype = '<robot> <moveName> [smooth=500]'
        self.description = 'Stops the move moveName'

    def execute(self, robot, options, arguments):
        smooth = 500

        if len(arguments) >= 2:
            smooth = arguments[2]

        robot.stopMove(arguments[0], smooth)

"""
    Relâche tous les servos
"""
class CompliantCommand(RobotsCommand):
    def define(self):
        self.name = 'compliant'
        self.description = 'Release all the servos'

    def executeFor(self, robot, options):
        print('Releasing servos for %s...' % robot.name)
        robot.motors.allCompliant()

class HardCommand(RobotsCommand):
    def define(self):
        self.name = 'hard'
        self.description = 'Hard all the servos'

    def executeFor(self, robot, options):
        print('Hard all servos for %s...' % robot.name)
        robot.motors.pullValues()
        robot.motors.allHard()
        robot.motors.pushValues()

"""
    Définit la valeur d'un angle 
"""
class SetCommand(RobotCommand):
    def define(self):
        self.name = 'set'
        self.description = 'Set a motor angle (in °)'
        self.prototype = '<robot> <motorName> <angle> [torque = 1023 [speed = 1023]]'
        self.arguments = 3

    def execute(self, robot, options, arguments):
        robot.motors.pullValues()
        motor = robot.motors[arguments[0]]
        arguments = arguments[1:]

        motor.setAngle(arguments[0])
        arguments = arguments[1:]

        if arguments:
            motor.setLoad(arguments[0])
            arguments = arguments[1:]
        else:
            motor.setLoad(1)
        
        if arguments:
            motor.setSpeed(arguments[0])
            arguments = arguments[1:]
        else:
            motor.setSpeed(1)

        print('Setting motor %s (id=%d) angle=%g, load=%g, float=%g' % (motor.name, motor.id, motor.goalAngle, motor.goalLoad, motor.goalSpeed))
            
        robot.motors.pushValues()
