#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys, os, time, math
import rhoban.robot as robot
import sockets.tcp as tcp

spider = None

try:
    robots = robot.Robots()
    robots.loadYaml('config.yml')

    robot = robots['default']
    robot.motors.start(30)
    robot.allCompliant()

    robots.stop()

except (Exception, KeyboardInterrupt, SystemExit):
    if robots:
        robots.stop()
    raise
