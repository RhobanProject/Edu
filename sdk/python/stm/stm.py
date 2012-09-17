'''
Created on 16 aout 2012

@author: Hugo
'''
from repeated_task import RepeatedTask
from yaml import load_all as load_all
from importlib import import_module
from time import time

class StateMachine(RepeatedTask):
    
    class Status:
        (Playing, Suspended, Stopped) = range(0,3)
        
    '''
    Constructor
    '''
    def __init__(self, debug=False):
        RepeatedTask.__init__(self,1,self.step)
        '''The frequency of the machine'''
        self.frequency = 1.0
        '''The list of states'''
        self.states = {}
        self.state = None
        self.status = self.Status.Stopped
        self.debug = debug
        self.scheduler = None
        
        '''The name of the machine'''
        self.name = ""
        '''Adescription of its behaviour'''
        self.description = ""
        '''The preamble definition'''
        self.preamble = ""
        '''The functions definition'''
        self.functions = ""
        self.begin = time()
        self.duration = float('inf')
        
    def set_state(self, state_name):
        self.lock.acquire()
        state = self.get_state(state_name)
        if state is not self.state:
            self.bye()
            self.state = state
            self.enter()
        self.state = state
        self.lock.release()

    def play(self, duration = float("inf"), threaded = True):
        self.lock.acquire()
        self.begin = time()
        self.duration = duration
        if self.status == self.Status.Stopped :
            if self.debug: print("Starting machine \'" + self.name+"\'")
            globals()[self.name] = self
            exec(self.preamble)
            for line in self.preamble.splitlines() :
                imports = line.split(' ')
                if len(imports) >= 2:
                    if imports[0]=='def':
                        globals()[imports[1][:-3]] = locals()[imports[1][:-3]]
                    elif imports[0]=='import':
                        import_module(imports[1])
                    elif imports[0]=='from' and imports[2]=='import' :
                        if len(imports) == 4:
                            globals()[imports[3]] = getattr(__import__(imports[1]), imports[3])
                        elif len(imports) == 6 and imports[4]=='as':
                            globals()[imports[5]] = getattr(__import__(imports[1]), imports[3]) 

            self.set_state("Initial")
            self.status = self.Status.Playing
            if threaded :
                RepeatedTask.start(self)
        elif self.status == self.Status.Suspended :
            if self.debug : print("Resuming machine \'"+ self.name+"\'")
            self.status = self.Status.Playing
        self.lock.release()
            
    def suspend(self):
        self.lock.acquire()
        if self.debug : print("Suspending machine \'"+ self.name+"\'")
        self.status = self.Status.Suspended
        self.lock.release()

    def stop(self, threaded = True):
        self.lock.acquire()
        if self.debug : print("Stopping machine \'"+ self.name+"\'")
        self.status = self.Status.Stopped
        if threaded : RepeatedTask.cancel(self)
        self.lock.release()
    
    def enter(self):
        if self.state and self.state.enter : exec(self.state.enter)

    def bye(self):
        if self.state and self.state.bye : exec(self.state.bye)

    def loop(self):
        if self.state and self.state.loop : exec(self.state.loop)

    def step(self):
        try:
            self.lock.acquire()
            if self.debug : print("Stepping machine \'"+ self.name+"\' with status " + str(self.status))
            if self.status == self.Status.Playing :
                self.loop()
                #print("Transition of machine " + self.name + " in state "+ str(self.state))
                self.set_state(self.transition())
                if time() - self.begin > self.duration:
                    if self.debug: print("Time elapsed for machine " + self.name + str(time() - self.begin) + " " + str(self.duration))
                    self.set_state("Final")
                if self.state.name == "Final":
                    if self.debug : print("Machine has reached its final state\'"+ self.name+"\'")
                    self.bye()
                    self.stop()
            self.lock.release()
        except Exception as e:
            print("Exception in machine "+ self.name + ": "+ str(e))
            pass
        #raise e

    def transition(self):  
        for transition in self.state.transitions:
            if not transition.condition or eval(transition.condition):
                if transition.do : exec(transition.do)
                return transition.next
        return self.state.name
    
    def get_state(self, state_name):
        for state in self.states:
            if state.name == state_name : return state
        raise BaseException("Could not find state with name "+ state_name + " in machine " + self.name)
        
    '''creates a list of machines extracted from a yaml file'''
    @classmethod
    def from_yaml(cls, filename):
        result = []
        with open(filename,'r') as yaml_stream:
            trees = load_all(yaml_stream)
            for tree in trees :
                machine = StateMachine()
                machine.from_tree(tree)
                result.append(machine)
        return result
  
    '''builds the machine descriptions from a yaml file'''
    @classmethod
    def yaml_to_py(cls, yaml_file, py_file):
        machines = StateMachine.from_yaml(yaml_file)
        with open(py_file,'w') as output:
            for machine in machines:
                output.write( machine.to_python() )
  
  
    '''get a machine by its name'''
    def get_machine(self, machine_name):
        if self.scheduler is None:
            return None
        else:
            return self.scheduler.get_machine(machine_name)

    '''set a machine state'''
    def set_machine(self, machine_name, state_name):
        if self.scheduler is None:
            return None
        else:
            machine = self.scheduler.get_machine(machine_name)
            if machine is not None:
                machine.set_state(state_name)
    
    '''If the chain is non empty, add_tag associates the value chain to the key 'tag'
    in the mapping 'mapping' '''
    @classmethod
    def add_tag(cls, tag,chain,mapping):
        if chain is not '' :
            mapping[tag] = chain
        
    '''If the mapping contains the key tag, return the key otherwise returns empty chain'''
    @classmethod
    def get_tag(cls, tag,mapping):
        if mapping.__contains__(tag)  :
            return mapping[tag]
        else :
            return ""
         
    class Transition(object):

        '''turns the transition into a map'''
        def toTree(self):
            result = {}
            StateMachine.add_tag('condition',self.condition,result)
            StateMachine.add_tag('do',self.do,result)
            StateMachine.add_tag('next',self.next,result)
            return result

        '''build the transition from a map'''
        def from_tree(self, mapping):
            self.condition = StateMachine.get_tag('condition' , mapping)
            self.do = StateMachine.get_tag('do' , mapping)
            self.next = StateMachine.get_tag('next' , mapping)
        
        
        def __init__(self, condition, do, nextt):
            self.condition = condition
            self.do = do
            self.next = nextt
    
    class State(object):
    
        '''turns the state into a map of maps'''
        def toTree(self):
            result = {}
            StateMachine.add_tag('name',self.name,result)
            StateMachine.add_tag('enter',self.enter,result)
            StateMachine.add_tag('loop',self.loop,result)
            StateMachine.add_tag('bye',self.bye,result)
            trans = []
            for transition in self.transitions :
                trans.append(transition.toTree())
            if trans :
                result['transitions'] = trans
            return result

        '''build the state from a map of maps'''
        def from_tree(self, mapping):
            self.name = StateMachine.get_tag('name' , mapping)
            self.enter = StateMachine.get_tag('enter' , mapping)
            self.loop = StateMachine.get_tag('loop' , mapping)
            self.bye = StateMachine.get_tag('bye' , mapping)
            self.transitions = []
            if mapping.__contains__('transitions'):
                transitions = mapping['transitions']
                for transitionDesc in transitions :
                    transition = StateMachine.Transition("","","")
                    transition.from_tree(transitionDesc)
                    self.transitions.append(transition)
        
        def __init__(self, name, enter, loop, bye):
            self.name = name
            self.enter = enter
            self.loop = loop
            self.bye = bye
            self.transitions = []
        
    '''turns the machine description into a map of maps'''
    def toTree(self):
        result = {}
        StateMachine.add_tag('name',self.name,result)
        StateMachine.add_tag('description',self.description,result)
        StateMachine.add_tag('frequency',self.frequency,result)
        StateMachine.add_tag('preamble',self.preamble,result)
        StateMachine.add_tag('functions',self.functions,result)
        states = []
        for state in self.states :
            states.append(state.toTree())
        result['states'] = states
        return { 'machine' : result  }

    '''builds a machine description from a map'''
    def from_tree(self, mapping):
        mapping = mapping['machine']
        self.name = StateMachine.get_tag('name' , mapping)
        self.functions = StateMachine.get_tag('functions' , mapping)
        self.description = StateMachine.get_tag('description' , mapping)
        self.frequency = StateMachine.get_tag('frequency' , mapping)
        self.preamble = StateMachine.get_tag('preamble' , mapping)
        self.states = []
        states = mapping['states']
        if states :
            for stateDesc in states:
                state = StateMachine.State("","","","")
                state.from_tree(stateDesc)
                self.states.append(state)
 
        
    '''turns the machine description to python code'''
    def to_python(self, filename = "", generate_main = True):            
        result = ""
        
        result += "\n\nfrom repeated_task import RepeatedTask\n\n"

        result += self.preamble
 
        result += "global " + self.name + "\n"
            
        result += "\n\n'''" + str(self.description) + "'''\n\n"

        result += "class " + self.name + "(RepeatedTask):\n"
        
        result += "    class Status:\n"
        result += "        (Playing, Suspended, Stopped) = range(0,3)\n"

        result +=\
'''
    def play(self, threaded = True):
        if self.status == self.Status.Stopped :
            print("Starting machine \'" + self.name+"\'")
            globals()[self.name] = self
            self.set_state("Initial")
            self.status = self.Status.Playing
            if threaded : RepeatedTask.start(self)
        elif self.status == self.Status.Suspended :
            if self.debug : print("Resuming machine \'"+ self.name+"\'")
            self.status = self.Status.Playing
            
    def set_state(self, state):
        if state is not self.state:
            self.bye()
            self.state = state
            self.enter()
        self.state = state
        
    def stop(self, threaded = True):
        if self.debug : print("Stopping machine \'"+ self.name+"\'")
        self.status = self.Status.Stopped
        RepeatedTask.cancel(self)

    def step(self):
        try:
            if self.debug : print("Stepping machine \'"+ self.name+"\' with status " + str(self.status))
            if self.status == self.Status.Playing :
                self.loop()
                #print("Transition of machine " + self.name + " in state "+ str(self.state))
                self.set_state(self.transition())
                if self.state == "Final" :
                    if self.debug : print("Machine has reached its final state\'"+ self.name+"\'")
                    self.bye()
                    self.stop()
        except Exception as e:
            print("Exception in machine "+ self.name + ": "+ str(e))
            pass
'''
        result += \
'''
    def __init__(self, verbose=False):
'''

        result += "        RepeatedTask.__init__(self,1,self.step)\n"
        result += "        self.name = \"" + self.name + "\"\n"
        result += "        self.frequency = "+ str(self.frequency) + "\n"
        result += "        self.status = self.Status.Stopped \n"
        result += "        self.state = None\n"
        result += "        self.debug = False\n"

        result += \
'''
    def enter(self):
'''
        if_pref = ""
        for state in self.states:
            if state.enter is not "":
                result += self.indent(if_pref + "if self.state == \"" + state.name + "\" :\n",2)
                result += self.indent(state.enter,3)
                if_pref = "el"
        if not if_pref: result += self.indent("pass\n\n",2)

        result += \
'''
    def bye(self):
'''
        if_pref = ""
        for state in self.states:
            if state.bye:
                result += self.indent(if_pref + "if self.state == \"" + state.name + "\" :\n",2)
                result += self.indent(state.bye,3)
                if_pref = "el"
        if not if_pref: result += self.indent("pass\n\n",2)
                
        result += \
'''
    def loop(self):
'''
        if_pref = ""
        for state in self.states:
            if state.loop:
                result += self.indent(if_pref + "if self.state == \"" + state.name + "\" :\n",2)
                result += self.indent(state.loop,3)
                if_pref = "el"
        if not if_pref: result += self.indent("pass\n\n",2)

        result += \
'''
    def transition(self):
'''
        if_pref = ""
        for state in self.states:
            if state.transitions:
                result += self.indent(if_pref + "if self.state == \"" + state.name + "\" :\n",2)
                if_pref2 = ""
                for transition in state.transitions:
                    indentl = 3
                    if transition.condition:
                        result += self.indent(if_pref2 + "if " + transition.condition + ":",indentl)
                        indentl = 4
                        if_pref2 = "el"
                    if transition.do:
                        result += self.indent(transition.do ,indentl)
                    result += self.indent("return \"" + transition.next +"\"",indentl)
                if_pref = "el"
        result += "        return self.state\n"
        
        result +="\n\n"

        if generate_main:
            result += "if __name__ == '__main__':\n"
            result += "    the_machine = " + self.name + "()\n"
            result += "    the_machine.play()\n"
            
        if filename != "":
            with open(filename,'w') as python_stream:
                python_stream.write( result )

        return result
        
    '''turns the machine description into a machine'''
    def to_machine(self):
        
        python_code = self.to_python()
        print(python_code)
        d = dict(locals(), **globals())
        exec(python_code ,d , d)
        exec(self.name + "=" + self.name +"Class()" ,d , d)
        exec("global " + self.name ,d , d)
        return eval(self.name ,d , d)
    
    '''add #nb indentations before each line of the text #text" and returns the result'''
    def indent(self, text,nb):
        result = ""
        flines = text.splitlines()
        for line in flines:
            result += nb * "    " + line + "\n"
        return result
        
         
    

class DiceMachineDescription(StateMachine):    
    def __init__(self):
        StateMachine.__init__(self)
        self.name = "TheDiceMachine"
        self.description = "\nSimulates a game where we throw a dice and win iff we make a six in less than n tries"
        self.frequency = 10
        self.preamble = \
'''
import random
'''

        self.functions = \
'''
def throw_dice(self):
    print('Throwing dice')
    self.dice = random.randint(1,6)
    self.tries += 1
    print('Dice is ', self.dice)
'''

        Initial = StateMachine.State("Initial",\
"""self.tries = 0
self.dice = 0""" , "self.throw_dice()","")
        Initial.transitions.append(StateMachine.Transition("self.tries>5","self.tries=0","Lost"))
        Initial.transitions.append(StateMachine.Transition("self.dice==6","","Won"))
        
        Lost = StateMachine.State("Lost", "", "print(\"You've lost\")", "print(\"Game Over\")")
        Lost.transitions.append(StateMachine.Transition("","","Final"))
        
        Won = StateMachine.State("Won", "", "print(\"You've won\")", "print(\"Game Over\")")
        Won.transitions.append(StateMachine.Transition("","","Final"))
        
        Final = StateMachine.State("Final", "", "print('Over, dice is ', self.dice)", "")
        
        self.states = [Initial,Lost, Won, Final]
        
        
        


