#include <cstdlib>
#include <math.h>
#include "CartWalk.h"

// Conversions
#ifndef DEG2RAD
#define DEG2RAD(x) (((x)/180.0)*M_PI);
#endif
#ifndef RAD2DEG
#define RAD2DEG(x) (((x)/M_PI)*180.0);
#endif

#define CARTABS(x) (((x)>0)?(x):(-(x)))
#define CARTMIN(a,b) ((a)<(b) ? (a) : (b))
#define CARTMAX(a,b) ((a)>(b) ? (a) : (b))

namespace Rhoban
{
    /**
     * Vous pouvez écrire du code qui sera exécuté à 
     * l'initialisation ici
     */
    CartWalk::CartWalk(double L1, double L2)
        : zOffset(3.0), yOffset(0.0), yLat(0.0), xOffset(0.0),
        swingGain(0.0), swingPhase(0.0), swingHeight(0.0),
        riseGain(0.0), hipOffset(0.0),
        t(0.0), timeGain(2.0),
        stepGain(0.0), lateralStepGain(0.0),
        turn(0.0), armsGain(0)
    {
        legModel.femurLength = L1;
        legModel.tibiaLength = L2;
    }

    void CartWalk::loadConfig(ConfigFile &config)
    {
#define CARTWALK_CONFIG(entry) \
        config.read("walk", #entry, entry, entry)

        CARTWALK_CONFIG(timeGain);
        CARTWALK_CONFIG(zOffset);
        CARTWALK_CONFIG(yOffset);
        CARTWALK_CONFIG(yLat);
        CARTWALK_CONFIG(xOffset);
        CARTWALK_CONFIG(swingGain);
        CARTWALK_CONFIG(riseGain);
        CARTWALK_CONFIG(hipOffset);
        CARTWALK_CONFIG(swingPhase);
        CARTWALK_CONFIG(swingHeight);
        CARTWALK_CONFIG(stepGain);
        CARTWALK_CONFIG(lateralStepGain);
        CARTWALK_CONFIG(turn);
        CARTWALK_CONFIG(armsGain);

        // Swinging
        swing.addPoint(0, 0);
        swing.addPoint(0.25, 1);
        swing.addPoint(0.5, 0);
        swing.addPoint(0.75, -1);
        swing.addPoint(1, 0);

        // Rising
        rise.addPoint(0, 0);
        rise.addPoint(0.2, 1.0);
        rise.addPoint(0.3, 1.0);
        rise.addPoint(0.4, 0.0);
        rise.addPoint(1.0, 0.0);

        // Stepping
        step.addPoint(0, -0.5);
        step.addPoint(0.2, -0.5);
        step.addPoint(0.3, 0.5);
        step.addPoint(1.0, -0.5);
    }

    void CartWalk::tick(double elapsed)
    {
        t += elapsed*timeGain;

        while (t > 1) t -= 1;
        while (t < 0) t += 1;

        // Computing turn differential
        double leftStepGain, rightStepGain;
        leftStepGain = rightStepGain = 1.0;
        if (turn > 0.1) {
            double l1 = 1.0/tan(DEG2RAD(turn));
            double ratio = l1/(8.0+yOffset);
            if (turn < 0) {
                rightStepGain = ratio;
            } else {
                leftStepGain = ratio;
            }
        }

        double lX, lY, lZ, rX, rY, rZ;
        double swingValue = swing.getMod(t+swingPhase);

        // Computing X, Y & Z
        lX = xOffset + step.getMod(t)*stepGain*leftStepGain;
        lY = yOffset + yLat - swingGain*swingValue + lateralStepGain*step.getMod(t);
        lZ = zOffset + rise.getMod(t)*riseGain + swingHeight*CARTABS(swingValue);

        rX = xOffset + step.getMod(t+0.5)*stepGain*rightStepGain;
        rY = yOffset - yLat + swingGain*swingValue - lateralStepGain*step.getMod(t+0.5);
        rZ = zOffset + rise.getMod(t+0.5)*riseGain + swingHeight*CARTABS(swingValue);

        // Computing IK
        LegAngles left = legModel.compute(lX, lY, lZ);
        LegAngles right = legModel.compute(rX, rY, rZ);

        // Setting angles
        a_l_hip_pitch = -RAD2DEG(left.hipLong) - hipOffset;
        a_l_hip_roll = -RAD2DEG(left.hipLat);
        a_l_hip_yaw = step.getMod(t)*turn;
        a_l_knee = RAD2DEG(left.knee);
        a_l_foot_pitch = RAD2DEG(left.ankleLong);
        a_l_foot_roll = RAD2DEG(left.ankleLat);

        a_r_hip_pitch = RAD2DEG(right.hipLong) + hipOffset;
        a_r_hip_roll = RAD2DEG(right.hipLat);
        a_r_hip_yaw = step.getMod(t+0.5)*turn;
        a_r_knee = -RAD2DEG(right.knee);
        a_r_foot_pitch = -RAD2DEG(right.ankleLong);
        a_r_foot_roll = -RAD2DEG(right.ankleLat);

        a_l_arm = -step.getMod(t)*armsGain;
        a_r_arm = step.getMod(t+0.5)*armsGain;
    }
}
