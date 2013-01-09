#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import time
from rhoban.main import RhobanMain
from rhoban.main import RobotsCommand, RobotCommand

class ExampleCommand(RobotCommand):
    def define(self):
        self.name = 'example'
        self.description = 'Example command'
        self.prototype = '<robotName>'
        self.arguments = 1

    def execute(self, robot, options, arguments):
        print('Example command for %s' % robot.name)

main = RhobanMain()
main.manager.addCommand(ExampleCommand(), 'example')
main.run()
