#!/usr/bin/python
# -*- coding: utf8 -*-

import sys, os, re
from xml.dom import minidom

"""
    Représente la specification d'une commande
"""
class CommandSpecification:
    destinationToIndex = {
        'error': 0,
        'server': 1,
        'system': 2,
        'low_level': 3,
        'move_scheduler': 4,
        'vision': 5,
        'localisation': 6
    }

    def __init__(self, name, description, destination, index, parametersPattern, answerPattern):
        self.name = name
        self.descritption = description
        self.destination = destination
        self.destinationIndex = self.destinationToIndex[destination]
        self.index = index
        self.parametersPattern = ParametersPattern(parametersPattern)
        self.answerPattern = ParametersPattern(answerPattern)

"""
    Message à envoyer au serveur
"""
class Message:
    pass

"""
    Créateur de message
"""
class MessageBuilder:
    def __init__(self, store):
        self.store = store
        self.uid = 0

    def __getattr__(self, name):
        specification = self.store.get(name)

        def messageBuilder(*args):
            pass

        return messageBuilder

"""
    Magasin de commandes
"""
class CommandStore:

    def __init__(self, filename):
        self.commands = {}
        self.builder = MessageBuilder(self)
        xmldoc = minidom.parse(filename)

        def getText(command, name):
            tag = command.getElementsByTagName(name)
            if tag != []:
                childNodes = tag[0].childNodes
                if childNodes != []:
                    return childNodes[0].data
            return ''

        for command in xmldoc.childNodes[0].getElementsByTagName("CommandSpecification"):
            name = getText(command, 'Name')
            description = getText(command, 'Description')
            destination = getText(command, 'Destination')
            commandIndex = getText(command,'CommandIndex')
            parametersPattern = getText(command, 'ParametersPattern')
            answerPattern = getText(command, 'AnswerPattern')

            self.commands[name] = CommandSpecification(name, description, destination, commandIndex, parametersPattern, answerPattern)

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


""" 
    Représente un motif de paramètre, et permet de vérifier qu'un
    paramètre est bien formé
"""
class ParameterPattern:
    typesMapping = {
        'float': float,
        'ui32': int,
        'int': int, 
        'byte': str
    }

    def __init__(self, specification):
        self.specification = specification
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
            if type(var) != list:
                return False
            
            for subVar in var:
                if not self.subPattern.check(subVar):
                    return False

            return True
        else:
            if type(var) == list:
                return False

            try:
                dummy = self.typesMapping[self.specification](var)
                return True
            except ValueError:
                 return False
