#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys, os, time
import rhoban.communication as com
import sockets.tcp as tcp

try:
    filename = '../common/commands.xml'
    store = com.CommandsStore()
    store.parseXml(filename)

    connection = com.Connection('localhost', 12345)
    connection.setStore(store)

    def func(x):
        print '~~> '+x[0]

    response = connection.ServerEcho_callback('Hello world', func)

    time.sleep(10)
    connection.stop()

except (Exception, KeyboardInterrupt, SystemExit):
    connection.stop()
    raise

