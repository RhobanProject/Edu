# -*- coding: utf-8 -*-

import yaml
from stm import StateMachine
from loader import StateMachineLoader

#from collections import OrderedDict

'''Performs a test for StateMachines
the dice_machine.yaml file is
 * loaded
 * deserialzed to a StateMachine description
 * reserialized to the file dice_machine_reserialized.py
 * turned to a py file dice_machine_generated.py
 * executed
'''

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
        
        print("Generating DiceMachine")
        with open("dice_machine.py",'w') as python_stream:
            python_stream.write( dice_machine_desc.to_python() )

        print("Creating machine scheduler")
        global scheduler
        scheduler = StateMachineLoader()        
        scheduler.load_machines( StateMachine.from_yaml("dice_machine.yaml") )
        scheduler.load_machines( StateMachine.from_yaml("ping_pong.yaml") )
        #scheduler.load_machines( StateMachine.from_yaml("drunk_sailor.yaml") )
        #print("Globals from top level ", globals().keys())
        #print("Locals from top level ", locals().keys())

             
    except yaml.scanner.ScannerError as e:
        print("Failed to parse file:\n" , e)
        
