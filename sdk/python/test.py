#!/usr/bin/python
# -*- coding: utf8 -*-

import sys, os
import rhoban.communication as com
import sockets.tcp as tcp

filename = '../common/commands.xml'
store = com.CommandsStore()
store.parseXml(filename)

connection = com.Connection('localhost', 12345)
connection.setStore(store)

response = connection.ServerEcho('Hello world')
data = response.readData()

print data[0]
