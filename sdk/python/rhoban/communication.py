#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys, os, re, struct, threading, time
from xml.dom import minidom
import sockets.tcp as tcp

"""
    Représente la specification d'une commande
"""
class CommandSpecification:
    def __init__(self, name, description, destination, command, parametersPattern, answerPattern):
        self.name = name
        self.description = description
        self.destination = destination
        self.command = command
        self.parametersPattern = ParametersPattern(parametersPattern)
        self.answerPattern = ParametersPattern(answerPattern, name)

"""
    Une connexion avec le serveur
"""
class Connection(tcp.TCPClient):
    def __init__(self):
        super(Connection, self).__init__()

    def connectTo(self, hostname, port, receptionCallback = None):
        self.hostname = hostname
        self.port = port
        self.receptionCallback = receptionCallback;
        self.waitHeader = True
        self.buffer = b''
        self.message = None
        self.length = 0
        self.counter = 0
        self.connect()


    def connect(self):
        super(Connection, self).connectTo(self.hostname, self.port)
        if self.connected :
            self.mailbox = Mailbox(self, self.receptionCallback)
            self.mailbox.start()


    def stop(self):
        self.close()
        self.connected = False

    def close(self):
        super(Connection, self).close()

    def setStore(self, store):
        self.store = store

    def sendMessageReceive(self, message, timeout = 5):
        if not self.connected :
            raise Exception("Cannot send message: disconnected")

        entry = MailboxEntry(self.store.getSpecification(message))
        entry.event = threading.Event()

        entry.event.clear()
        self.mailbox.entries[message.uid] = entry
        self.sendMessage(message)
        entry.event.wait(timeout)

        if message.uid in self.mailbox.entries:
            response = self.mailbox.entries[message.uid].response
            del self.mailbox.entries[message.uid]
            return response

        return None

    def sendMessageCallback(self, message, callback):
        if not self.connected :
            raise Exception("Cannot send message: disconnected")

        entry = MailboxEntry(self.store.getSpecification(message))
        entry.callback = callback
        self.mailbox.entries[message.uid] = entry
        self.counter = self.counter + 1

        if self.counter % 300 == 0:
            self.mailbox.garbageCollect()

        self.sendMessage(message)

    def sendMessage(self, message):
        if not self.connected :
            raise Exception("Cannot send message: disconnected")
        self.transmit(message.getRaw())

    def sendAnswer(self, incomingMessage, data, commandName = None):
        if not self.connected :
            raise Exception("Cannot send answer: disconnected")

        if commandName == None :
            specification = incomingMessage.specification
        else :
            specification = self.store.commands[commandName]

        answer = Message(incomingMessage.uid, incomingMessage.destination, incomingMessage.source, specification.command )
        answer.isAnswer = True
        answer.data = specification.answerPattern.getData(*data)
        self.transmit(answer.getRaw())

    def processData(self):
        change = True
        headerSize = 16
        
        while change:
            change = False
            if self.waitHeader:
                if len(self.buffer) >= headerSize:
                    (uid, source, destination, command, length) = struct.unpack('>IHHII', self.buffer[0:headerSize])
                    self.buffer = self.buffer[headerSize:]
                    self.message = Message(uid, source, destination, command)
                    self.length = length
                    self.waitHeader = False
                    change = True
            else:
                if len(self.buffer) >= self.length:
                    self.waitHeader = True
                    self.message.data = self.buffer[0:self.length]
                    self.buffer = self.buffer[self.length:]
                    return self.message

        return None

    def getMessage(self):
        message = None

        try:
            while message == None:
                if len(self.buffer):
                    message = self.processData()

                if message != None:
                    break

                self.buffer += self.receive(1024)

            message.readData(self.store)
            
        except Exception as e:
            print("Connection caught exception " + str(e) + " while receiving message")
            raise e

        return message

    def __getattr__(self, name):
        responseSuffix = '_response'
        callbackSuffix = '_callback'

        if name.endswith(responseSuffix):
            name = name[:-len(responseSuffix)]
            def buildAndSend(*args):
                message = self.store.builder.build(name, *args)
                return self.sendMessageReceive(message)
        else:
            if name.endswith(callbackSuffix):
                name = name[:-len(callbackSuffix)]
                def buildAndSend(*args):
                    callback = args[-1]
                    args = args[:-1]
                    message = self.store.builder.build(name, *args)
                    return self.sendMessageCallback(message, callback)
            else:
                def buildAndSend(*args):
                    message = self.store.builder.build(name, *args)
                    return self.sendMessage(message)

        return buildAndSend

"""
    Une entrée dans la boite aux lettres
"""
class MailboxEntry:
    def __init__(self, specification):
        self.creation = time.time()
        self.response = None
        self.specification = specification
        self.callback = None
        self.event = None

    def expires(self):
        return time.time()-self.creation > 300

"""
    Boîte aux lettres d'échange de messages
"""
class Mailbox(threading.Thread):
    def __init__(self, connection, incomingMessageProcessor = None):
        super(Mailbox, self).__init__()
        self.entries = {}
        self.connection = connection
        self.incomingMessageProcessor = incomingMessageProcessor;

    def run(self):
        while self.connection.connected:
            
            try :
                message = self.connection.getMessage()
    
                if message == None:
                    break
    
                uid = message.uid
                
                if message.isAnswer and uid in self.entries:
                    entry = self.entries[uid]
                    entry.response = message.data
    
                    if entry.event != None:
                        entry.event.set()
    
                    if entry.callback != None:
                        entry.callback(entry.response)
                        del self.entries[uid]
                    
                if self.incomingMessageProcessor != None:
                    try :
                        self.incomingMessageProcessor(message)
                    except Exception as e :
                        print("Exception " + str(e) + " while processing incoming message " + str(message.uid))

            except Exception as e:
                print("Mailbox caught exception '" + str(e)  + "', disconnecting")
                self.connection.stop()
                 
    def garbageCollect(self):
        for uid in self.entries:
            if self.entries[uid].expires():
                del self.entries[uid]

"""
    Message à envoyer au serveur
"""
class Message:
    def __init__(self, uid, source, destination, command):
        self.uid = uid
        self.source = source
        self.destination = destination
        self.command = str(int(command)  & 0x7FFFFFFF)
        self.isAnswer = (int(command) & 0x80000000 != 0)
        self.data = b''

    def getRaw(self):
        raw = b''
        raw += struct.pack('>I', int(self.uid))
        raw += struct.pack('>H', int(self.source))
        raw += struct.pack('>H', int(self.destination))
        if self.isAnswer :
            raw += struct.pack('>I', int(self.command) | 0x80000000 )
        else :
            raw += struct.pack('>I', int(self.command) )
        raw += struct.pack('>I', len(self.data))
        raw += self.data

        return raw

    def readData(self, store):
        self.specification = store.getSpecification(self)
        if self.isAnswer :
            self.data = self.specification.answerPattern.readData(self.data)
        else :
            self.data = self.specification.parametersPattern.readData(self.data)
        

"""
    Créateur de message
"""
class MessageBuilder:
    def __init__(self, store):
        self.store = store
        self.uid = 0

    def getUid(self):
        self.uid += 1

        return self.uid

    def build(self, name, *args):
        specification = self.store.get(name)

        message = Message(self.getUid(), 0, specification.destination, specification.command)
        message.data = specification.parametersPattern.getData(*args)
        return message
    

    def __getattr__(self, name):
        def messageBuilder(*args):
            return self.build(name, *args)

        return messageBuilder

"""
    Magasin de commandes
"""
class CommandsStore:
    destinationToIndex = {
        'error': 0,
        'server': 1,
        'system': 2,
        'low_level': 3,
        'move_scheduler': 4,
        'vision': 5,
        'localisation': 6,
        'stm': 7
    }

    def __init__(self):
        self.commands = {}
        self.indexCommands = {}
        self.builder = MessageBuilder(self)

    def parseXml(self, filename):
        xmldoc = minidom.parse(filename)
        if not hasattr(xmldoc,'childNodes') :
            raise Exception("Failed to parse xml file "+ filename);
        

        def getText(command, name):
            tag = command.getElementsByTagName(name)
            if tag != []:
                childNodes = tag[0].childNodes
                if childNodes != []:
                    return childNodes[0].data
            return ''

        for commandSpec in xmldoc.childNodes[0].getElementsByTagName("CommandSpecification"):
            name = getText(commandSpec, 'Name')
            description = getText(commandSpec, 'Description')
            destination = getText(commandSpec, 'Destination')
            command = getText(commandSpec,'CommandIndex')
            parametersPattern = getText(commandSpec, 'ParametersPattern')
            answerPattern = getText(commandSpec, 'AnswerPattern')

            self.addSpecification(CommandSpecification(name, description, self.destinationToIndex[destination], command, parametersPattern, answerPattern))

    def addSpecification(self, specification):
            self.commands[specification.name] = specification
            self.indexCommands[(int(specification.destination), int(specification.command))] = specification

    def getSpecification(self, message):
        try :
            if message.isAnswer :
                cmd = self.indexCommands[( int(message.source), int(message.command) )]
            else :
                cmd = self.indexCommands[( int(message.destination), int(message.command) )]
            return cmd
        except KeyError:
            raise Exception("No command with destination " + str(message.destination) + " and command " + str(message.command) + " in command store")

    def readData(self, message):
            if message.isAnswer :
                return self.getSpecification(message).answerPattern.readData(message.data)
            else :
                return self.getSpecification(message).parametersPattern.readData(message.data)

    def get(self, name):
        return(self.commands[name])

    def __iter__(self):
        return iter(self.commands)

"""
    Représente le motif de plusieurs paramètres
"""
class ParametersPattern:
    def __init__(self, patternsString, name = ''):
        self.name = name
        self.patterns = []
        patternsString = patternsString.strip()

        if patternsString:
            for pattern in re.split('\s+', patternsString):
                self.patterns += [ParameterPattern(pattern)]

    def __iter__(self):
        return iter(self.patterns)

    def __len__(self):
        return len(self.patterns)

    def getData(self, *args):
        if len(args) != len(self.patterns):
            raise Exception('Arguments error, found %s arguments where %s expected' % (len(args), len(self.patterns)))

        data = b''

        try:
            for (argument, pattern) in zip(args, self.patterns):
                data += pattern.getData(argument)
        except (ValueError, TypeError):
            raise TypeError('Bad arguments')

        return data

    def readData(self, data):
        arguments = []

        try:
            for pattern in self.patterns:
                (data, argument) = pattern.readData(data)
                arguments += [argument]
        except Exception as e:
            raise IOError('Unable to read arguments from data for command %s (%s)' + str(e) % (self.name, repr(data)))

        if data:
            raise IOError('Remaining %d bytes of data for command %s (%s)' % (len(data), self.name, repr(data)))

        return arguments

""" 
    Représente un motif de paramètre, et permet de vérifier qu'un
    paramètre est bien formé
"""
class ParameterPattern:
    typesMapping = {
        'float': [float, 'f', 4],
        'ui32': [int, 'I', 4],
        'uint': [int, 'I', 4],
        'int': [int, 'i', 4],
        'byte': [int, 'B', 1],
        'string': [str, 's', 1],
        'bool': [bool, '?', 1],
    }

    def __init__(self, specification, baseType = None):
        self.specification = specification.replace('string', 'byte[]')
        self.subPattern = None
        self.depth = 0
 
        if baseType == None:
            self.baseType = specification.strip('[]')
        else:
            self.baseType = baseType
        
        if not self.baseType in self.typesMapping:
            raise Exception('Unknown type: ' + self.baseType)
        
        if self.specification.endswith('[]'):
            tmp = self.specification

            while tmp.endswith('[]'):
                self.depth += 1
                tmp = tmp[:-2]

            self.subPattern = ParameterPattern(self.specification[:-2], self.baseType)

    def cpp(self):
        depth = self.depth

        if self.baseType == 'string':
            depth = depth-1

        return ('vector<'*depth) + self.baseType + (' >'*depth) + ' '
    
    def check(self, var):
        if self.subPattern != None:
            if type(var) != list and type(var) != str:
                return False

            if type(var) == str:
                return self.specification == 'byte[]'
            
            for subVar in var:
                if not self.subPattern.check(subVar):
                    return False

            return True
        else:
            if type(var) == list:
                return False

            try:
                dummy = self.typesMapping[self.baseType][0](var)
                return True
            except ValueError:
                 return False
                    
    def castType(self, argument):         
        return self.typesMapping[self.baseType][0](argument)

    def getData(self, argument):
        data = b''

        if self.depth > 1:
            data += struct.pack('>I', len(argument))

            for subArgument in argument:
                data += self.subPattern.getData(subArgument)
        else:
            if self.depth == 1:
                packFormat = '>' + str(len(argument)) + self.typesMapping[self.baseType][1]
                data += struct.pack('>I', len(argument))

                if type(argument) == list:
                    data += struct.pack(packFormat, *argument)
                else:
                    if type(argument) == str:
                        data += struct.pack(packFormat, bytes(argument, 'utf-8'))
                    else:
                        data += struct.pack(packFormat, argument)
            else:
                argument = self.castType(argument)
                data += struct.pack('>' + self.typesMapping[self.baseType][1], argument)

        return data

    def readData(self, data):
        if self.subPattern != None:
            length = struct.unpack('>I', data[:4])[0]
            data = data[4:]
            argument = []

            if self.depth == 1:
                size = length * self.typesMapping[self.baseType][2]
                argument = list(struct.unpack('>' + str(length) + self.typesMapping[self.baseType][1], data[:size]))

                if self.specification == 'byte[]' and self.baseType == 'string':
                    argument = str(argument[0],'utf8')

                return (data[size:], argument)
            else:
                for n in range(length):
                    (data, subArgument) = self.subPattern.readData(data)
                    argument += [subArgument]

                return (data, argument)
        else:
            length = self.typesMapping[self.baseType][2]
            argument = struct.unpack('>' + self.typesMapping[self.baseType][1], data[:length])[0]

            return (data[length:], argument)

