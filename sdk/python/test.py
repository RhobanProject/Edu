#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys, os, time
import rhoban.robot as robot
import sockets.tcp as tcp

try:
    directory = os.getenv('HOME') + '/Rhoban/Environments/RobotBoard/Spider/'
    lowLevelConfig = directory + 'ConfigFiles/LowLevelConfig.xml'
    moveSchedulerConfig = directory + 'ConfigFiles/MoveSchedulerConfig.xml'

    spider = robot.Robot()
    spider.connect()

    if spider.notLoaded():
        spider.configs.loadLowLevelConfig(lowLevelConfig)
        spider.configs.loadMoveSchedulerConfig(moveSchedulerConfig)
        spider.scan()

    spider.allCompliant()

    time.sleep(10)
    spider.stop()

except (Exception, KeyboardInterrupt, SystemExit):
    spider.stop()
    raise
