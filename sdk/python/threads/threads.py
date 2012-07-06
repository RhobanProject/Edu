#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys, os, re, struct, threading

class StoppableThread(threading.Thread):
    def __init__(self):
        super(StoppableThread, self).__init__()
        self._stop = threading.Event()

    def stop(self):
        self._stop.set()

    def stopped(self):
        return self._stop.isSet()

