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
    time.sleep(5)

    robot.loadMove('/configs/SpiderSinus.xml')
#    robot.startMove('SpiderSinus', 0, 1000)

    robots.stop()

except (Exception, KeyboardInterrupt, SystemExit):
    if robots:
        robots.stop()
    raise
