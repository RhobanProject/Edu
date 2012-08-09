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

    spider.motors.allHard()

    t = 0
    while True:
        for name in spider.motors:
            spider.motors[name].goalAngle = math.sin(t)*50
        t = t+0.005
        time.sleep(0.01)

    spider.stop()

except (Exception, KeyboardInterrupt, SystemExit):
    if spider:
        spider.allCompliant()
        spider.stop()
    raise
