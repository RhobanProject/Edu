'''
/*************************************************
* Publicly released by Rhoban System, September 2012
* www.rhoban-system.fr
*
* Freely usable for non-commercial purposes
*
* Licence Creative Commons *CC BY-NC-SA
* http://creativecommons.org/licenses/by-nc-sa/3.0
*************************************************/
'''
from repeated_task import RepeatedTask
from time import time

class StateMachineLoader(RepeatedTask):

    class ScheduledMachine():
        def __init__(self, machine):
            self.machine = machine
            self.max_counter = 1
            self.counter = 0
            self.begin = time()                
                
        def get_interval(self):
            return self.machine.interval

        interval = property(get_interval) 


    def __init__(self):
        RepeatedTask.__init__(self,0.1,self.step)
        self.machines = []
        RepeatedTask.start(self)

    ''' Load a machine,
     with an optional maximum duration
     and optionally waiting for the machine to reach its final state'''
    def load(self, machine, duration = float("inf"), wait = False):
        self.lock.acquire()
        if wait:
            machine.play(duration, True)
            machine.join()
        else :
            print("Loading machine "+ machine.name)
            machine.stop()
            machine.scheduler = self
            self.machines.append(StateMachineLoader.ScheduledMachine(machine))
            self.update_frequencies()
            machine.play(duration, False)
        self.lock.release()
   
    ''' Load a list of machines and starts them
    with an optional maximum duration'''
    def load_machines(self, machines, duration = float("inf"), wait = False):
        if wait:
            for machine in machines :
                machine.play(duration,True)
            for machine in machines :
                machine.join()
        else :                        
            self.lock.acquire()
            for machine in machines :
                machine.scheduler = self
                self.machines.append(StateMachineLoader.ScheduledMachine(machine))
                self.update_frequencies()
                machine.play(duration, False)
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
                
