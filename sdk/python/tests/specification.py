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
    paramClass = com.ParameterPattern

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

        argument = [1,1.0,'c','hello']
        data = '\x00\x00\x00\x01?\x80\x00\x00c\x00\x00\x00\x05hello'

        self.assertEquals(pattern.getData(*argument), data)
        self.assertEquals(pattern.readData(data), argument)

    def test_array_pattern(self):
        pattern = self.paramsClass('ui32[][]')

        argument = [[[1,2],[3,4]]]
        data = '\x00\x00\x00\x02\x00\x00\x00\x02\x00\x00\x00\x01\x00\x00\x00\x02\x00\x00\x00\x02\x00\x00\x00\x03\x00\x00\x00\x04'
        
        self.assertEquals(pattern.getData(*argument), data)
        self.assertEquals(pattern.readData(data), argument)

    def test_bad_patterns(self):
        pattern = self.paramsClass('ui32')

        self.assertRaises(TypeError, pattern.getData, [])
        self.assertRaises(TypeError, pattern.getData, 'x')
        self.assertRaises(TypeError, pattern.getData, None)

    def test_bad_datas(self):
        pattern = self.paramsClass('ui32')

        dataIncomplete = '\x00\x00\x00'
        dataOverflow = '\x00\x00\x00\x01\x02'

        self.assertRaises(IOError, pattern.readData, dataIncomplete)
        self.assertRaises(IOError, pattern.readData, dataOverflow)

    def test_bad_datas_array(self):
        pattern = self.paramsClass('ui32[]')

        data = '\x00\x00\x00\x99\x00\x00\x00\x01'

        self.assertRaises(IOError, pattern.readData, data)

    def test_strings(self):
        pattern = self.paramsClass('string string[]')

        argument = ['hello', ['worl', 'd!']]
        data = '\x00\x00\x00\x05hello\x00\x00\x00\x02\x00\x00\x00\x04worl\x00\x00\x00\x02d!'
        
        self.assertEquals(pattern.getData(*argument), data)
        self.assertEquals(pattern.readData(data), argument)

    def test_builder(self):
        store = self.storeClass()
        store.addSpecification(self.specClass('test', 'desc', 12, 34, 'ui32 ui32', ''))
        message = store.builder.test(5, 8)

        self.assertEquals(message.getRaw(), '\x00\x00\x00\x01\x00\x00\x00\x0c\x00\x00\x00"\x00\x00\x00\x08\x00\x00\x00\x05\x00\x00\x00\x08')

    def test_cpp(self):
        self.assertEqual(self.paramClass('ui32').cpp().strip(), 'ui32')
        self.assertEqual(self.paramClass('float').cpp().strip(), 'float')
        self.assertEqual(self.paramClass('byte').cpp().strip(), 'byte')
        self.assertEqual(self.paramClass('string').cpp().strip(), 'string')
        self.assertEqual(self.paramClass('ui32[]').cpp().strip(), 'vector<ui32 >')
        self.assertEqual(self.paramClass('string[]').cpp().strip(), 'vector<string >')
        self.assertEqual(self.paramClass('float[][]').cpp().strip(), 'vector<vector<float > >')


if __name__ == '__main__':
    unittest.main()
