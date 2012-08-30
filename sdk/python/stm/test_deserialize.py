# -*- coding: utf-8 -*-

import yaml
import stm

#from collections import OrderedDict


if __name__ == '__main__':
    print(yaml.__file__)
    
    try:
         
        machine = stm.DiceMachineDescription()

        python_stream = open("dice2.yaml",'w')
        yaml.dump(machine.toTree(), python_stream, default_flow_style=False, indent = 4)
        python_stream.close()
        
        yaml_stream = open("dice2.yaml",'r')
        python_stream = open("dice3.yaml",'w')
        truc = yaml.load(yaml_stream)
        print(truc)
        machine.from_tree(truc)
        yaml.dump(machine.toTree(), python_stream, default_flow_style=False, indent = 4)
        python_stream.close()
        yaml_stream.close()
        
        
     
    except yaml.scanner.ScannerError as e:
        print("Failed to parse file:\n" , e)
        
