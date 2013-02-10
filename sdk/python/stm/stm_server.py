'''
Created on 27 janv. 2013

@author: Hugo
'''

from repeated_task import RepeatedTask
from stm_loader import StateMachineLoader
from stm import StateMachine
import rhoban.communication as com
from rhoban.communication import CommandsStore
import datetime


class StateMachineServer(StateMachineLoader):
    
    def __init__(self,  hostname = 'localhost', port = 12345, storeFileName = '../common/commands.xml'):

        print("Creating StmServer connected to " + hostname + ":" + str(port))
        StateMachineLoader.__init__(self)
        
        self.hostname = hostname
        self.port = port

        self.callbacks = {'StmError' : self.error,
                           'StmLoadXMLMachine':self.processLoadXMLMachineMessage,
                           'StmStartMachine':self.processStartMachineMessage,
                           'StmStopMachine':self.processStopMachineMessage,
                           'StmKillMachine':self.processKillMachineMessage,
                           'StmGetState' : self.processGetStateMessage,
                           'StmGetStates' : self.processGetStatesMessage,
                           'StmGetMachinesInfo':self.processGetMachinesInfo}
        
        self.store = com.CommandsStore()
        self.store.parseXml(storeFileName)


    def start(self):
        self.connection = com.Connection()
        self.connection.setStore(self.store)
        self.connection_keeper =  RepeatedTask(1,self.connectIfNeeded)
        self.connection_keeper.start()

    def connectIfNeeded(self):
        try :
            if (not self.connection.connected):
                print("[" + str(datetime.datetime.now()) + "] Stm server trying to connect to server")
                self.connection.connectTo(self.hostname, self.port, self.incomingMessageProcessor)
                self.testConnection()
                self.registerAsComponent()
                print("[" + str(datetime.datetime.now()) + "] Stm server connected to server")       
            else :
                self.testConnection()
                pass
        except Exception as e :
            print("[" + str(datetime.datetime.now()) + "] Stm server: connection dead: " + str(e))
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
        response = self.connection.ServerEcho_response('Hello', 'you')

        if response is not None and response[0] == 'Hello' and response[1] == 'you':
            print("[" + str(datetime.datetime.now()) + '] Connection alive')
        else :
            raise Exception('Connection dead')

    def error(self, message):
        print('Received error ' + message[0] + "\n")
        
    def processGetStatesMessage(self, message):
        return [ self.machines.keys() , [machine.machine.state.name for machine in self.machines.values()] ]

    def processGetStateMessage(self, message):
        return [ message[0] , self.getMachine(message[0]).machine.state.name]
    
    def processGetMachinesInfo(self,message):
        return [ self.machines.keys() , [machine.machine.statusString() for machine in self.machines.values()] ]
        
    def processLoadXMLMachineMessage(self, message):
        xml = message[0]
        print("Loading machine " + xml )
        machine = StateMachine.from_xml_stream(xml)
        self.load(machine, False, float("inf"), True)
        return ['Loaded machine ' + machine.name]

    def getMachine(self, name):
        if name in self.machines :
            return self.machines[name]
        else :
            raise Exception("Unknown machine '" + name + "'")
            
    def processStartMachineMessage(self,message):
        self.startMachine(message[0]);
        return ['Started machine ' + message[0]]

    def processStopMachineMessage(self,message):
        machine = self.getMachine(message[0])
        machine.machine.stop()
        return ['Stopped machine ' + message[0]]

    def processKillMachineMessage(self,message):
        self.kill(message[0])
        return ['Killed machine ' + message[0]]
        
    def incomingMessageProcessor(self, message):
        try :
            if message.specification.name in self.callbacks:
                '''print("Processing message " + message.specification.name)'''
                callback = self.callbacks[message.specification.name];
                if(callback != None) : 
                    self.lock.acquire()
                    try :
                        data = callback(message.data)
                        if data != None :
                            '''print("Sending answer to message " + message.specification.name)'''
                            self.connection.sendAnswer(message, data)
                        self.lock.release()

                    except Exception as e :
                        self.lock.release()
                        self.connection.sendAnswer(message, [str(e)], 'StmError')


        except Exception as e:
            print("Got exception " + str(e) + " while receiving message: " + str(message.uid))


