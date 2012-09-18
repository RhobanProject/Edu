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
from os import system
from time import sleep
from sys import exit 
#from collections import OrderedDict

'''Demonstrates several features of state machines'''

if __name__ == '__main__':
    print(yaml.__file__)
    
    try:
        print("Deserializing and reserializing DiceMachine")
        with open("dice_machine.yaml",'r') as yaml_stream:
            with open("dice_machine_reserialized.yaml",'w') as deserial_stream:
                truc = yaml.load(yaml_stream)
                dice_machine_desc = StateMachine()
                dice_machine_desc.from_tree(truc)
                yaml.dump(dice_machine_desc.toTree(), deserial_stream, default_flow_style=False, indent = 4)
        

        print("\n\n" + 40*"*" + "\nDynamic stm execution demo\n" + 40*"*" + "\n")

        sleep(1)
        
        print("\n\nPlaying the drunk sailor as a standalone thread (and waiting for the end)")
        drunk_sailor = StateMachine.from_yaml("drunk_sailor.yaml")[0]
        drunk_sailor.play()
        drunk_sailor.join()

        print("\n\n" + 40*"*" + "\nStm loader demo\n" + 40*"*" + "\n")

        sleep(1)

        print("Creating machine loader")
        loader = StateMachineLoader()

        print("Loading the dice machine into the loader (not waiting for the end)")
        loader.load( StateMachine.from_yaml("dice_machine.yaml")[0] )

        print("\n\nLoading two machines playing PingPong for five seconds (and waiting for the end)")
        loader.load_machines( StateMachine.from_yaml("ping_pong.yaml") , True, 5.0 )

        sleep(5)

        print("\n\n" + 40*"*" + "\nPython code generation\n" + 40*"*" + "\n")

        sleep(1)

        print("Generating DiceMachine")
        StateMachine.yaml_to_py("dice_machine.yaml","dice_machine.py")

        print("Generating DrunkSailor")
        StateMachine.yaml_to_py("drunk_sailor.yaml","drunk_sailor.py")

        print("Generating PingPong")
        StateMachine.yaml_to_py("ping_pong.yaml","ping_pong.py")

        #print("Playing ping pong game from generated python code")
        #with open("ping_pong.py",'r') as input:
        #    exec(input.read())

        print("Playing the drunk sailor from generated python code")
        with open("drunk_sailor.py",'r') as input:
            exec(input.read())

        print("Playing dice game from generated python code")
        with open("dice_machine.py",'r') as input:
            exec(input.read())        
      
    except yaml.scanner.ScannerError as e:
        print("Failed to parse file:\n" , e)
        
