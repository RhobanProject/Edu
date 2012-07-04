#!/usr/bin/python
# -*- coding: utf8 -*-

import sys, os
import rhoban.communication as com

filename = '../common/commands.xml'
store = com.CommandStore(filename)

message = store.builder.SchedulerStopMove('Dance', 1000)
