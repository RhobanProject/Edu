#ifndef _CONFIG_H
#define _CONFIG_H

/**
 * Numéro du device à utiliser
 *
 * Sous windows, représente le numéro du port COM (COM1, COM2...)
 * Sous linux, le numéro du device ttyUSB (/dev/ttyUSB0, /dev/ttyUSB1...)
 */
#define DEVICE_INDEX 0

/**
 * Numéro de la baudrate à utiliser
 */
#define BAUDRATE_NUM 1

/**
 * Vitesse du scheduler de mouvement (en Hz)
 */
#define SCHEDULER_FREQUENCY 100

#endif // _CONFIG_H
