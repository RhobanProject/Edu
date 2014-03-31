#ifndef _CART_WALK_H
#define _CART_WALK_H

#include <configfile/ConfigFile.h>
#include "Function.h"
#include "SigmabanLeg.h"

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

        // Parameters
        double timeGain;

        double stepGain;
        double lateralStepGain;

        double xOffset;
        double yOffset;
        double yLat;
        double zOffset;
        double hipOffset;

        double riseGain;

        double swingGain;
        double swingHeight;
        double swingPhase;

        double turn;

        double armsGain;

        // Sizes
        double L1, L2, L3;

        virtual void tick(double elapsed);
        
        float a_l1, a_l2, a_l3, a_larm, a_lhip, a_lhip_rot;
        float a_r1, a_r2, a_r3, a_rarm, a_rhip, a_rhip_rot;
        float a_l3lat, a_r3lat;

    protected:
        SigmabanLeg legModel;
};

#endif // _CART_WALK_H
