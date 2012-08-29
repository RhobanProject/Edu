'''
Created on 16 ao�t 2012

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
        condition = ""
        do = ""
        next = ""

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
        name = ""
        enter = ""
        loop = ""
        transitions = []
    
        def toTree(self):
            result = {}
            addToMappingIfNonEmpty('name',self.name,result)
            addToMappingIfNonEmpty('enter',self.enter,result)
            addToMappingIfNonEmpty('loop',self.loop,result)
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
            self.transitions = []
            transitions = mapping['transitions']
            if transitions :
                for transitionDesc in transitions :
                    transition = StateMachineDescription.Transition("","","")
                    transition.from_tree(transitionDesc)
                    self.transitions.append(transition)
        
        def __init__(self, name, enter, loop):
            self.name = name
            self.enter = enter
            self.loop = loop
    
    '''The name of the machine'''
    name = ""
    
    '''Adescription of its behaviour'''
    description = ""
    
    '''The premable and fucntions definition'''
    preamble = ""
    
    '''The list of states'''
    states = {}
    
    '''The list of transitions'''
    transitions = {}
    
    '''Thre frequency of the machine'''
    frequency = 50
    
    def toTree(self):
        result = {}
        addToMappingIfNonEmpty('name',self.name,result)
        addToMappingIfNonEmpty('description',self.description,result)
        addToMappingIfNonEmpty('frequency',self.frequency,result)
        addToMappingIfNonEmpty('preamble',self.preamble,result)
        states = []
        for state in self.states :
            states.append(state.toTree())
        result['states'] = states
        return { 'machine' : result  }

    def from_tree(self, mapping):
        mapping = mapping['machine']
        self.name = getFromMapping('name' , mapping)
        self.description = getFromMapping('description' , mapping)
        self.frequency = getFromMapping('frequency' , mapping)
        self.preamble = getFromMapping('preamble' , mapping)
        self.states = []
        states = mapping['states']
        if states :
            for stateDesc in states:
                state = StateMachineDescription.State("","","")
                state.from_tree(stateDesc)
                self.states.append(state)
 
    def __init__(self):
        '''
        Constructor
        '''
        

class DiceMachineDescription(StateMachineDescription):    
        name = "TheDiceMachine";
        description = "\nSimulates a game where we throw a dice and win iff we make a six in less than n tries"
        preamble = "\nimport random\nvoid throw_dice() { dice = 7 * rand() / RAND_MAX; }";

        Initial = StateMachineDescription.State("Initial","tries = 0", "\ntries+=1\nthrow_dice()")
        Initial.transitions.append(StateMachineDescription.Transition("i>5","i=0","Lost"))
        Initial.transitions.append(StateMachineDescription.Transition("dice==6","","Won"))
        
        Lost = StateMachineDescription.State("Lost", "print(\"You've lost\")", "print(\"Game Over\")")
        Lost.transitions.append(StateMachineDescription.Transition("","","Final"))
        
        Won = StateMachineDescription.State("Won", "print(\"You've won\")", "print(\"Game Over\")" )
        Won.transitions.append(StateMachineDescription.Transition("","","Final"))
        
        Final = StateMachineDescription.State("", "", "")
        
        states = [Initial,Lost, Won, Final]

    
