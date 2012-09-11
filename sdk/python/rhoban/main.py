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
                'basic': [HelpCommand(), StatusCommand(), EmergencyCommand()],
                'moves': [LoadMoveCommand(), KillMoveCommand(),
                    StartMoveCommand(), StopMoveCommand(), UpdateConstantCommand()],
                'motors': [CompliantCommand(), HardCommand(),
                    SetCommand(), ScanCommand(), SnapshotCommand(),
                    InitCommand(), ZeroCommand(), MonitorCommand()],
                'sensors': [SensorsCommand()]
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
        print('Rhoban System - http://edu.rhoban-system.fr/sdk/commandes.html')
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
                self.executeFor(robot, options)
        else:
            for name in arguments:
                if name in self.robots.robots:
                    self.executeFor(self.robots.robots[name], options)
                else:
                    print('Robot %s not found' % name)
                    return

    def executeFor(self, robot, options):
        pass

    def stop(self):
        if self.robots:
            self.robots.stop()


"""
    Commande abstraire qui s'applique à un seul robot
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
    Commande permettant de visualiser le statut de la connexion
"""
class StatusCommand(RobotsCommand):
    def define(self):
        self.name = 'status'
        self.prototype = '[-v]'
        self.description = 'Displays the connection\'s status, -v for detailed informations'
        self.options = 'v'

    def executeFor(self, robot, options):
        loadedLabel = {True: 'loaded', False: 'not loaded'}

        print('. Status for ' +robot.name + ':')

        # Connection
        if robot.isConnected():
            print('|- Server version: %d' % robot.serverVersion())
            print('|- Connected at %s:%d' % (robot.connection.hostname, robot.connection.port))
        else:
            print('|- Disconnected')
            return

        # Configurations
        loaded = robot.configs.isLowLevelConfigLoaded()
        print('|- Low level config: %s' % (loadedLabel[loaded]))

        loaded = robot.configs.isMoveSchedulerConfigLoaded()
        print('|- Move scheduler config: %s' % (loadedLabel[loaded]))

        # Motors
        robot.motors.pullValues()
        availableMotors = filter(lambda motor: motor.lastUpdate != None, robot.motors.motors.values())
        print('|- %d/%d motors (%s)' % (len(availableMotors), len(robot.motors.motors), ','.join(map(str, robot.motors.idMotors.keys()))))
    
        if '-v' in options:
            prefix = ' \-'
            for id, motor in robot.motors.motors.items():
                if motor.lastUpdate:
                    print('%s %s (id: %d), angle: %g, speed: %g, load: %g' % (prefix, motor.name, motor.id, motor.getAngle(), motor.getSpeed(), motor.getLoad()))
                else:
                    print('%s %s (id: %d) Not detected' % (prefix, motor.name, motor.id))
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

        if len(arguments) >= 1:
            smooth = arguments[1]

        robot.stopMove(arguments[0], smooth)

"""
    Mise à jour d'une constante
"""
class UpdateConstantCommand(RobotCommand):
    def define(self):
        self.name = 'updateconstant'
        self.arguments = 4
        self.prototype = '<robot> <moveName> <constantName> <value>'
        self.description = 'Updates a move constant'

    def execute(self, robot, options, arguments):
        robot.updateConstant(arguments[0], arguments[1], arguments[2])

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
        self.description = 'Harden all the servos'

    def executeFor(self, robot, options):
        print('Harden servos for %s...' % robot.name)
        robot.motors.pullValues()
        robot.motors.allHard()
        robot.motors.pushValues()

"""
    Définit la valeur d'un angle 
"""
class SetCommand(RobotCommand):
    def define(self):
        self.name = 'set'
        self.options = 'r'
        self.description = 'Set a motor angle (in °), -r for relative angle'
        self.prototype = '[-r] <robot> <motorName> <angle> [load=1 [speed=1]]'
        self.arguments = 3

    def execute(self, robot, options, arguments):
        robot.motors.pullValues()
        motor = robot.motors[arguments[0]]
        arguments = arguments[1:]

        if '-r' in options:
            motor.setRelAngle(arguments[0])
        else:
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

        print('Setting motor %s (id=%d) angle=%g, load=%g, speed=%g' % (motor.name, motor.id, motor.goalAngle, motor.goalLoad, motor.goalSpeed))
            
        robot.motors.pushValues()


"""
    Fait prendre la position initial à un ou plusieurs robots
"""
class InitCommand(RobotsCommand):
    def define(self):
        self.name = 'init'
        self.description = 'Take the initial position for servos'

    def executeFor(self, robot, options):
        print('Taking initial servos position for %s' % robot.name)
        robot.motors.goToInit(5, True)

"""
    Fait prendre la position zero à un ou plusieurs robots
"""
class ZeroCommand(RobotsCommand):
    def define(self):
        self.name = 'zero'
        self.description = 'Take the zero position for servos'

    def executeFor(self, robot, options):
        print('Taking zero servos position for %s' % robot.name)
        robot.motors.goToZero(5, True)

"""
    Lance un scan
"""
class ScanCommand(RobotsCommand):
    def define(self):
        self.name = 'scan'
        self.description = 'Run a scan'

    def executeFor(self, robot, options):
        print('Running a scan for %s' % robot.name)
        robot.motors.scan()

"""
    Arrêt d'urgence
"""
class EmergencyCommand(RobotsCommand):
    def define(self):
        self.name = 'emergency'
        self.description = 'Emergency stop'

    def executeFor(self, robot, options):
        print('Emergency stopping all moves and turning compliant for %s' % robot.name)
        robot.emergency()

"""
    Capture de la valeur de moteurs
"""
class SnapshotCommand(RobotsCommand):
    def define(self):
        self.name = 'snapshot'
        self.options = 'rp'
        self.prototype = '[-r] [-p]'
        self.description = 'Snapshots the angles\' values (-r for relative, -p for python)'

    def executeFor(self, robot, options):
        print('Snapshoting motors for %s' % robot.name)
        robot.motors.pullValues()
        snapshot = {}

        for name, motor in robot.motors.motors.items():
            if motor.lastUpdate != None:
                if '-r' in options:
                    snapshot[motor.name] = motor.getRelAngle()
                else:
                    snapshot[motor.name] = motor.getAngle()

        if '-p' in options:
            print(repr(snapshot))
        else:
            for motor, angle in snapshot.items():
                print('%s: %s' % (motor, angle))

"""
    Monitorer les moteurs d'un robot
"""
class MonitorCommand(RobotCommand):
    def define(self):
        self.name = 'monitor'
        self.prototype = '[-f frequency] [-i] <robotName>'
        self.options = 'f:i'
        self.arguments = 1
        self.description = 'Monitors the servos (-i sorts by id)'

    def execute(self, robot, options, arguments):
        fmt = ' | %-10s | %-4s | %-7s | %-12s | %-8s | %-8s |'

        frequency = float(options.get('-f', 1))
        robot.motors.start(3 * frequency)

        while True:
            os.system('clear')
            headline = (fmt % ('Name', 'Id', 'Present', 'Angle', 'Load', 'Speed'))
            print(' Monitoring %s' % robot.name)
            print(' ' + ('-' * (len(headline)-1)))
            print(headline)
            print(' ' + ('-' * (len(headline)-1)))

            if '-i' in options:
                items = sorted(robot.motors.idMotors.items())
            else:
                items = sorted(robot.motors.motors.items())

            for name, motor in items:
                if motor.lastUpdate != None:
                    load = '%g%%' % round(100*motor.getLoad(), 1)
                    speed = '%g%%' % round(100*motor.getSpeed(), 1)
                    print(fmt % (motor.name, motor.id, 'Yes', motor.getAngle(), load, speed))
                else:
                    print(fmt % (motor.name, motor.id, 'No', '-', '-', '-'))

            print(' ' + ('-' * (len(headline)-1)))
            time.sleep(1.0/frequency)

"""
    Monitorer les sensors
"""
class SensorsCommand(RobotCommand):
    def define(self):
        self.name = 'sensors'
        self.prototype = '<robotName>'
        self.arguments = 1
        self.description = 'Monitor the sensors values'

    def execute(self, robot, options, arguments):
        robot.sensors.start(5)
        fmt = ' | %10s | %10s |'

        while True:
            os.system('clear')
            headline = (fmt % ('Name', 'Value'))
            separator = ' ' + ('-' * (len(headline)-1))

            print(' Monitoring sensors of %s' % robot.name)
            print(separator)
            print(headline)
            print(separator)

            for name, sensor in robot.sensors.sensors.items():
                if sensor.values:
                    print(fmt % (sensor.name, sensor.values[0]))
                else:
                    print(fmt % (sensor.name, '?'))

            print(separator)
            time.sleep(0.5)

