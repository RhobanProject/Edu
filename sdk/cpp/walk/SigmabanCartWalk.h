#ifndef _SIGMABAN_CART_WALK_H
#define _SIGMABAN_CART_WALK_H

#include <rhoban/robot/Robots.h>
#include "CartWalk.h"

class SigmabanCartWalk : public CartWalk
{
    public:
        SigmabanCartWalk(double L1, double L2);

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
