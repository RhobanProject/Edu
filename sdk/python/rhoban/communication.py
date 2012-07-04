#!/usr/bin/python
# -*- coding: utf8 -*-

import sys, os, re, struct
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
        self.answerPattern = ParametersPattern(answerPattern)

"""
    Une connexion avec le serveur
"""
class Connection(tcp.TCPClient):
    def __init__(self, hostname, port):
        super(Connection, self).__init__()
        self.waitHeader = True
        self.buffer = ''
        self.message = None
        self.length = 0
        self.connectTo(hostname, port)

    def sendMessage(self, message):
        self.transmit(message.getRaw())

    def processData(self):
        change = True
        headerSize = 16
        
        while change:
            change = False
            if self.waitHeader:
                if len(self.buffer) >= headerSize:
                    (uid, destination, command, length) = struct.unpack('>IIII', self.buffer[0:headerSize])
                    self.buffer = self.buffer[headerSize:]
                    self.message = Message(uid, destination, command)
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

        while message == None:
            if len(self.buffer):
                message = self.processData()

            if message != None:
                break

            self.buffer += self.receive(1024)
        
        return message
        

"""
    Message à envoyer au serveur
"""
class Message:
    def __init__(self, uid, destination, command):
        self.uid = uid
        self.destination = destination
        self.command = command
        self.data = ''

    def getRaw(self):
        raw = ''
        raw += struct.pack('>I', int(self.uid))
        raw += struct.pack('>I', int(self.destination))
        raw += struct.pack('>I', int(self.command))
        raw += struct.pack('>I', len(self.data))
        raw += self.data

        return raw

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

    def __getattr__(self, name):
        specification = self.store.get(name)
        uid = self.getUid()

        def messageBuilder(*args):
            message = Message(uid, specification.destination, specification.command)
            message.data = specification.parametersPattern.getData(*args)

            return message

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
        'localisation': 6
    }

    def __init__(self):
        self.commands = {}
        self.indexCommands = {}
        self.builder = MessageBuilder(self)

    def parseXml(self, filename):
        xmldoc = minidom.parse(filename)

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

    def readData(self, message):
        specification = self.indexCommands[(int(message.destination), int(message.command))]
        return specification.answerPattern.readData(message.data)

    def get(self, name):
        return(self.commands[name])

    def __iter__(self):
        return iter(self.commands)

"""
    Représente le motif de plusieurs paramètres
"""
class ParametersPattern:
    def __init__(self, patternsString):
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
            raise Exception('Arguments error')

        data = ''

        for (argument, pattern) in zip(args, self.patterns):
            data += pattern.getData(argument)

        return data

    def readData(self, data):
        arguments = []

        for pattern in self.patterns:
            (data, argument) = pattern.readData(data)
            arguments += [argument]

        return arguments

""" 
    Représente un motif de paramètre, et permet de vérifier qu'un
    paramètre est bien formé
"""
class ParameterPattern:
    typesMapping = {
        'float': [float, 'f', 4],
        'ui32': [int, 'I', 4],
        'int': [int, 'i', 4],
        'byte': [str, 'c', 1],
        'string': [str, 's', 1],
    }

    def __init__(self, specification):
        self.specification = specification.replace('string', 'byte[]')
        self.subPattern = None
        self.baseType = specification.strip('[]')
        self.depth = 0
        
        if self.specification.endswith('[]'):
            tmp = self.specification

            while tmp.endswith('[]'):
                self.depth += 1
                tmp = tmp[:-2]

            self.subPattern = ParameterPattern(self.specification[:-2])

    def cpp(self):
        return ('vector<'*self.depth) + self.baseType + ('>'*self.depth) + ' '
    
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

    def getData(self, argument):
        data = ''

        if self.subPattern != None:
            data += struct.pack('>I', len(argument))

            for subArgument in argument:
                data += self.subPattern.getData(subArgument)
        else:
            if self.depth == 1:
                packFormat = '>' + str(len(argument)) + self.typesMapping[self.baseType][1]
                data += struct.pack('>I', len(argument))
                if type(argument) == 'list':
                    data += struct.pack(packFormat, *argument)
                else:
                    data += struct.pack(packFormat, argument)
            else:
                data += struct.pack('>' + self.typesMapping[self.baseType][1], self.typesMapping[self.baseType][0](argument))

        return data

    def readData(self, data):
        if self.subPattern != None:
            length = struct.unpack('>I', data[:4])[0]
            data = data[4:]
            argument = []

            if self.depth == 1:
                size = length * self.typesMapping[self.baseType][2]
                argument = list(struct.unpack('>' + str(length) + self.typesMapping[self.baseType][1], data[:size]))

                if self.specification == 'byte[]':
                    argument = argument[0]

                return (data[size:], argument)
            else:
                for n in xrange(length):
                    (data, subArgument) = self.subPattern.readData(data)
                    argument += [subArgument]

                return (data, argument)
        else:
            length = self.typesMapping[self.baseType][2]
            argument = struct.unpack('>' + self.typesMapping[self.baseType][1], data[:length])[0]

            return (data[length:], argument)

