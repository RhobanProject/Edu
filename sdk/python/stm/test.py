# -*- coding: utf-8 -*-
'''
/*************************************************
* Publicly released by Rhoban System, September 2012
* www.rhoban-system.fr
*
* Freely usable for non-commercial purposes
*
* Licence Creative Commons *CC BY-NC-SA
* http://creativecommons.org/licenses/by-nc-sa/3.0
*************************************************/
'''

import yaml
from stm.stm import StateMachine
from stm.stm_loader import StateMachineLoader
from stm.stm_component import StateMachineServer
from os import system
from time import sleep
from sys import exit 
#from collections import OrderedDict

'''Demonstrates several features of state machines'''

if __name__ == '__main__':
    print(yaml.__file__)
    
    try:
 
        server = StateMachineServer()
        server.testConnection()
      
    except yaml.scanner.ScannerError as e:
        print("Failed to parse file:\n" , e)
        
