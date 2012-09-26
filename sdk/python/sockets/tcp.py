#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys, os, socket, pickle

"""
    Client TCP, peut se connecter et envoyer/recevoir des données
"""
class TCPClient(object):
    def __init__(self):
        self.socket = None
        self.connected = False

    def connectTo(self, hostname, port):
        if self.socket != None:
            self.close()

        self.socket = socket.socket(socket.AF_INET,socket.SOCK_STREAM)

        try:
            self.connected = True
            self.socket.connect((hostname, port))
        except Exception:
            self.connected = False

    def transmit(self, data):
        self.socket.send(data)

    def receive(self, size):
        ret = self.socket.recv(size)

        if ret != 0:
            return ret
        else:
            self.connected = False
            return 0

    def close(self):
        self.socket.shutdown(2)
        self.socket.close()
        self.connected = False
