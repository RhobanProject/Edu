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

    double leftStep, rightStep;
    double lX, lY, lZ, rX, rY, rZ;
    double swingValue = swing.getMod(t+swingPhase);

    leftStep = rightStep = 1.0;
    if (turn > 0.1) {
        double l1 = 1.0/tan(DEG2RAD(turn));
        double ratio = l1/(8.0+yOffset);
        if (turn < 0) {
            rightStep = ratio;
        } else {
            leftStep = ratio;
        }
    }

    lX = xOffset + step.getMod(t)*stepGain*leftStep;
    lY = yOffset + yLat - swingGain*swingValue + lateralStepGain*step.getMod(t);
    lZ = zOffset + rise.getMod(t)*riseGain + swingHeight*CARTABS(swingValue);

    rX = xOffset + step.getMod(t+0.5)*stepGain*rightStep;
    rY = yOffset - yLat + swingGain*swingValue - lateralStepGain*step.getMod(t+0.5);
    rZ = zOffset + rise.getMod(t+0.5)*riseGain + swingHeight*CARTABS(swingValue);

    LegAngles left = legModel.compute(lX, lY, lZ);
    LegAngles right = legModel.compute(rX, rY, rZ);

    a_l1 = -RAD2DEG(left.hipLong) - hipOffset;
    a_lhip = -RAD2DEG(left.hipLat);
    a_l2 = RAD2DEG(left.knee);
    a_l3 = RAD2DEG(left.ankleLong);
    a_l3lat = RAD2DEG(left.ankleLat);
    a_lhip_rot = step.getMod(t)*turn;

    a_r1 = RAD2DEG(right.hipLong) + hipOffset;
    a_rhip = RAD2DEG(right.hipLat);
    a_r2 = -RAD2DEG(right.knee);
    a_r3 = -RAD2DEG(right.ankleLong);
    a_r3lat = -RAD2DEG(right.ankleLat);
    a_rhip_rot = step.getMod(t+0.5)*turn;

    a_larm = -step.getMod(t)*armsGain;
    a_rarm = step.getMod(t+0.5)*armsGain;

    // printf("L=%g / R=%g\n", a_larm, a_rarm);
    // printf("L1=%g L2=%g L3=%g\n", a_l1, a_l2, a_l3);
}
