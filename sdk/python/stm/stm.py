'''
Created on 16 aout 2012

@author: Hugo
'''
from timer import RepeatedTimer
from yaml import load as yamlload

class StateMachine(RepeatedTimer):
    name = ""
    
    class States:
        (Initial , Final) = range(0,2)

    class Status:
        (Playing, Suspended, Stopped) = range(0,3)
        
    def __init__(self, debug=False):
        RepeatedTimer.__init__(self,1,self.step)
        self.frequency = 1.0
        self.state = self.States.Initial
        self.status = self.Status.Stopped
        self.debug = debug
        
    def get_frequency(self):
        return self.__frequency

    def set_frequency(self, frequency):
        if frequency > 0.001 and frequency < 500 :
            self.__frequency = frequency
            self.interval = 1.0 / frequency

    frequency = property(get_frequency, set_frequency) 

    def set_state(self, state):
        self.state = state
        
    def play(self):
        if self.status == self.Status.Stopped :
            if self.debug : print("Starting machine \'" + self.name+"\'")
            self.state = self.States.Initial
            self.enter()
            self.status = self.Status.Playing
            RepeatedTimer.start(self)
        elif self.status == self.Status.Suspended :
            if self.debug : print("Resuming machine \'"+ self.name+"\'")
            self.status = self.Status.Playing
    
    def suspend(self):
        if self.debug : print("Suspending machine \'"+ self.name+"\'")
        self.status = self.Status.Suspended

    def stop(self):
        if self.debug : print("Stopping machine \'"+ self.name+"\'")
        self.status = self.Status.Stopped
        RepeatedTimer.cancel(self)
    
    def step(self):
        if self.debug : print("Stepping machine \'"+ self.name+"\' with status " + str(self.status))
        if self.status == self.Status.Playing :
            self.loop()
            newstate = self.transition()
            if newstate != self.state :
                self.bye()
                self.state = newstate
                self.enter()
            if self.state is self.States.Final :
                if self.debug : print("Machine has reached its final state\'"+ self.name+"\'")
                self.bye()
                self.stop()

    '''builds the machine from a yaml file'''
    @classmethod
    def from_file(cls, filename):
        description = StateMachineDescription.from_file(filename)
        return description.toMachine()


    
class StateMachineDescription(object):
    
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
            StateMachineDescription.add_tag('condition',self.condition,result)
            StateMachineDescription.add_tag('do',self.do,result)
            StateMachineDescription.add_tag('next',self.next,result)
            return result

        '''build the transition from a map'''
        def from_tree(self, mapping):
            self.condition = StateMachineDescription.get_tag('condition' , mapping)
            self.do = StateMachineDescription.get_tag('do' , mapping)
            self.next = StateMachineDescription.get_tag('next' , mapping)
        
        
        def __init__(self, condition, do, nextt):
            self.condition = condition
            self.do = do
            self.next = nextt
    
    class State(object):
    
        '''turns the state into a map of maps'''
        def toTree(self):
            result = {}
            StateMachineDescription.add_tag('name',self.name,result)
            StateMachineDescription.add_tag('enter',self.enter,result)
            StateMachineDescription.add_tag('loop',self.loop,result)
            StateMachineDescription.add_tag('bye',self.bye,result)
            trans = []
            for transition in self.transitions :
                trans.append(transition.toTree())
            if trans :
                result['transitions'] = trans
            return result

        '''build the state from a map of maps'''
        def from_tree(self, mapping):
            self.name = StateMachineDescription.get_tag('name' , mapping)
            self.enter = StateMachineDescription.get_tag('enter' , mapping)
            self.loop = StateMachineDescription.get_tag('loop' , mapping)
            self.bye = StateMachineDescription.get_tag('bye' , mapping)
            self.transitions = []
            if mapping.__contains__('transitions'):
                transitions = mapping['transitions']
                for transitionDesc in transitions :
                    transition = StateMachineDescription.Transition("","","")
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
        StateMachineDescription.add_tag('name',self.name,result)
        StateMachineDescription.add_tag('description',self.description,result)
        StateMachineDescription.add_tag('frequency',self.frequency,result)
        StateMachineDescription.add_tag('preamble',self.preamble,result)
        StateMachineDescription.add_tag('functions',self.functions,result)
        states = []
        for state in self.states :
            states.append(state.toTree())
        result['states'] = states
        return { 'machine' : result  }

    '''builds the machine description from a map'''
    def from_tree(self, mapping):
        mapping = mapping['machine']
        self.name = StateMachineDescription.get_tag('name' , mapping)
        self.functions = StateMachineDescription.get_tag('functions' , mapping)
        self.description = StateMachineDescription.get_tag('description' , mapping)
        self.frequency = StateMachineDescription.get_tag('frequency' , mapping)
        self.preamble = StateMachineDescription.get_tag('preamble' , mapping)
        self.states = []
        states = mapping['states']
        if states :
            for stateDesc in states:
                state = StateMachineDescription.State("","","","")
                state.from_tree(stateDesc)
                self.states.append(state)
 
    '''builds the machine description from a yaml file'''
    @classmethod
    def from_file(cls, filename):
        with open(filename,'r') as yaml_stream:
            tree = yamlload(yaml_stream)
            description = StateMachineDescription()
            description.from_tree(tree)
            return description
                

    '''turns the machine description to python code'''
    def toPython(self):
        result = ""
        
        result += "\n\nfrom stm import StateMachine\n\n"

        for line in self.preamble.splitlines() :
            imports = line.split(' ')
            if imports[0]=='import':
                result += "global " + imports[1] + "\n"
            elif imports[0]=='from' and imports[2]=='import' :
                result += "global " + imports[3] + "\n"
            result += line + "\n"
        
        result += "class " + self.name + "(StateMachine):\n"
        
        result += \
'''
    def __init__(self, verbose=False):
'''
        result += "        StateMachine.__init__(self, verbose)\n"
        result += "        self.name = \"" + self.name + "\"\n"
        result += "        self.frequency = "+ str(self.frequency) + "\n"
        
        result += "\n"
        
        result += self.indent(self.functions, 1)
        
        result += "    class States:\n"
        result += "        ("
        for state in self.states:
            result += state.name + ", "
        result += ") = range(0," + str(len(self.states)) + ")\n\n"
        
        
        result += \
'''
    def enter(self):
'''
        for state in self.states:
            if state.enter is not "":
                result += "        if self.state == self.States." + state.name + " :\n"
                result += self.indent(state.enter,3)

        result +="\n\n"

        result += \
'''
    def bye(self):
'''
        for state in self.states:
            if state.bye is not "":
                result += "        if self.state == self.States." + state.name + " :\n"
                result += self.indent(state.bye,3)

        result +="\n\n"
                
        result += \
'''
    def loop(self):
'''
        for state in self.states:
            if state.loop is not "":
                result += "        if self.state == self.States." + state.name + " :\n"
                result += self.indent(state.loop,3)

        result +="\n\n"

        result += \
'''
    def transition(self):
'''
        for state in self.states:
            if state.transitions:
                result += "        if self.state == self.States." + state.name + " :\n"
                for transition in state.transitions:
                    indentl = 3
                    if transition.condition:
                        result += self.indent("if " + transition.condition + ":",indentl)
                        indentl = 4
                    if transition.do:
                        result += self.indent(transition.do ,indentl)
                    result += self.indent("return self.States." + transition.next ,indentl)
        result += "        return self.state\n"


        result +="\n\n"

        result += "if __name__ == '__main__':\n    the_machine = " + self.name + "()\n    the_machine.play()\n"

    
        return result
        
    '''turns the machine description into a machine'''
    def toMachine(self):
        
        exec(self.toPython())
                    
        machine = eval( self.name + "()")
        return machine
    
    def imports(self):
        result = []
        for line in self.preamble.splitlines() :
            imports = line.split(' ')
            if imports[0]=='import':
                result.append(imports[1])
        return result
    
    '''add #nb indentations before each line of the text #text" and returns the result'''
    def indent(self, text,nb):
        result = ""
        flines = text.splitlines()
        for line in flines:
            result += nb * "    " + line + "\n"
        return result
        
    def __init__(self):
        '''
        Constructor
        '''
        '''The name of the machine'''
        self.name = ""
    
        '''Adescription of its behaviour'''
        self.description = ""
    
        '''The preamble definition'''
        self.preamble = ""

        '''The functions definition'''
        self.functions = ""
    
        '''The list of states'''
        self.states = {}
    
        '''The list of transitions'''
        self.transitions = {}
    
        '''The frequency of the machine'''
        self.frequency = 1
        
    

class DiceMachineDescription(StateMachineDescription):    
    def __init__(self):
        StateMachineDescription.__init__(self)
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

        Initial = StateMachineDescription.State("Initial",\
"""self.tries = 0
self.dice = 0""" , "self.throw_dice()","")
        Initial.transitions.append(StateMachineDescription.Transition("self.tries>5","self.tries=0","Lost"))
        Initial.transitions.append(StateMachineDescription.Transition("self.dice==6","","Won"))
        
        Lost = StateMachineDescription.State("Lost", "", "print(\"You've lost\")", "print(\"Game Over\")")
        Lost.transitions.append(StateMachineDescription.Transition("","","Final"))
        
        Won = StateMachineDescription.State("Won", "", "print(\"You've won\")", "print(\"Game Over\")")
        Won.transitions.append(StateMachineDescription.Transition("","","Final"))
        
        Final = StateMachineDescription.State("Final", "", "print('Over, dice is ', self.dice)", "")
        
        self.states = [Initial,Lost, Won, Final]
        
        
        


