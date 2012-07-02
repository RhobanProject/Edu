#define MOTORS_MAIN
#include <stdlib.h>
#include <stdio.h>
#include <config.h>
#include <util/time.h>
#include <pthread.h>
#include <motor/motor.h>
#include <motor/commands.h>
#include <dynamixel/dynamixel.h>

#define MIN(a, b) ((a)<(b) ? (a) : (b))
#define MAX(a, b) ((a)>(b) ? (a) : (b))

#define TORQUE2DOUBLE(torque) (((double)torque)/0x3ff)
#define DOUBLE2TORQUE(torque) ((unsigned short)(MIN(1, MAX(0, torque))*0x3ff))

#define ANGLE2DOUBLE(angle) (((double)(angle-0x1ff)/0x3ff)*300)
#define DOUBLE2ANGLE(angle) ((unsigned short)((MIN(150, MAX(-150, angle))/300.0)*0x3ff+0x1ff))

int motor_count;

static pthread_t dispatching_thread;

static int dispatching = 0;

static void *motor_dispatching(void *data);
static void motor_process(int id);

__attribute__((packed))
struct motor_registers {
    unsigned short angle;
    unsigned short speed;
    unsigned short torque;
};

/**
 * Processes the motor id (get & update)
 */
static void motor_process(int id)
{
    struct motor_registers registers;
    struct motor *motor = &motors[id];

    // Gets the current values
    dxl_read(id, REGISTER_PRESENT, &registers, sizeof(struct motor_registers));
    motor->angle = ANGLE2DOUBLE(registers.angle);

    // Write the goal values
    registers.angle = DOUBLE2ANGLE(motor->goal_angle);
    registers.torque = DOUBLE2TORQUE(motor->goal_torque);
    registers.speed = 1024;
    dxl_write(id, REGISTER_GOAL, &registers, sizeof(struct motor_registers));
}

/**
 * Monitoring thread
 */
static void *motor_dispatching(void *data)
{
    int id = 0;

    while (dispatching) {
        for (id=1; id<motor_count; id++) {
            motor_process(id);
        }

        sleep_ms(1000/DISPATCHER_FREQUENCY);
    }

    return NULL;
}

void motor_start_dispatching()
{
    motor_count = (sizeof(motors)/sizeof(struct motor));
    motor_release();

    dispatching = 1;
    pthread_create(&dispatching_thread, NULL, motor_dispatching, NULL);
}

void motor_stop_dispatching()
{
    motor_release();

    dispatching = 0;
    pthread_join(dispatching_thread, NULL);
}
