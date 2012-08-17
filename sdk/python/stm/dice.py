# -*- coding: utf-8 -*-

'''
Created on 16 ao�t 2012

@author: Hugo
'''

""" The code corresponds to the following state machine
machine : TheDiceMachine
  
  global: |
      tries
      dice
      void throw_dice() { dice = 7 * rand() / RAND_MAX; }

   freqency : 50

   states:
   -    name: Initial
        start : i = 0
        loop: |
            i += 1
            throw_dice()
        transitions:
           -  condition : i > 5
              do: i = 0
              next: Lost
           -  condition : dice == 6
              next : Won

   -    name : Lost
        loop: print("You've lost");
        exit : print("Game Over")
        transitions:
            - next: Final

   -    name : Won
        loop: print("You've won");
        exit : print("Game Over")
        transitions:
            - next: Final

   -    name : Final
   """
   
import random

class States:
    (Initial , Lost , Won , Final ) = range(0,4)
 
dice = 0
tries = 0

def throw_dice():
    global dice
    global tries
    print('Throwing dice')
    dice = random.randint(1,6)
    tries += 1
    print('Dice is ', dice)

def enter(state):
    global dice
    global tries
    if state == States.Initial :
        tries = 0

def bye(state):
    global dice
    global tries
    if state == States.Lost :
        print('Game Over')
    elif state == States.Won :
        print('Game Over')
                          
def loop(state):
    global dice
    global tries
    if state == States.Initial :
        throw_dice()
    elif state == States.Won:
        print('You Win')
    elif state == States.Lost:
        print('You Lose')
    elif state == States.Final:
        print('Over, dice is ', dice)
        exit(0)
        
def transition(state):
    global dice
    global tries
    if state == States.Initial :
        if tries > 4 :
            return States.Lost
        elif dice == 6 :
            return States.Won
    elif state == States.Lost:
        return States.Final
    elif state == States.Won:
        return States.Final
    return state
            
if __name__ == '__main__':
    state = States.Initial
    while True :
        loop(state)
        newstate = transition(state)
        if newstate != state :
            bye(state)
            enter(newstate)
            state = newstate

