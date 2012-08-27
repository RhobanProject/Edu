#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys, os, re, threading, time
from datetime import datetime
import configurations as config
import communication as com

"""
    Classe permetant d'intéragir avec les capteurs
"""
class Sensors(threading.Thread):
    def __init__(self, connection):
        super(Sensors, self).__init__()
        self.connection = connection
        self.sensors = {}

    def __len__(self):
        return len(self.sensors)

    def __getitem__(self, key):
        return self.sensors[key]

    def __iter__(self):
        return iter(self.sensors)

    def stop(self):
        self.running = False

    def start(self, frequency = 10):
        self.frequency = frequency
        self.sensors = {}
        self.running = True

        sensors = self.connection.LowLevelGetAnalogicSensors_response()

        if not sensors:
            raise Exception('Unable to get the sensors values')

        for sensorName in sensors[0]:
            self.sensors[sensorName] = Sensor(sensorName)

        super(Sensors, self).start()
    
    def run(self):
        sensors = self
        names = []
        queue = 5
        for name in self.sensors:
            names += [name]

        def processResponse(response):
            for name, values in zip(response[0], response[1]):
                if name in sensors.sensors:
                    sensors.sensors[name].values = values

        while self.running:
            self.connection.GetSensorsValues_callback(names, queue, processResponse)
            time.sleep(1.0/self.frequency)

"""
    Un capteur
"""
class Sensor(object):
    def __init__(self, name):
        self.name = name
        self.values = []
