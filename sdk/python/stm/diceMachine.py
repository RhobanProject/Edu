# -*- coding: utf-8 -*-

'''
Created on 16 ao�t 2012

@author: Hugo
'''

import stm

"""Simulates a game where we throw a dice and win if we make a six in less than n tries"""
   
import random

class TheDiceMachine(stm.StateMachine):

    name = "TheDiceMachine"
    
    class States:
        (Initial , Final, Lost , Won) = range(0,4)

    def throw_dice(self):
        self.dice = random.randint(1,6)
        print("Dice is ", self.dice)
        self.tries += 1
        
    def enter(self):
        if self.state == self.States.Initial :
            self.tries = 0
            self.dice = 0

    def bye(self):
        if self.state == self.States.Lost :
            print('Game Over')
        elif self.state == self.States.Won :
            print('Game Over')
            
    def loop(self):
        if self.state == self.States.Initial :
            self.throw_dice()
        elif self.state == self.States.Won:
            print('You Win')
        elif self.state == self.States.Lost:
            print('You Lose')
        elif self.state == self.States.Final:
            print('Over, dice is ', self.dice)
        
    def transition(self):
        if self.state == self.States.Initial :
            if self.tries > 4 :
                return self.States.Lost
            elif self.dice == 6 :
                return self.States.Won
        elif self.state == self.States.Lost:
            return self.States.Final
        elif self.state == self.States.Won:
            return self.States.Final
        return self.state
    
        
            
if __name__ == '__main__':
    the_machine = TheDiceMachine()
    the_machine.run()
