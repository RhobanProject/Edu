#!/usr/bin/python
# -*- coding: utf-8 -*-

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



