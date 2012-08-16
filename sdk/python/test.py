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

    spider.loadLowLevelConfig(lowLevelConfig)
    spider.loadMoveSchedulerConfig(moveSchedulerConfig)

    spider.motors.start(30)
    spider.allCompliant()

    spider.motors['ML1'].setLoad(1023)
    spider.motors['ML2'].setLoad(1023)
    spider.motors['ML3'].setLoad(1023)

    while True:
        if spider.motors['MR1'].currentAngle != None:
            spider.motors['ML1'].setAngle(-1*spider.motors['MR1'].currentAngle)
            spider.motors['ML2'].setAngle(-1*spider.motors['MR2'].currentAngle)
            spider.motors['ML3'].setAngle(-1*spider.motors['MR3'].currentAngle)

    spider.stop()

except (Exception, KeyboardInterrupt, SystemExit):
    if spider:
        if spider.isConnected():
            spider.allCompliant()
        spider.stop()
    raise
