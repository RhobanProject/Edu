#include <stdlib.h>
#include <stdio.h>
#include <util/time.h>
#include <main/main.h>
#include <motor/motor.h>

void motor_release()
{
    int id;

    for (id=0; id<motor_count; id++) {
        motors[id].goal_torque = 0;
    }

    sleep_ms(100);
}

REGISTER_COMMAND(release, motor_release, "Relâche tous les moteurs");
