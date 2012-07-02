#ifndef _MOTOR_H
#define _MOTOR_H

#define REGISTER_GOAL           30
#define REGISTER_PRESENT        36

/**
 * Représente un moteur (abstrait)
 *
 * Les angles sont compris entre -180 et +180
 * Le torque est la force appliquée, comprise en 0 et 1
 */
struct motor {
    // Angle d'initialisation
    double ini_angle;
    // Direction
    char direction;
    // Torque cible
    double goal_torque;
    // Angle minimum
    double min_angle;
    // Angle maximum
    double max_angle;
    // Angle courant
    double angle;
    // Angle cible
    double goal_angle;
};

/**
 * Ensemble des moteurs et de leur configuration
 */
#ifdef MOTORS_MAIN
struct motor motors[] = {
    {},
    {0, 1, 0}, // ID=1
    {0, 1, 0}, // ID=2
    {0, 1, 0}, // ID=3
    {0, 1, 0}, // ID=4
    {0, 1, 0}, // ID=5
    {0, 1, 0}, // ID=6
    {0, 1, 0}, // ID=7
    {0, 1, 0}, // ID=8
    {0, 1, 0}, // ID=9
    {0, 1, 0}, // ID=10
    {0, 1, 0}, // ID=11
    {0, 1, 0}, // ID=12
    {0, 1, 0}, // ID=13
    {0, 1, 0}, // ID=14
    {0, 1, 0}, // ID=15
    {0, 1, 0}, // ID=16
    {0, 1, 0}, // ID=17
    {0, 1, 0}, // ID=18
};
#else
extern struct motor motors[];
#endif

extern int motor_count;

/**
 * Lance le dispatching des moteurs
 */
void motor_start_dispatching();

/**
 * Stoppe le dispatching des moteurs
 */
void motor_stop_dispatching();

#endif // _MOTOR_H
