#ifndef _SIGMABAN_WALK_H
#define _SIGMABAN_WALK_H

#include <rhoban/robot/Robots.h>
#include "ClockWalk.h"

class SigmabanWalk : public ClockWalk
{
    public:
        SigmabanWalk(double L1, double L2, double L3);

        // Set the robot instance to animate
        void setRobot(Robot *robot);

        // Ticks the walk
        void tick(double elapsed);

        // Set all the motors to 0
        void zero();

    protected:
        Robot *robot;
};

#endif
