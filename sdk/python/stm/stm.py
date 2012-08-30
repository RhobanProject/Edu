'''
Created on 16 ao?t 2012

@author: Hugo
'''
import yaml
from collections import OrderedDict

class StateMachine(object):
    name = ""
    
    class States:
        (Initial , Final) = range(0,2)

    state = States.Initial
        
    def run(self):
        print("Starting machine \'" + self.name+"\'")
        self.state = self.States.Initial
        self.enter()
        while self.state is not self.States.Final :
            self.loop()
            newstate = self.transition()
            if newstate != self.state :
                self.bye()
                self.state = newstate
                self.enter()
        self.bye()
        print("Stopping machine \'"+ self.name+"\'")


'''If the chain is non empty, addToMappingIfNonEmpty associates the value chain to the key 'tag'
in the mapping 'mapping' '''
def addToMappingIfNonEmpty(tag,chain,mapping):
    if chain is not '' :
        mapping[tag] = chain
        
def getFromMapping(tag,mapping):
    if mapping.__contains__(tag)  :
        return mapping[tag]
    else :
        return ""
    
class StateMachineDescription(object):
         
    class Transition(object):

        def toTree(self):
            result = {}
            addToMappingIfNonEmpty('condition',self.condition,result)
            addToMappingIfNonEmpty('do',self.do,result)
            addToMappingIfNonEmpty('next',self.next,result)
            return result

        def from_tree(self, mapping):
            self.condition = getFromMapping('condition' , mapping)
            self.do = getFromMapping('do' , mapping)
            self.next = getFromMapping('next' , mapping)
        
        
        def __init__(self, condition, do, nextt):
            self.condition = condition
            self.do = do
            self.next = nextt
    
    class State(object):
    
        def toTree(self):
            result = {}
            addToMappingIfNonEmpty('name',self.name,result)
            addToMappingIfNonEmpty('enter',self.enter,result)
            addToMappingIfNonEmpty('loop',self.loop,result)
            addToMappingIfNonEmpty('bye',self.bye,result)
            trans = []
            for transition in self.transitions :
                trans.append(transition.toTree())
            if trans :
                result['transitions'] = trans
            return result

        def from_tree(self, mapping):
            self.name = getFromMapping('name' , mapping)
            self.enter = getFromMapping('enter' , mapping)
            self.loop = getFromMapping('loop' , mapping)
            self.bye = getFromMapping('bye' , mapping)
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
        
    def toTree(self):
        result = {}
        addToMappingIfNonEmpty('name',self.name,result)
        addToMappingIfNonEmpty('description',self.description,result)
        addToMappingIfNonEmpty('frequency',self.frequency,result)
        addToMappingIfNonEmpty('preamble',self.preamble,result)
        addToMappingIfNonEmpty('functions',self.functions,result)
        states = []
        for state in self.states :
            states.append(state.toTree())
        result['states'] = states
        return { 'machine' : result  }

    def from_tree(self, mapping):
        mapping = mapping['machine']
        self.name = getFromMapping('name' , mapping)
        self.functions = getFromMapping('functions' , mapping)
        self.description = getFromMapping('description' , mapping)
        self.frequency = getFromMapping('frequency' , mapping)
        self.preamble = getFromMapping('preamble' , mapping)
        self.states = []
        states = mapping['states']
        if states :
            for stateDesc in states:
                state = StateMachineDescription.State("","","","")
                state.from_tree(stateDesc)
                self.states.append(state)
 
    def toPython(self):
        result = ""
        result += "\n\nimport stm\n\n"
        
        result += self.preamble + "\n\n"
        
        result += "class " + self.name + "(stm.StateMachine):\n"
        result += "    name = \"" + self.name + "\"\n"
        
        result += "\n"
        
        result += self.indent(self.functions, 1)
        
        result += "    class States:\n"
        result += "        ("
        for state in self.states:
            result += state.name + ", "
        result += ") = range(0," + str(len(self.states)) + ")\n\n"
        
        result += self.indent(self.functions,1)
        
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

        result += "if __name__ == '__main__':\n    the_machine = " + self.name + "()\n    the_machine.run()\n"

    
        return result
        
    def indent(self, text,nb):
        result = ""
        flines = text.split('\n')
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
    
        '''Thre frequency of the machine'''
        self.frequency = 50
        
    

class DiceMachineDescription(StateMachineDescription):    
    def __init__(self):
        super( DiceMachineDescription, self ).__init__()
        self.name = "TheDiceMachine";
        self.description = "\nSimulates a game where we throw a dice and win iff we make a six in less than n tries"
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
        
        
        


