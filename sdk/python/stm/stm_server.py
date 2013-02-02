'''
Created on 27 janv. 2013

@author: Hugo
'''

from repeated_task import RepeatedTask
from stm_loader import StateMachineLoader
from stm import StateMachine
import rhoban.communication as com
from rhoban.communication import CommandsStore
import time

class StateMachineServer(StateMachineLoader, RepeatedTask):
    
    def __init__(self,  hostname = 'localhost', port = 12345, storeFileName = '../common/commands.xml'):

        print("Creating StmServer connected to " + hostname + ":" + str(port))
        StateMachineLoader.__init__(self)
        
        self.hostname = hostname
        self.port = port

        self.callbacks = {'StmError' : self.error,
                           'StmLoadXMLMachine':self.processLoadXMLMessage,
                           'StmStartMachine':self.processStartMachineMessage,
                           'StmStopMachine':self.processStopMachineMessage,
                           'StmKillMachine':self.processKillMachineMessage,
                           'StmGetStates' : self.processGetStatesMessage,
                           'StmGetMachinesInfo':self.processGetMachinesInfo}
        
        self.store = com.CommandsStore()
        self.store.parseXml(storeFileName)

        self.connection = com.Connection()
        self.connection.setStore(self.store)

        RepeatedTask.__init__(self,1,self.connectIfNeeded)
        self.machines = []

    def start(self):
        RepeatedTask.start(self)
        
    def connectIfNeeded(self):
        if (not self.connection.connected):
            try :
                print("Stm server trying to connect to server")
                self.connection.connectTo(self.hostname, self.port, self.incomingMessageProcessor)
                self.testConnection()
                self.registerAsComponent()
                print("Stm server connected to server")       
            except Exception as e :
                print("Failed to connect: " + str(e))
                self.connection.stop()
        

    def registerAsComponent(self):
        message = self.store.builder.build('ServerRegisterComponent', com.CommandsStore.destinationToIndex['stm'])
        print('Registering component...')
        answer = self.connection.sendMessageReceive(message)
        if answer is not None :
            print('Answer from server:' + str(answer))
        else :
            raise Exception('No answer to register component command')

    def testConnection(self):
        print('Testing echo "Hello world"...')
        response = self.connection.ServerEcho_response('Hello', 'you')
        print('response ' + str(response))

        if response is not None and response[0] == b'Hello' and response[1] == b'you':
            print("\n"+'Connection test: ok')
        else :
            raise Exception('Connection test: failed')

    def error(self, message):
        print('Received error ' + message[0] + "\n")
        
    def processGetStatesMessage(self, message):
        return [ [machine.name for machine in self.machines] , [machine.state.name for machine in self.machines] ]
    
    def processGetMachinesInfo(self,message):
        return [ [machine.name for machine in self.machines] , [str(machine.status) for machine in self.machines] ]
        
    def processLoadXMLMessage(self, message):
        xml = message[0]
        machine = StateMachine.from_xml(xml)
        self.load(machine, False, float("inf"), True)
        return 'Loaded machine ' + machine.name

    def getMachine(self, name):
        if machineName in self.machines :
            return self.machines[machineName]
        else :
            raise Exception("Unknown machine '" + machineName + "'")
            
    def processStartMachineMessage(self,message):
        machine = getMachine(message[0])
        machine.start()
        return 'Started machine ' + message[0]

    def processStopMachineMessage(self,message):
        machine = getMachine(message[0])
        machine.stop()
        return 'Stopped machine ' + message[0]

    def processKillMachineMessage(self,message):
        self.kill(message[0])
        return 'Killed machine ' + message[0];
        
    def incomingMessageProcessor(self, message):
        try :
            if message.specification.name in self.callbacks:
                callback = self.callbacks[message.specification.name];
                if(callback != None) : 
                    answer = None

                    self.lock.acquire()
                    try :
                        data = callback(message.data)
                    except Exception as e:
                        self.lock.release()
                        self.connection.sendAnswer(message, str(e), 'StmError')
                    self.lock.release()

                    if data != None :
                        self.connection.sendAnswer(message, data)

        except Exception as e:
            print("Got exception " + str(e) + " while receiving message: " + str(message.uid))


