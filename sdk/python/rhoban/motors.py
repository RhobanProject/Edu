#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys, os, re, threading, time
from datetime import datetime
import rhoban.configurations as config
import rhoban.communication as com

"""
    Classe permettant de gérer et de dispatcher un ensemble de moteurs
"""
class Motors(threading.Thread):
    def __init__(self, connection):
        super(Motors, self).__init__()
        self.connection = connection
        self.configuration = None
        self.motors = {}
        self.idMotors = {}
        self.running = False

    def setConfig(self, configuration):
        self.configuration = configuration

        for name, servo in self.configuration.servos.items():
            self.motors[name] = Motor(servo.id, name, servo.iniAngle, servo.zeroAngle)
            self.idMotors[int(servo.id)] = self.motors[name]

    def __len__(self):
        return len(self.motors)

    def __getitem__(self, key):
        return self.motors[key]

    def __iter__(self):
        return iter(self.motors)

    def start(self, frequency):
        self.frequency = frequency

        if self.configuration == None:
            raise Exception('No suitable motors configuration')

        self.running = True
        super(Motors, self).start()

    def stop(self):
        self.running = False

    def compilant(self, name):
        self.motors[name].setLoad(0)

    def allCompliant(self):
        self.connection.ServosAllCompliant()

        for name, motor in self.motors.items():
            motor.setLoad(0)

    def hard(self, name):
        self.motors[name].setLoad(1)

    def allHard(self):
        for name, motor in self.motors.items():
            motor.setLoad(1)

    def pullValues(self):
        values = self.connection.ServosGetValues_response(1)
        self.processValues(values)

    def pushValues(self):
        ids = []
        angles = []
        speeds = []
        loads = []

        for name, motor in self.motors.items():
            if motor.goalAngle != None and motor.currentAngle != None and motor.currentSpeed != None and motor.dirty:
                ids += [motor.id]
                angles += [int(motor.goalAngle*1000)]
                speeds += [int(motor.goalSpeed*1023)]
                loads += [int(motor.goalLoad*1023)]

        if len(ids):
            self.connection.ServosSetValues(1, ids, angles, speeds, loads)
            
    def processValues(self, values):
        for id, angle, speed, load in zip(values[0], values[1], values[2], values[3]):
            if id in self.idMotors:
                motor = self.idMotors[int(id)]
                motor.lastUpdate = datetime.now()

                motor.currentAngle = angle
                if motor.goalAngle == None:
                    motor.goalAngle = motor.currentAngle

                motor.currentSpeed = speed/1023.0
                if motor.goalSpeed == None:
                    motor.goalSpeed = max(0, motor.currentSpeed)

                motor.currentLoad = load/1023.0
                if motor.goalLoad == None:
                    motor.goalLoad = max(0, motor.currentLoad)
    
    def goToZero(self, duration = 5, verbose = False):
        self.pullValues()
        for name, motor in self.motors.items():
            motor.setAngle(motor.zeroAngle)

        self.raiseLoad(duration, verbose)

    def goToInit(self, duration = 5, verbose = False):
        self.pullValues()
        for name, motor in self.motors.items():
            motor.setRelAngle(0)

        self.raiseLoad(duration, verbose)

    def raiseLoad(self, duration = 5, verbose = False):
        cs = duration * 100
        for x in range(cs):
            if verbose:
                sys.stdout.write("\rLoad: %3d%%" % (round(100*(x+1)/float(cs))))
                sys.stdout.flush()
            for name, motor in self.motors.items():
                motor.setLoad((x +1) / float(cs))
            time.sleep(0.01)
            self.pushValues()
        if verbose:
            print('')
            
    def run(self):
        motors = self

        def motorsValues(values):
            motors.processValues(values)

        while self.running:
            self.connection.ServosGetValues_callback(1, motorsValues)
            self.pushValues()
            time.sleep(1.0/self.frequency)

    def scan(self):
        for name, motor in self.motors.items():
            motor.lastUpdate = None

        self.connection.ServosScan(250, 'Normal')
"""
    Représente un moteur
"""
class Motor:
    def __init__(self, id, name, iniAngle, zeroAngle):
        self.lastUpdate = None
        self.id = int(id)
        self.name = name
        self.iniAngle = float(iniAngle)
        self.zeroAngle = float(zeroAngle)

        self.goalAngle = None
        self.currentAngle = None

        self.goalSpeed = None
        self.currentSpeed = None

        self.goalLoad = None
        self.currentLoad = None

        self.dirty = False

    def setAngle(self, angle):
        self.dirty = True
        self.goalAngle = float(angle)

    def setRelAngle(self, angle):
        self.dirty = True
        self.goalAngle = self.iniAngle + self.zeroAngle + float(angle)

    def setLoad(self, load):
        self.dirty = True
        self.goalLoad = float(load)

    def setSpeed(self, speed):
        self.dirty = True
        self.goalSpeed = float(speed)

    def getAngle(self):
        return float(self.currentAngle)

    def getRelAngle(self):
        return float(self.currentAngle - self.iniAngle - self.zeroAngle)

    def getSpeed(self):
        return float(self.currentSpeed)

    def getLoad(self):
        return float(self.currentLoad)
