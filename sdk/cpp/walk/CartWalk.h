#ifndef _CART_WALK_H
#define _CART_WALK_H

#include <configfile/ConfigFile.h>
#include "Function.h"
#include "SigmabanLeg.h"

namespace Rhoban
{
    class CartWalk
    {
        public:
            CartWalk(double L1, double L2);
            void loadConfig(ConfigFile &config);

            // Current time
            double t;

            // Splines
            Function rise;
            Function step;
            Function swing;

            // Time gain (frequency)
            double timeGain;

            // Position offset
            double xOffset;
            double yOffset;
            double yLat;
            double zOffset;
            double hipOffset;

            // Rising of the steps
            double riseGain;

            // Swinging
            double swingGain;
            double swingHeight;
            double swingPhase;

            // Arms
            double armsGain;

            // Stepping
            double stepGain;

            // Lateral stepping
            double lateralStepGain;

            // Turning
            double turn;

            // Sizes
            double L1, L2, L3;

            virtual void tick(double elapsed);

            float a_l_hip_pitch, a_l_knee, a_l_foot_pitch, a_l_arm, a_l_hip_roll, a_l_hip_yaw, a_l_foot_roll;
            float a_r_hip_pitch, a_r_knee, a_r_foot_pitch, a_r_arm, a_r_hip_roll, a_r_hip_yaw, a_r_foot_roll;

        protected:
            SigmabanLeg legModel;
    };
}

#endif // _CART_WALK_H
