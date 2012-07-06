#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys, os
import unittest

loader = unittest.TestLoader()
suite = unittest.TestSuite()
pattern = '*.py'

dir_py = os.path.join(os.path.dirname(__file__), 'tests')
package_tests = loader.discover(start_dir=dir_py, pattern=pattern)
suite.addTests(package_tests)

unittest.TextTestRunner().run(suite)

