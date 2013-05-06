'''
Created on 31 janv. 2013

@author: Hugo
'''

import argparse
import time
import yaml

from stm_server import StateMachineServer

parser = argparse.ArgumentParser()

#-db DATABSE -u USERNAME -p PASSWORD -size 20
parser.add_argument("-s", "--server", help="Server ip address")
parser.add_argument("-p", "--port", help="Server port", type=int)
parser.add_argument("-c", "--command_store", help="Command store address")


class expando:
    pass

args = expando()

'''default values'''
args.server = 'localhost'
args.port = '7777'
args.command_store = 'commands.xml'

'''config file'''
try :
    config = yaml.load(open('StmServerConfig.yml', 'rb').read())
    for param in ['server','port','command_store'] :
        if param in config['stm']:
            setattr(args,param,config['stm'][param])
except Exception as e:
    print("Could not load config file: " + str(e) )

'''command line args'''
cline_args = parser.parse_args()
for param in ['server','port','command_store'] :
    val = getattr(cline_args,param)
    if val != None :
        setattr(args,param,val)

server = StateMachineServer(args.server,args.port, args.command_store)
time.sleep(1)
server.start()

while True :
    time.sleep(1)
    
