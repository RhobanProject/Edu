# -*- coding: utf-8 -*-

import yaml



if __name__ == '__main__':
    print(yaml.__file__)
    
    try:
        stream = open("dice.yaml", 'r')
        result = yaml.load(stream)
        print(result)
    except yaml.scanner.ScannerError as e:
        print("Failed to parse file:\n" , e)
        
    result = yaml.load("""
        name: Vorlin Laruknuzum
        sex: Male
        class: Priest
        title: Acolyte
        hp: [32, 71]
        sp: [1, 13]
        gold: 423
        inventory:
        - a Holy Book of Prayers (Words of Wisdom)
        - an Azure Potion of Cure Light Wounds
        - a Silver Wand of Wonder
        """)
    
    print(result)
