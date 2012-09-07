# -*- coding: utf-8 -*-

import yaml
from stm import StateMachine, StateMachineDescription
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
                dice_machine_desc = StateMachineDescription()
                dice_machine_desc.from_tree(truc)
                yaml.dump(dice_machine_desc.toTree(), deserial_stream, default_flow_style=False, indent = 4)
        
        print("Generating DiceMachine")
        with open("dice_machine_generated.py",'w') as python_stream:
            python_stream.write( dice_machine_desc.toPython() )

            
        print("Creating machine scheduler")
        global scheduler
        scheduler = StateMachineLoader()        
        print("Executing DiceMachine\n")
        machine = StateMachine.from_file("dice_machine.yaml")
        scheduler.load_machine(machine)
        
        #machine.play()
        
        
        #machine.play()
        #os.system("dice_machine_generated.py")
     
    except yaml.scanner.ScannerError as e:
        print("Failed to parse file:\n" , e)
        
