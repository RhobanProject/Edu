'''
Created on 6 sept. 2012

@author: Hugo
'''
from threading import Thread, Event
from time import time

class RepeatedTimer(Thread):
    """Call a function after every specified number of seconds:

    t = RepeatedTimer(30.0, f)
    t.start()
    t.cancel() # stop the timer's action if it's still waiting
    """
    def __init__(self, interval, function):
        Thread.__init__(self)
        self.interval = interval
        self.function = function
        self.finished = Event()

    def cancel(self):
        """Stop the timer if it hasn't finished yet"""
        self.finished.set()

    def run(self):
        while not self.finished.is_set() :
            self.function()
            #print("Repeated timer waiting " + str(self.interval) + " at " + str(time()))
            self.finished.wait(self.interval)
        