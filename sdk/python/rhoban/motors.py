#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys, os, re, threading, time
from datetime import datetime
import configurations as config
import communication as com

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

        for name, id in self.configuration.servos.items():
            self.motors[name] = Motor(id, name)
            self.idMotors[int(id)] = self.motors[name]

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

        for name in self.motors:
            motor = self.motors[name]
            if motor.goalAngle != None and motor.currentAngle != None and motor.currentSpeed != None and motor.dirty:
                ids += [motor.id]
                angles += [int(motor.goalAngle*1000)]
                speeds += [int(motor.goalSpeed*1023)]
                loads += [int(motor.goalLoad*1023)]

        if len(ids):
            self.connection.ServosSetValues(1, ids, angles, speeds, loads)
            
    def processValues(self, values):
        for id, angle, speed, load in zip(values[0], values[1], values[2], values[3]):
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
            
    def run(self):
        motors = self

        def motorsValues(values):
            motors.processValues(values)

        while self.running:
            self.connection.ServosGetValues_callback(1, motorsValues)
            self.pushValues()
            time.sleep(1.0/self.frequency)

"""
    Représente un moteur
"""
class Motor:
    def __init__(self, id, name):
        self.lastUpdate = None
        self.id = int(id)
        self.name = name

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

    def setLoad(self, load):
        self.dirty = True
        self.goalLoad = float(load)

    def setSpeed(self, speed):
        self.dirty = True
        self.goalSpeed = float(speed)

    def getAngle(self):
        return float(self.currentAngle)

    def getSpeed(self):
        return float(self.currentSpeed)

    def getLoad(self):
        return float(self.currentLoad)
