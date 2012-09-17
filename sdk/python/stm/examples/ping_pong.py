

from repeated_task import RepeatedTask

from time import time
global ThePingMachine


'''The ping machine
'''

class ThePingMachine(RepeatedTask):
    class Status:
        (Playing, Suspended, Stopped) = range(0,3)

    def play(self, threaded = True):
        if self.status == self.Status.Stopped :
            print("Starting machine '" + self.name+"'")
            globals()[self.name] = self
            self.set_state("Initial")
            self.status = self.Status.Playing
            if threaded : RepeatedTask.start(self)
        elif self.status == self.Status.Suspended :
            if self.debug : print("Resuming machine '"+ self.name+"'")
            self.status = self.Status.Playing
            
    def set_state(self, state):
        if state is not self.state:
            self.bye()
            self.state = state
            self.enter()
        self.state = state
        
    def stop(self, threaded = True):
        if self.debug : print("Stopping machine '"+ self.name+"'")
        self.status = self.Status.Stopped
        RepeatedTask.cancel(self)

    def step(self):
        try:
            if self.debug : print("Stepping machine '"+ self.name+"' with status " + str(self.status))
            if self.status == self.Status.Playing :
                self.loop()
                #print("Transition of machine " + self.name + " in state "+ str(self.state))
                self.set_state(self.transition())
                if self.state == "Final" :
                    if self.debug : print("Machine has reached its final state'"+ self.name+"'")
                    self.bye()
                    self.stop()
        except Exception as e:
            print("Exception in machine "+ self.name + ": "+ str(e))
            pass

    def __init__(self, verbose=False):
        RepeatedTask.__init__(self,1,self.step)
        self.name = "ThePingMachine"
        self.frequency = 5.0
        self.status = self.Status.Stopped 
        self.state = None
        self.debug = False

    def enter(self):
        if self.state == "Initial" :
            print('Starting ping machine')
            ThePingMachine.begin = time()
        elif self.state == "Receive" :
            print(str(time() - ThePingMachine.begin) )

    def bye(self):
        pass
        

    def loop(self):
        pass
        

    def transition(self):
        if self.state == "Initial" :
            return "Send"
        elif self.state == "Send" :
            print("Ping ",end="")
            ThePongMachine.set_state("Receive")
            return "Wait"
        elif self.state == "Receive" :
            return "Send"
        return self.state


if __name__ == '__main__':
    the_machine = ThePingMachine()
    the_machine.play()


from repeated_task import RepeatedTask

global ThePongMachine


'''The pong machine
'''

class ThePongMachine(RepeatedTask):
    class Status:
        (Playing, Suspended, Stopped) = range(0,3)

    def play(self, threaded = True):
        if self.status == self.Status.Stopped :
            print("Starting machine '" + self.name+"'")
            globals()[self.name] = self
            self.set_state("Initial")
            self.status = self.Status.Playing
            if threaded : RepeatedTask.start(self)
        elif self.status == self.Status.Suspended :
            if self.debug : print("Resuming machine '"+ self.name+"'")
            self.status = self.Status.Playing
            
    def set_state(self, state):
        if state is not self.state:
            self.bye()
            self.state = state
            self.enter()
        self.state = state
        
    def stop(self, threaded = True):
        if self.debug : print("Stopping machine '"+ self.name+"'")
        self.status = self.Status.Stopped
        RepeatedTask.cancel(self)

    def step(self):
        try:
            if self.debug : print("Stepping machine '"+ self.name+"' with status " + str(self.status))
            if self.status == self.Status.Playing :
                self.loop()
                #print("Transition of machine " + self.name + " in state "+ str(self.state))
                self.set_state(self.transition())
                if self.state == "Final" :
                    if self.debug : print("Machine has reached its final state'"+ self.name+"'")
                    self.bye()
                    self.stop()
        except Exception as e:
            print("Exception in machine "+ self.name + ": "+ str(e))
            pass

    def __init__(self, verbose=False):
        RepeatedTask.__init__(self,1,self.step)
        self.name = "ThePongMachine"
        self.frequency = 5.0
        self.status = self.Status.Stopped 
        self.state = None
        self.debug = False

    def enter(self):
        if self.state == "Initial" :
            print('Starting pong machine')
            ThePongMachine.begin = time()
        elif self.state == "Receive" :
            print(str(time() - ThePongMachine.begin))

    def bye(self):
        pass
        

    def loop(self):
        pass
        

    def transition(self):
        if self.state == "Initial" :
            return "Wait"
        elif self.state == "Send" :
            print("Pong ",end="")
            ThePingMachine.set_state("Receive")
            return "Wait"
        elif self.state == "Receive" :
            return "Send"
        return self.state


if __name__ == '__main__':
    the_machine = ThePongMachine()
    the_machine.play()
