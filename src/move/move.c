#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <util/time.h>
#include <motor/motor.h>
#include <main/main.h>
#include <config.h>

static void move_init();
static void move_tick(unsigned int t);

/**
 * Initialise le mouvement
 */
static void move_init()
{
    // TODO: Initialiser les structures etc.
}

/**
 * Apellé plusieurs fois par seconde (cf SCHEDULER_FREQUENCY),
 * fais évoluer le mouvement
 */
static void move_tick(unsigned int t)
{
    // TODO: Effectuer des actions
}

/**
 * Lance le mouvement
 */
void move_run()
{
    unsigned int t = 0;
    
    move_init();

    while (1) {
        move_tick(t++);
        sleep_ms(1000/SCHEDULER_FREQUENCY);
    }
}

REGISTER_COMMAND(move, move_run, "Lance un mouvement");

