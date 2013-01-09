#!/usr/bin/env python
# -*- coding: utf-8 -*-

import time, math
from rhoban.robot import Robots

robots = Robots()
robots.loadYaml('config.yml')

sigmaban = robots['sigmaban']
sigmaban.motors.start(50)
t = 0

sigmaban.motors.allHard()

while True:
    t = t + 0.01
    sigmaban.motors['Coude D'].setAngle(math.sin(t)*40)
    time.sleep(0.01)

robots.stop()
