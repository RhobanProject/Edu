#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys, os
import unittest
sys.path.append(os.path.join(os.path.dirname(__file__), '..'))
import rhoban.communication as com

"""
    Test que les patterns matchent bien les bonnes variables
    python
"""
class TestPatterns(unittest.TestCase):

    className = com.ParameterPattern

    def test_int(self):
        spec = self.className('int')

        self.assertTrue(spec.check(1))
        self.assertFalse(spec.check('x'))
        self.assertFalse(spec.check([]))

    def test_float(self):
        spec = self.className('float')

        self.assertTrue(spec.check(1.23))
        self.assertTrue(spec.check(1))
        self.assertFalse(spec.check('x'))
        self.assertFalse(spec.check([]))

    def test_byte(self):
        spec = self.className('byte')

        self.assertTrue(spec.check(23))
        self.assertFalse(spec.check([]))

    def test_arrays(self):
        spec = self.className('float[][]')

        self.assertTrue(spec.check([[1.2, 5.3], [22]]))
        self.assertTrue(spec.check([]))
        self.assertFalse(spec.check(1))
        self.assertFalse(spec.check(1.23))
        self.assertFalse(spec.check([['a']]))

if __name__ == '__main__':
    unittest.main()
