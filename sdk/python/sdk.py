#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys, os, time, math
import rhoban.robot as robot
import sockets.tcp as tcp

spider = None

try:
    print('Loading robots')
    robots = robot.Robots()
    robots.loadYaml('config.yml')
    robot = robots['default']

    print('Starting move...')
    robot.startMove('SpiderSinus', 0, 1000)

    print('Sleeping...')
    time.sleep(3)

    print('Setting amplitude to 0.5...')
    robot.updateConstant('SpiderSinus', 'Amplitude', 0.5)

    print('Sleeping...')
    time.sleep(3)

    print('Stopping move...')
    robot.stopMove('SpiderSinus', 1000)
    
    print('Loaded moves: ' + repr(robot.getLoadedMoves()))

    print('Killing move...')
    robot.killMove('SpiderSinus')
    
    print('Loaded moves: ' + repr(robot.getLoadedMoves()))

    robot.allCompliant()
    robots.stop()

except (Exception, KeyboardInterrupt, SystemExit):
    if robots:
        robots.stop()
    raise
