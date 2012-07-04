#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys, os
from xml.dom import minidom

"""
    Représente la specification d'une commande
"""
class CommandSpecification:
    def __init__(self, name, description, destination, index, parametersPattern, answerPattern):
        self.name = name
        self.descritption = description
        self.destination = destination
        self.index = index
        self.parametersPattern = parametersPattern
        self.answerPattern = answerPattern

"""
    Magasin de commandes
"""
class CommandStore:

    def __init__(self, filename):
        self.commands = {}
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
        
        if self.specification.endswith('[]'):
            self.subPattern = ParameterPattern(self.specification[:-2])
    
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

if __name__ == '__main__':
    dictionnary = CommandStore(os.path.join(os.path.dirname(__file__), '../../common/commands.xml'))
    print(dictionnary.get('SendXml'))
