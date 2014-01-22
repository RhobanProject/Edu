#ifndef _CLOCK_WALK_H
#define _CLOCK_WALK_H

#include <configfile/ConfigFile.h>
#include "Function.h"

class ClockWalk
{
    public:
        ClockWalk(double L1, double L2, double L3);
        void loadConfig(ConfigFile &config);

        virtual void tick(double elapsed);

    protected:
        Function rise;
        Function step;
        Function bar;

        // Sizes
        double L1, L2, L3;

        void barSpline(double delta);
        void computeIK(double X, double Z, double phi, double *t1, double *t2, double *t3);
        void computeIKpolar(double r, double phi, double *t1, double *t2, double *t3);

        // Current time
        double t;

        // Time gain
        double timeGain;

        // Offset for body position and ankle position
        double offset;
        double offsetAnkle;

        // Step phases
        double phase;

        // Left & right offset for rise and phi
        double lR, lPhi, rR, rPhi; 

        // Rise height
        double riseGain;

        // Step size
        double stepGain;
        double ampGain;

        // Bar phase, offset & gain
        float barPhase;
        float barOffset; 
        double barGain;

        // Left/Right gain
        float leftGain;
        float rightGain;

        // Leg Gap
        float legGap;

        // Bar delta (for its spline)
        double barDelta;

        // Turning
        double turn;

        // Computed angles
        float a_l1, a_l2, a_l3, a_larm, a_lhip, a_lhip_rot;
        float a_r1, a_r2, a_r3, a_rarm, a_rhip, a_rhip_rot;
        float a_bar;
};

#endif // _CLOCK_WALK_H
