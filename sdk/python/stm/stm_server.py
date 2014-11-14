'''
Created on 27 janv. 2013

@author: Hugo
'''

from repeated_task import RepeatedTask
from stm_loader import StateMachineLoader
from threading import RLock
from stm import StateMachine
import rhoban.communication as com
from rhoban.communication import CommandsStore
import datetime

class StateMachineServer(StateMachineLoader):
    
    def __init__(self,  hostname = 'localhost', port = 12345, storeFileName = '../common/commands.xml'):

        print("Creating StmServer connected to " + hostname + ":" + str(port))
        
        self.hostname = hostname
        self.port = port

        self.callbacks = {'StmError' : self.error,
                           'StmLoadXMLMachine':self.processLoadXMLMachineMessage,
                           'StmStartMachine':self.processStartMachineMessage,
                           'StmStopMachine':self.processStopMachineMessage,
                           'StmKillMachine':self.processKillMachineMessage,
                           'StmGetState' : self.processGetStateMessage,
                           'StmGetStates' : self.processGetStatesMessage,
                           'StmGetMachinesInfo':self.processGetMachinesInfo,
                           'StmSetMachineAttributes':self.processSetMachineAttributes,
                           'StmGetMachineAttributes':self.processGetMachineAttributes,
                           'StmEvaluatePythonExpressions':self.processEvaluatePythonExpressions,
                           'StmPing':self.processPing,
                           'StmGetAndSetMachineAttributes':self.StmGetAndSetMachineAttributes
                           }
        
        self.store = com.CommandsStore()
        self.store.parseXml(storeFileName)
        self.pending_messages = []
        self.lock_messages = RLock()
        StateMachineLoader.__init__(self)


    def start(self):
        self.connection = com.Connection()
        self.connection.setStore(self.store)
        self.connection_keeper =  RepeatedTask(10,self.connectIfNeeded)
        self.connection_keeper.start()

    '''Asynchronous reception and processing of messages
       messages are stored in an array and processed when no machine is stepping
       this is necessary for machine to send and get messages without creating race conditions in the mailbox
    '''
        
    def incomingMessageProcessor(self, message):
        self.lock_messages.acquire()
        self.pending_messages.append(message)
        self.lock_messages.release()

    def processPendingMessages(self):
        self.lock_messages.acquire()
        for message in self.pending_messages:
            self.processMessage(message)
        self.pending_messages = []
        self.lock_messages.release()

    def step(self):
        StateMachineLoader.step(self)
        self.processPendingMessages()
        
    def connectIfNeeded(self):
        try :
            if (not self.connection.connected):
                print("[" + str(datetime.datetime.now()) + "] Stm server trying to connect to server")
                self.connection.connectTo(self.hostname, self.port, self.incomingMessageProcessor)
                self.testConnection()
                self.registerAsComponent()
                print("[" + str(datetime.datetime.now()) + "] Stm server connected to server")       
#            else :
 #               self.testConnection()
  #              pass
        except Exception as e :
            print("[" + str(datetime.datetime.now()) + "] Stm server: connection dead: " + str(e))
            self.connection.stop()
        

    def registerAsComponent(self):
        print('Registering component...')
        answer = self.connection.ServerRegisterComponent_response(com.CommandsStore.destinationToIndex['stm'])
        if answer is not None :
            print('Answer from server:' + str(answer))
        else :
            raise Exception('No answer to register component command')

    def testConnection(self):
        response = self.connection.ServerEcho_response("Hello", "you")

        if response is not None and response[0] == "Hello" and response[1] == "you":
            print("[" + str(datetime.datetime.now()) + '] STM Server: Connection alive')
        else :
            raise Exception('Connection dead')

    def error(self, message):
        print('Received error ' + message[0] + "\n")
        
    def processPing(self, message):
        return [ message[0] ]
    
    def processGetStatesMessage(self, message):
        return [ self.machines.keys() , [machine.machine.state.name for machine in self.machines.values()] ]

    def processGetStateMessage(self, message):
        machine = self.getMachine(message[0])
        if machine :
            state = machine.machine.state
            if state :
                return [ message[0] , state.name]
        return [ message[0] , 'Initial']
        
    def processGetMachinesInfo(self,message):
        answer = [ self.machines.keys() , [machine.machine.statusString() for machine in self.machines.values()] , [machine.machine.error for machine in self.machines.values()] ]
        return answer

    def processLoadXMLMachineMessage(self, message):
        xml = message[0]
        print("Loading machine " + xml )
        machine = StateMachine.from_xml_stream(xml)
        self.load(machine, False, float("inf"), True)
        for key in self.store.commands:
            setattr( machine, key, getattr(self.connection, key) )
            setattr( machine, key+"_response", getattr(self.connection, key+"_response") )
        machine.GetMoveValues = lambda movename, blocknames : machine.SchedulerGetOutputValues_response(movename,blocknames)
        return ['Loaded machine ' + machine.name]

    def getMachine(self, name):
        if name in self.machines :
            return self.machines[name]
        else :
            return None;
            
    def processStartMachineMessage(self,message):
        print('Starting machine ' + message[0]) 
        self.startMachine(message[0]);
        return ['Started machine ' + message[0]]

    def processStopMachineMessage(self,message):
        machine = self.getMachine(message[0])
        if machine :
            machine.machine.stop()
            return ['Stopped machine ' + message[0]]
        else :
            return ['Unknown machine ' + message[0]]


    def processKillMachineMessage(self,message):
        self.kill(message[0])
        return ['Killed machine ' + message[0]]
      
      
    def processSetMachineAttributes(self, message):
        machine = self.getMachine(message[0])
        if machine :
            machine.machine.set_attributes(message[1],message[2])
        
    def processGetMachineAttributes(self, message):
        machine = self.getMachine(message[0])
        if machine :
            return [machine.machine.get_attributes(message[1])]
        else:
            return []

    def StmGetAndSetMachineAttributes(self, message):
        machine = self.getMachine(message[0])
        if machine :
            machine.machine.set_attributes(message[1],message[2])
            return [machine.machine.get_attributes(message[3])]
        else :
            return []
        
    def processEvaluatePythonExpressions(self, message):
        if message[0] :
            machine = self.getMachine(message[0])
            if machine :
                machine.x = message[2]
                return [ [ machine.machine.evaluate(expression) for expression in message[1] ] ]
            else :
                return []
        else :
            x = message[2]
            answer = []
            for expression in message[1] :
                answer.append( float( eval(expression) ) )
            return [ answer ]
             
        
    def processMessage(self, message):
        try :
            if message.specification.name in self.callbacks:
                callback = self.callbacks[message.specification.name];
                #print("Processing message " + message.specification.name + " " +  str( message.uid ) );
                if(callback != None) : 
                    self.lock.acquire()
                    try :
                        data = callback(message.data)
                        if data != None :
                            self.connection.sendAnswer(message, data)
                        #else :
                        #    print('No answer to ' + message.specification.name)
                        self.lock.release()

                    except Exception as e :
                        self.lock.release()
                        print("Error '" + str(e) + "', failed to process command '" + message.specification.name + "'")
                        self.connection.sendAnswer(message, [str(e)], 'StmError')

        except Exception as e:
            print("Got exception " + str(e) + " while receiving message: " + str(message.uid))
    
