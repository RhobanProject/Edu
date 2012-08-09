#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys, os, re, threading, time
import configurations as config
import communication as com

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

        for name, id in self.configuration.servos.items():
            self.motors[name] = Motor(id, name)
            self.idMotors[int(id)] = self.motors[name]

        self.running = True
        super(Motors, self).start()

    def stop(self):
        self.running = False

    def compilant(self, name):
        self.motors[name].goalLoad = 0

    def allCompliant(self):
        self.connection.ServosAllCompliant()

        for name, motor in self.motors.items():
            motor.goalLoad = 0


    def hard(self, name):
        self.motors[name].goalLoad = 1023

    def allHard(self):
        for name, motor in self.motors.items():
            motor.goalLoad = 1023

    def run(self):
        def motorsValues(values):
            for id, angle, speed, load in zip(values[0], values[1], values[2], values[3]):
                motor = self.idMotors[int(id)]

                motor.currentAngle = angle
                if motor.goalAngle == None:
                    motor.goalAngle = motor.currentAngle

                motor.currentSpeed = speed
                if motor.goalSpeed == None:
                    motor.goalSpeed = max(0, motor.currentSpeed)

                motor.currentLoad = load
                if motor.goalLoad == None:
                    motor.goalLoad = max(0, motor.currentLoad)

        while self.running:
            self.connection.ServosGetValues_callback(1, motorsValues)

            ids = []
            angles = []
            speeds = []
            loads = []

            for name in self.motors:
                motor = self.motors[name]
                if motor.goalAngle != None and motor.currentAngle != None and motor.currentSpeed != None:
                    ids += [motor.id]
                    angles += [int(motor.goalAngle*1000)]
                    speeds += [int(motor.goalSpeed)]
                    loads += [int(motor.goalLoad)]

            if len(ids):
                self.connection.ServosSetValues(1, ids, angles, speeds, loads)

            time.sleep(1.0/self.frequency)

class Motor:
    def __init__(self, id, name):
        self.id = int(id)
        self.name = name

        self.goalAngle = None
        self.currentAngle = None

        self.goalSpeed = None
        self.currentSpeed = None

        self.goalLoad = None
        self.currentLoad = None
