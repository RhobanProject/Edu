#include <cstdlib>
#include <math.h>
#include "ClockWalk.h"

// Conversions
#ifndef DEG2RAD
#define DEG2RAD(x) (((x)/180.0)*M_PI);
#endif
#ifndef RAD2DEG
#define RAD2DEG(x) (((x)/M_PI)*180.0);
#endif

#define R_MIN 10.0
#define R_MAX (L1+L2+L3-0.1)

#define CLOCKABS(x) (((x)>0)?(x):(-(x)))

/**
 * Vous pouvez écrire du code qui sera exécuté à 
 * l'initialisation ici
 */
ClockWalk::ClockWalk(double L1_, double L2_, double L3_)
    : L1(L1_), L2(L2_), L3(L3_),
    t(0), timeGain(1.0), offset(-16), offsetAnkle(-6),
    phase(0.5), 
    lR(0), lPhi(0), rR(0), rPhi(0),
    riseGain(3.0), stepGain(40), ampGain(2.0),
    barPhase(0.06), barOffset(0.0), barGain(15),
    leftGain(1.0), rightGain(1.0),
    legGap(10), barDelta(0.05)
{
    // Rising
    rise.addPoint(0,0);
    rise.addPoint(0.1,1);
    rise.addPoint(0.2,0);
    rise.addPoint(1,0);

    // Stepping
    step.addPoint(0,0);
    step.addPoint(0.05,0);
    step.addPoint(0.15,1);
    step.addPoint(1,0);

    // Bar
    barSpline(barDelta);
}
    
void ClockWalk::barSpline(double delta)
{
    bar.clear();
    bar.addPoint(0, 0);
    bar.addPoint(delta, 1);
    bar.addPoint(0.5, 0);
    bar.addPoint(0.5+delta, -1);
    bar.addPoint(1, 0);
}

void ClockWalk::loadConfig(ConfigFile &config)
{
    config.read("walk", "legGap", 5, legGap);
    config.read("walk", "offset", -16, offset);
    config.read("walk", "offsetAnkle", -6, offsetAnkle);

    config.read("walk", "phase", 0.5, phase);

    config.read("walk", "barPhase", 0.06, barPhase);
    config.read("walk", "barGain", 5.0, barGain);
    config.read("walk", "barOffset", 0.0, barOffset);

    config.read("walk", "timeGain", 1.0, timeGain);
    config.read("walk", "riseGain", 3.0, riseGain);
    config.read("walk", "stepGain", 38.0, stepGain);
    config.read("walk", "ampGain", 2.0, ampGain);

    config.read("walk", "leftGain", 1.0, leftGain);
    config.read("walk", "rightGain", 1.0, rightGain);
}

void ClockWalk::computeIK(double X, double Z, double phi, double *t1, double *t2, double *t3)
{
    double phiRad = DEG2RAD(phi);
    double x = X-L3*sin(phiRad);
    double z = Z-L3*cos(phiRad);

    double a0_2 = x*x+z*z;
    double a0 = sqrt(a0_2);
    double l1_2 = L1*L1;
    double l2_2 = L2*L2;
    double alpha = atan2(x, z);

    *t1 = -(acos((a0_2+l1_2-l2_2)/(2*a0*L1))+alpha);
    *t2 = M_PI-acos((l1_2+l2_2-a0_2)/(2*L1*L2));
    *t3 = -phiRad - (*t1) - (*t2);

    *t1 = RAD2DEG(*t1);
    *t2 = RAD2DEG(*t2);
    *t3 = RAD2DEG(*t3);
}

void ClockWalk::computeIKpolar(double r, double phi, double *t1, double *t2, double *t3)
{
    double phiRad = DEG2RAD(phi);

    if (r > R_MAX) {
        r = R_MAX;
    }
    if (r < R_MIN) {
        r = R_MIN;
    }

    computeIK(r*sin(phiRad), r*cos(phiRad), phi, t1, t2, t3);
}

void ClockWalk::tick(double elapsed)
{
    t += elapsed*timeGain;

    while (t > 1) t -= 1;
    while (t < 0) t += 1;

    // Sinus
    double lRise = L1+L2+L3-rise.getMod(t)*riseGain*CLOCKABS(ampGain);
    double lStep = step.getMod(t)*stepGain*ampGain*leftGain;
    double l1, l2, l3;
    computeIKpolar(lR+lRise, offset+lPhi+lStep, &l1, &l2, &l3);

    a_l1 = l1;
    a_larm = l1*0.3;
    a_l2 = l2;
    a_l3 = -l3-offsetAnkle;
    a_lhip = -legGap;

    double rRise = L1+L2+L3-rise.getMod(t+phase)*riseGain*CLOCKABS(ampGain);
    double rStep = step.getMod(t+phase)*stepGain*ampGain*rightGain;
    double r1, r2, r3;
    computeIKpolar(rR+rRise, offset+rPhi+rStep, &r1, &r2, &r3);

    a_r1 = -r1;
    a_rarm = -r1*0.3;
    a_r2 = -r2;
    a_r3 = r3+offsetAnkle;
    a_rhip = legGap;

    double b = bar.getMod(t+barPhase)*barGain*ampGain;
    a_bar = b+barOffset;
}