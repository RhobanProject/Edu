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
import datetime
from threading import Event


'''A loader for state machines, check stm.py for documentation'''

class StateMachineLoader(RepeatedTask):

    class ScheduledMachine():
        def __init__(self, machine):
            self.machine = machine
            self.max_counter = 1
            self.counter = 0
                
        def get_interval(self):
            return self.machine.interval

        interval = property(get_interval) 


    def __init__(self):
        RepeatedTask.__init__(self,0.1,self.step)
        self.machines = {}
        RepeatedTask.start(self)

    ''' Load a machine or a list of machines,
     with an optional maximum duration
     and optionally waiting for the machine to stop
     and optionally waiting for the machine to be started stopped'''
    def load(self, machine, wait_stop = False, duration = float("inf"), start_stopped = False):
        self.lock.acquire()
        print("Loading machine "+ machine.name)
        if machine.status is not machine.Status.Stopped :
            self.lock.release()
            raise BaseException("Cannot load a running machine")
        for submachine in machine.submachines.values() :
            self.load(submachine, False, duration, True)
        if machine.name in self.machines :
            self.lock.release()
            raise Exception("Machine " + machine.name + " already loaded")
        machine.loader = self
        self.machines[machine.name] = StateMachineLoader.ScheduledMachine(machine)
        self.update_frequencies()
        self.lock.release()
        if not start_stopped:
            machine.play(wait_stop, duration, False)
   
    def startMachine(self, machineName, duration = float("inf")):
        machine = self.getMachine(machineName)
        machine.machine.play(False, duration, False)

    def stopMachine(self, machineName):
        machine = self.getMachine(machineName)
        machine.machine.stop()

    def setState(self, machineName, stateName):
        machine = self.getMachine(machineName)
        machine.machine.set_state(stateName)

    def kill(self, machineName):
        self.lock.acquire()
        if machineName in self.machines :
            del self.machines[machineName]
        else :
            self.lock.release()
            raise Exception("Unknown machine '" + machineName + "'")
        self.lock.release()
        
    def update_frequencies(self):
        self.lock.acquire()
        self.interval = 0.1
        for machine in self.machines.values():
            self.interval = min(self.interval, machine.interval)
        for machine in self.machines.values():
            machine.counter = 0
            machine.max_counter =  int(round(machine.interval / self.interval))
        self.lock.release()
    
    def step(self):
        self.lock.acquire()
        for machine in self.machines.values():
            try:
                machine.counter = machine.counter + 1
                if machine.counter >= machine.max_counter :
                    machine.counter = 0
                    machine.machine.step()
            except Exception as e:
                print("Exception '" + str(e) + "' when stepping machine '" + machine.name +" in state " + machine.state.name)
        self.lock.release()
        
            
                
