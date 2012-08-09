#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys, os, time, math
import rhoban.robot as robot
import sockets.tcp as tcp

spider = None

try:
    directory = os.getenv('HOME') + '/Rhoban/Environments/RobotBoard/Spider/'
    lowLevelConfig = directory + 'ConfigFiles/LowLevelConfig.xml'
    moveSchedulerConfig = directory + 'ConfigFiles/MoveSchedulerConfig.xml'

    spider = robot.Robot()
    spider.connect()

    spider.configs.loadLowLevelConfig(lowLevelConfig)
    spider.configs.loadMoveSchedulerConfig(moveSchedulerConfig)

    spider.motors.start(50)
    spider.allCompliant()

    spider.motors['ML1'].goalLoad = 1023
    spider.motors['ML2'].goalLoad = 1023
    spider.motors['ML3'].goalLoad = 1023

    while True:
        if spider.motors['MR1'].currentAngle != None:
            spider.motors['ML1'].goalAngle = -1*spider.motors['MR1'].currentAngle
            spider.motors['ML2'].goalAngle = -1*spider.motors['MR2'].currentAngle
            spider.motors['ML3'].goalAngle = -1*spider.motors['MR3'].currentAngle

    spider.stop()

except (Exception, KeyboardInterrupt, SystemExit):
    if spider:
        spider.allCompliant()
        spider.stop()
    raise
