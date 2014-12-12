#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys, os, socket, pickle

from threading import RLock

"""
    Client TCP, peut se connecter et envoyer/recevoir des données
"""
class TCPClient(object):
    def __init__(self):
        self.socket = None
        self.connected = False
        self.socketLock = RLock()

    def connectTo(self, hostname, port):
        self.socketLock.acquire()
        try:
            if self.socket != None:
                self.close()
            self.socket = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
            self.socket.connect((hostname, int(port)))
            self.connected = True
        except Exception as e:
            print("TCPClient failed to connect to " + hostname + ":" + str(port))
            self.connected = False
        self.socketLock.release()

    def transmit(self, data):
        try :
            self.socketLock.acquire()
            self.socket.send(data)
        except Exception as e :
            self.socketLock.release()
            raise(e)
        self.socketLock.release()

    def receive(self, size):
        ret = self.socket.recv(size)
        if ret != 0:
            return ret
        else:
            self.connected = False
            return 0

    def close(self):
        self.socketLock.acquire()
        try :
            self.socket.shutdown(2)
            self.socket.close()
            self.connected = False
        except Exception :
            self.connected = False
        self.socketLock.release()
