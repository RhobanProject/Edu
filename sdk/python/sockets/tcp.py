#!/usr/bin/python
# -*- coding: utf8 -*-

import sys, os, socket

"""
    Client TCP, peut se connecter et envoyer/recevoir des données
"""
class TCPClient:
    def __init__(self):
        self.socket = None
        pass

    def connectTo(self, hostname, port):
        if self.socket != None:
            self.close()

        self.socket = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
        self.socket.connect((hostname, port))

    def transmit(self, data):
        self.socket.send(data)

    def receive(self, size):
        return self.socket.recv(size)

    def close(self):
        self.socket.close()
