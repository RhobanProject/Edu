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
from yaml import load as load
from xml.dom import minidom
from importlib import import_module
from time import time
import datetime

'''
The straightforward way to create a StateMachine is from yaml files (see the examples folder)
          my_stm = StateMachine.from_yaml("my_stm.yaml")[0]

There are three ways to use a StateMachine
1) as a standalone thread
          my_stm.play(duration,True)
  (as well as my_stm.join() and my_stm.stop())
2) generating python code
          StateMachine.yaml_to_py("my_stm.yaml","my_stm.py")
3) using the stm loader, which allows to animate multiple stms with a single thread
   this is the best way to run mutliple stms that interact with each other.
   For example:
   
          loader = StateMachineLoader()
          my_stms = StateMachine.from_yaml("my_stm.yaml")
          my_stms2 = StateMachine.from_yaml("my_stm2.yaml")
          my_stm = StateMachine.from_yaml("my_stm3.yaml")[0]
          
          loader.load_machines(my_stms , False, 5.0)
          loader.load_machines(my_stms2, True)
          loader.load_machine(my_stms)

   will 
      * load and execute the machines from the my_stm2.yaml file for 5 seconds
      * then immediately load and execute the machines from the my_stms2.yaml file,
              and wait for all of these machines to reach their final state
      * then load and execute the first machine from the my_stm3.yaml file 
'''

class StateMachine(RepeatedTask):
    
    class Status:
        (Playing, Suspended, Stopped) = range(0,3)
        
    '''
    Constructor
    '''
    def __init__(self, debug=False):
        RepeatedTask.__init__(self,1,self.step)

        '''The name of the machine'''
        self.name = ""
        
        '''A description of its behaviour'''
        self.description = ""
        
        '''The frequency of the machine'''
        self.frequency = 1.0
        
        '''The preamble'''
        self.preamble = ""

        '''The code executed on stop'''
        self.onstop = ""
        
        '''The dict of states (name-> state)'''
        self.states = {}
        self.states['Initial'] = StateMachine.State("Initial")
        self.states['Final'] = StateMachine.State("Final")
         
        '''The dict of submachines'''
        self.submachines = {}
        
        '''The current state'''
        self.state = None
        
        '''The playing status of the machine'''
        self.status = self.Status.Stopped

        '''The error status of the machine'''
        self.error = ""

        '''The date where playing begins'''
        self.begin = time()
        
        '''The total playing duration'''
        self.duration = float("inf")
        
        '''The map of monitored moves to the map from states to output values'''
        self.monitored_states = {}

        self.debug = debug
        
        '''True if the machine is animated by a thread, falseif anaimated by calls to step (by an stm loader for example)'''
        self.threaded = False
        
        self.globals = globals()
        self.locals = locals()
        
        '''Minimal time to spend in the current state'''
        self.min_ticks = 0
              
    def statusString(self):
        status = ''

        if self.status == self.Status.Playing:
            status = 'Playing'
        elif self.status == self.Status.Stopped:
            status = 'Stopped'
        elif self.status == self.Status.Suspended:
            status = 'Suspended'

        if self.state == None:
            status += ' (Initial)'
        else:
            status += ' ('+ self.state.name+')'

        return status
        
    '''Plays the machine for the given duration
    optionally waits for the machine to be stopped before returning
    last option is used by the stm_loader to prevent creation of too many threads'''
    def play(self, wait_stopped = False, duration = float("inf"), threaded = True):
        try :
            self.lock.acquire()
            for submachine in self.submachines.values() :
                submachine.play(False, duration, threaded)
            self.begin = time()
            self.duration = duration
            self.error = ""
            if self.status == self.Status.Stopped :
                if self.debug: print("Starting machine \'" + self.name+"\'")
                self.state = None
                globals()[self.name] = self
                if self.preamble:
                    try :
                        exec(self.preamble, self.globals, self.locals)
                    except Exception as e:
                        self.error = "[" + str(datetime.datetime.now().time()) + "] " + " Failed to exec preamble : " + str(e)
                        print("Failed to exec preamble of machine "+ self.name + ": "+ str(e))
                        raise Exception(self.error)
                        
                for line in self.preamble.splitlines() :
                    imports = line.split(' ')
                    if len(imports) >= 2:
#                        if imports[0]=='def':
#                            globals()[imports[1][:-3]] = locals()[imports[1][:-3]]
                        if imports[0]=='import':
                            import_module(imports[1])
                        elif imports[0]=='from' and imports[2]=='import' :
                            if len(imports) == 4:
                                globals()[imports[3]] = getattr(__import__(imports[1]), imports[3])
                            elif len(imports) == 6 and imports[4]=='as':
                                globals()[imports[5]] = getattr(__import__(imports[1]), imports[3]) 
    
                self.set_state("Initial")
                self.status = self.Status.Playing
                if threaded:
                    RepeatedTask.start(self)
                self.threaded = threaded
                    
            elif self.status == self.Status.Suspended :
                if self.debug : print("Resuming machine \'"+ self.name+"\'")
                self.status = self.Status.Playing
            self.lock.release()
            
        except Exception as e:
            self.error = "[" + str(datetime.datetime.now().time()) + "] " + " Failed to step machine : " + str(e)
            print("Failed to step machine "+ self.name + ": "+ str(e))
            self.lock.release()
            
        if wait_stopped:
            self.wait_stopped()


    '''Suspends the machine execution, use play() to resume'''            
    def suspend(self):
        self.lock.acquire()
        if self.debug : print("Suspending machine \'"+ self.name+"\'")
        self.status = self.Status.Suspended
        self.lock.release()

    '''Stops the machine execution, and stops the thread as well'''            
    def stop(self):
        if self.status == self.Status.Playing or self.status == self.Status.Suspended :
           if self.debug : print("Stopping machine \'"+ self.name+"\'")
           if self.threaded:
               RepeatedTask.cancel(self)
           self.status = self.Status.Stopped
           for submachine in self.submachines.values():
               submachine.stop()
           if self.onstop:
               try :
                  exec(self.onstop, self.globals, self.locals)
               except Exception as e:
                  self.error = "[" + str(datetime.datetime.now().time()) + "] " + " Failed to exec onstop : " + str(e)
                  print("Failed to exec onstop of machine "+ self.name + ": "+ str(e))
                  raise Exception(self.error)
           self.state = self.states["Initial"]
       
    '''creates a list of machines from a yaml file'''
    @classmethod
    def from_yaml(cls, filename):
        with open(filename,'r') as yaml_stream:
            tree = load(yaml_stream)
            return StateMachine.from_tree(tree)
  
    '''creates a list of machines from an xml file'''
    @classmethod
    def from_xml(cls, filename):
        xmldoc = minidom.parse(filename)
        return StateMachine.from_xml_doc(xmldoc)

    '''creates a list of machines from an xml stream'''
    @classmethod
    def from_xml_stream(cls, stream):
        print(stream)
        xmldoc = minidom.parseString(stream)
        return StateMachine.from_xml_doc(xmldoc)

    '''creates a list of machines from an xml doc'''
    @classmethod
    def from_xml_doc(cls, xmldoc):
        
        def buildtree(rootNode):
            if len(rootNode.childNodes) == 0:
                return ""
            elif len(rootNode.childNodes) == 1 and rootNode.firstChild.nodeType == rootNode.firstChild.TEXT_NODE:
                return rootNode.firstChild.nodeValue
            else:
                result = {}
                n = 0
                for node in rootNode.childNodes:
                    if node.nodeType != node.TEXT_NODE:
                        if node.nodeName in result.keys():
                            n = n + 1
                            result[node.nodeName + "#" + str(n)] = buildtree(node)
                        else:
                            result[node.nodeName] = buildtree(node)
                return result
            

        tree = buildtree(xmldoc)
        return StateMachine.from_tree(tree['Stm'])
    
    
       
    '''builds an executable python script from a yaml file'''
    @classmethod
    def yaml_to_py(cls, yaml_file, py_file):
        machine = StateMachine.from_yaml(yaml_file)
        with open(py_file,'w') as output:
            output.write( machine.to_python() )
            
    '''builds an executable python script from an xml file'''
    @classmethod
    def xml_to_py(cls, xml_file, py_file):
        machine = StateMachine.from_xml(xml_file)
        with open(py_file,'w') as output:
            output.write( machine.to_python() )
  
    '''Changes the machine state'''
    def set_state(self, state_name):
        try :
            self.lock.acquire()
            state = self.states[state_name]
            if state is not self.state:
                if self.state and self.state.bye : exec(self.state.bye, self.globals, self.locals)
                if state and state.enter : exec(state.enter, self.globals, self.locals)
            self.state = state
            self.lock.release()
        except Exception as e:
            self.lock.release()
            raise e
    
    '''sets a machine variable'''
    def set_attributes(self, keys, values):
        if len(keys) != len(values):
            raise Exception("Cannot set variables: not same number of keys and values")
        for i in range(len(keys)):
            exec("self."+ keys[i] + "=" + str(values[i]), self.globals, self.locals)
        
    '''gets a machine variable'''
    def get_attributes(self, keys):
        values = []
        for key in keys:
            values.append( float( eval("self."+ key, self.globals, self.locals) ) )
        return values

    '''try to use connection to find attribute
    def __getattr__(self, name):
        return self.connection.name'''

    '''evaluates a n expression in the context of a machine'''
    def evaluate(self, expression):
        return float( eval( expression, self.globals, self.locals) )

    '''performs one step of the stm'''
    def step(self):
        try:
            self.lock.acquire()
            if self.status == self.Status.Playing :
                try:
                    if self.state and self.state.loop :
                        if self.min_ticks > 0 :
                           self.min_ticks = self.min_ticks - 1
                        else :
                            exec(self.state.loop, self.globals, self.locals)
                except Exception as e:
                    raise Exception("Failed to loop in state '" + self.state.name + "': " + str(e))
                if self.debug: print("Transition of machine " + self.name + " in state "+ self.state.name)
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
            self.error = "[" + str(datetime.datetime.now().time()) + "] " + str(e)
            print("Exception in machine "+ self.name + ": "+ str(e))
            pass
        #raise e

    '''adds the state from a given move to the list of monitored states'''
    def monitor(self, move, state):
        if not move in self.monitored_states:
            self.monitored_states[move] = {}
        if not state in self.monitored_states[move]:
            self.monitored_states[move][state] = None
        return self.monitored_states[move][state]


    ''' Performs the transition and returns the name of the new state'''
    def transition(self):
        try:  
            for transition in self.state.transitions:
                if not transition.condition or eval(transition.condition, self.globals, self.locals):
                    if transition.do :
                        exec(transition.do, self.globals, self.locals)
                    return transition.next
        except Exception as e:
            raise Exception("Failed to compute transition in state '" + self.state.name + "': " + str(e))

        return self.state.name
                  
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
            StateMachine.add_tag('fire',self.do,result)
            StateMachine.add_tag('next',self.next,result)
            return result

        '''build the transition from a map'''
        def from_tree(self, mapping):
            self.condition = StateMachine.get_tag('condition' , mapping)
            self.do = StateMachine.get_tag('fire' , mapping)
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
            print("Added state " + self.name + " from xml")
            self.enter = StateMachine.get_tag('enter' , mapping)
            self.loop = StateMachine.get_tag('loop' , mapping)
            self.bye = StateMachine.get_tag('bye' , mapping)
            self.transitions = []
            if 'transitions' in mapping.keys() and mapping['transitions']:
                print("Mapping contains transitions")
                for transitionDesc in mapping['transitions'].values():
                    transition = StateMachine.Transition("","","")
                    transition.from_tree(transitionDesc)
                    self.transitions.append(transition)
            else:
                print("Found no transitions")
                
        
        def __init__(self, name = "", enter = "", loop = "", bye = ""):
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
        StateMachine.add_tag('onstop',self.onstop,result)
        states = []
        for state in self.states.values() :
            states.append(state.toTree())
        result['states'] = states
        submachines = []
        for submachine in self.submachines.values() :
            submachines.append(submachine.toTree())
        result['submachines'] = submachines
        return { 'machine' : result  }

    '''builds a machine description from a map
    if the second argument is None the machine is build from scratch
    otherwise the data in the tree is appended to the machine (used for cloning)
    '''
    @classmethod
    def from_tree(cls, mapping, machine = None):
        if machine == None : machine = StateMachine()
        machine.name = StateMachine.get_tag('name' , mapping)
        machine.description += StateMachine.get_tag('description' , mapping)
        if mapping.__contains__('frequency') :
            machine.frequency = float(StateMachine.get_tag('frequency' , mapping))
        machine.preamble += StateMachine.get_tag('preamble' , mapping)
        machine.onstop += StateMachine.get_tag('onstop' , mapping)

        if mapping.__contains__('states') :
            states = mapping['states']
            print("Found " + str(len(states)) + " states")
            for stateDesc in states.values():
                state = StateMachine.State()
                state.from_tree(stateDesc)
                machine.states[state.name] = state
                print("Added state " + state.name)
            
        if mapping.__contains__('submachines') :
            submachines = mapping['submachines']
            for subtree in submachines:
                instance_nb = StateMachine.get_tag('instances' , subtree)
                if instance_nb:
                    nb = int(instance_nb)
                    globals()[subtree['name']] = {}
                else:
                    nb = 1
                for index in range(1,nb+1) :
                    '''We first check whether the submachine extends another'''
                    clone_name = StateMachine.get_tag('extends' , subtree)
                    if clone_name :
                        clone_tree = None
                        for tree in submachines :
                            if tree['name'] == clone_name : clone_tree = tree
                        clone = StateMachine.from_tree(clone_tree)
                        submachine = StateMachine.from_tree(subtree, clone)
                    else :
                        submachine = StateMachine.from_tree(subtree)
                    if instance_nb:
                        submachine.name += "["+ str(index) + "]"
                        submachine.index = index
                        globals()[subtree['name']][index] = submachine
                    machine.submachines[submachine.name] = submachine
                    print("Created submachine " + submachine.name)
        return machine

    '''
       Create a GraphVIZ representation 
    '''
    def to_graphviz(self):
        dot = "digraph %s {\n" % self.name
        dot+= "    label=\"%s\";\n" % self.name

        def escape(s):
            s = "\\n".join(filter(lambda x: not x.startswith('print'), s.split("\n")))
            return s.replace("\"", "\\\"")

        for state in self.states:
            dot += "    %s;\n" % state.name

            if escape(state.loop):
                dot += "    %s -> %s [label=\"%s\"];\n" % (state.name, state.name, escape(state.loop))

            for transition in state.transitions:
                dot += "    %s -> %s [label=\"%s\"];\n" % (state.name, transition.next, escape(transition.condition))


        dot += "};\n"
        return dot
 
    '''turns the machine description to python code'''
    def to_python(self, filename = "", only_core_code = False):            
        result = ""
        
        if not only_core_code:
            result += "\n\nfrom repeated_task import RepeatedTask\n\n"
            result += self.preamble
            result += "global " + self.name + "\n"
        
        for submachine in self.submachines.values():
            result += submachine.preamble + "\n"
        for submachine in self.submachines.values():
            result += "global " + submachine.name + "\n"

        result += "\n\n'''" + str(self.description) + "'''\n\n"


        if not only_core_code:
            result += "class StateMachine(RepeatedTask):\n"
            result += "    class Status:\n"
            result += "        (Playing, Suspended, Stopped) = range(0,3)\n"
            result +=\
'''
    def play(self):
        for submachine in self.submachines.values():
            submachine.play() 
        if self.status == self.Status.Stopped :
            print("Starting machine \'" + self.name+"\'")
            self.set_state("Initial")
            self.status = self.Status.Playing
            RepeatedTask.start(self)
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
        for submachine in self.submachines.values():
            submachine.stop() 
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
        result += "\n\nclass " + self.name + "Class(StateMachine):\n"
        result += \
'''
    def __init__(self, verbose=False):
'''

        for submachine in self.submachines.values():
            result += self.indent("global " + submachine.name, 2)
        result += self.indent("RepeatedTask.__init__(self,1,self.step)\n",2)
        result += self.indent("self.name = \"" + self.name + "\"\n",2)
        result += self.indent("self.frequency = "+ str(self.frequency) + "\n",2)
        result += self.indent("self.status = self.Status.Stopped \n",2)
        result += self.indent("self.state = None\n",2)
        result += self.indent("self.debug = False\n",2)
        result += self.indent("self.submachines = {}",2)
        for submachine in self.submachines.values():
            result += self.indent(submachine.name + " = " + submachine.name + "Class()\n", 2)
            result += self.indent("self.submachines['" +submachine.name + "'] = " + submachine.name,2)

        result += \
'''
    def enter(self):
'''
        if_pref = ""
        for state in self.states.values():
            if state.enter is not "":
                result += self.indent(if_pref + "if self.state == \"" + state.name + "\" :\n",2)
                result += self.indent(state.enter,3)
                if_pref = "el"
        if not if_pref: result += self.indent("pass\n",2)

        result += \
'''
    def bye(self):
'''
        if_pref = ""
        for state in self.states.values():
            if state.bye:
                result += self.indent(if_pref + "if self.state == \"" + state.name + "\" :\n",2)
                result += self.indent(state.bye,3)
                if_pref = "el"
        if not if_pref: result += self.indent("pass\n",2)
                
        result += \
'''
    def loop(self):
'''
        if_pref = ""
        for state in self.states.values():
            if state.loop:
                result += self.indent(if_pref + "if self.state == \"" + state.name + "\" :\n",2)
                result += self.indent(state.loop,3)
                if_pref = "el"
        if not if_pref: result += self.indent("pass\n",2)

        result += \
'''
    def transition(self):
'''
        if_pref = ""
        for state in self.states.values():
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

        for submachine in self.submachines.values():
            result += submachine.to_python("",True)
            
        if not only_core_code:
            result += "if __name__ == '__main__':\n"
            result += "    " + self.name + " = " + self.name + "Class()\n"
            result += "    " + self.name + ".play()\n"
            
        if filename != "":
            with open(filename,'w') as python_stream:
                python_stream.write( result )

        return result
        
    '''add #nb indentations before each line of the text #text" and returns the result'''
    def indent(self, text,nb):
        result = ""
        flines = text.splitlines()
        for line in flines:
            result += nb * "    " + line + "\n"
        return result
