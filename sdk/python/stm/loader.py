'''
Created on 30 août 2012

@author: Hugo
'''

class StateMachineLoader():
    
    def __init__(self):
        self.machines = []

    def load_machine(self, machine):
        machine.scheduler = self
        self.machines.append(machine)
        machine.play()
                        