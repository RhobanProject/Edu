'''
Created on 30 août 2012

@author: Hugo
'''
from timer import RepeatedTimer

class StateMachineLoader(RepeatedTimer):

    class ScheduledMachine():
        def __init__(self, machine):
            self.machine = machine
            self.max_counter = 1
            self.counter = 0
            
        def get_interval(self):
            return self.machine.interval

        interval = property(get_interval) 


    def __init__(self):
        RepeatedTimer.__init__(self,0.1,self.step)
        self.machines = []
        RepeatedTimer.start(self)

    def load_machine(self, machine):
        self.lock.acquire()
        print("Loading machine "+ machine.name)
        machine.stop()
        machine.scheduler = self
        self.machines.append(StateMachineLoader.ScheduledMachine(machine))
        self.update_frequencies()
        machine.play(False)
        self.lock.release()
   
    def load_machines(self, machines):
        self.lock.acquire()
        for machine in machines :
            machine.scheduler = self
            self.machines.append(StateMachineLoader.ScheduledMachine(machine))
            self.update_frequencies()
            machine.play(False)
            #print("Globals from load machines ", globals().keys())
            #print("Locals from load machines ", locals().keys())
        self.lock.release()
    
    def update_frequencies(self):
        self.lock.acquire()
        self.interval = 0.1
        for machine in self.machines:
            self.interval = min(self.interval, machine.interval)
        for machine in self.machines:
            machine.counter = 0
            machine.max_counter =  int(round(machine.interval / self.interval))
        self.lock.release()
    
    def step(self):
        for machine in self.machines:
            machine.counter = machine.counter + 1
            if machine.counter >= machine.max_counter :
                machine.counter = 0
                machine.machine.step()
                
    def get_machine(self, machine_name):
        for machine in self.machines :
            if machine.machine.name == machine_name:
                return machine.machine
        return None
         
    def set_machine_state(self, machine_name, state):
        machine = self.scheduler.get_machine(machine_name)
        machine.state = state
