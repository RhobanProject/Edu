#!/usr/bin/python
# -*- coding: utf8 -*-

import sys, os
import unittest
sys.path.append(os.path.join(os.path.dirname(__file__), '..'))
import rhoban.communication as com

"""
    Test la cohérence de l'encodage/décodage des données
"""
class TestSpecification(unittest.TestCase):

    storeClass = com.CommandsStore
    specClass = com.CommandSpecification
    paramsClass = com.ParametersPattern

    def test_store(self):
        store = self.storeClass()
        store.addSpecification(self.specClass('test', 'desc', 12, 34, '', ''))

        self.assertEquals(store.get('test').description, 'desc')
        self.assertEquals(store.get('test').destination, 12)
        self.assertEquals(store.get('test').command, 34)
        self.assertEquals(store.get('test').parametersPattern.patterns, [])
        self.assertEquals(store.get('test').answerPattern.patterns, [])
    
    def test_spec(self):
        store = self.storeClass()
        spec = self.specClass('test', 'desc', 12, 34, 'ui32 ui32', 'ui32')

        self.assertEquals(len(spec.parametersPattern), 2)
        self.assertEquals(len(spec.answerPattern), 1)

    def test_simple_pattern(self):
        pattern = self.paramsClass('ui32 float byte string')

        self.assertEquals(pattern.getData(1,1.0,'c','hello'), '\x00\x00\x00\x01?\x80\x00\x00c\x00\x00\x00\x05hello')
        self.assertEquals(pattern.readData('\x00\x00\x00\x01?\x80\x00\x00c\x00\x00\x00\x05hello'), [1,1.0,'c','hello'])

    def test_array_pattern(self):
        pattern = self.paramsClass('ui32[][]')

        self.assertEquals(pattern.getData([[1,2],[3,4]]), '\x00\x00\x00\x02\x00\x00\x00\x02\x00\x00\x00\x01\x00\x00\x00\x02\x00\x00\x00\x02\x00\x00\x00\x03\x00\x00\x00\x04')
        self.assertEquals(pattern.readData('\x00\x00\x00\x02\x00\x00\x00\x02\x00\x00\x00\x01\x00\x00\x00\x02\x00\x00\x00\x02\x00\x00\x00\x03\x00\x00\x00\x04'), [[[1,2],[3,4]]])


if __name__ == '__main__':
    unittest.main()
