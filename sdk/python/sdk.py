#!/usr/bin/env python
# -*- coding: utf-8 -*-

import time
from rhoban.main import RhobanMain
from rhoban.main import RobotsCommand, RobotCommand

class ADCCommand(RobotCommand):
    def define(self):
        self.name = 'adc'
        self.prototype = '<robotName>'
        self.arguments = 1
        self.description = 'Show the sensors values'

    def execute(self, robot, options, arguments):
        while True:
            values = robot.connection.GetSensorsValues_response(["Test"], 5)

            print(repr(values))

            time.sleep(0.1)

main = RhobanMain()
main.manager.addCommand(ADCCommand(), 'adc')
main.run()
