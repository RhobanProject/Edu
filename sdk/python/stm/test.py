# -*- coding: utf-8 -*-

import yaml
import stm
import os

#from collections import OrderedDict


if __name__ == '__main__':
    print(yaml.__file__)
    
    try:
         
        print("Creating yaml DiceMachine description")
        machine = stm.DiceMachineDescription()
        with open("test/diceMachine.yaml",'w') as serial_stream:
            serialized = yaml.dump(machine.toTree(), default_flow_style=False, indent = 4)
            serial_stream.write(serialized)
            
        print("Deserializing and reserializing DiceMachine")
        with open("test/diceMachine.yaml",'r') as yaml_stream:
            with open("test/diceMachineReserialized.yaml",'w') as deserial_stream:
                truc = yaml.load(yaml_stream)
                newmachine = stm.StateMachineDescription()
                newmachine.from_tree(truc)
                yaml.dump(newmachine.toTree(), deserial_stream, default_flow_style=False, indent = 4)
        
        print("Generating DiceMachine")
        with open("test/diceMachineGenerated.py",'w') as python_stream:
            python_stream.write( newmachine.toPython() )

        print("Executing DiceMachine\n")
        os.system("test/diceMachineGenerated.py")
     
    except yaml.scanner.ScannerError as e:
        print("Failed to parse file:\n" , e)
        
