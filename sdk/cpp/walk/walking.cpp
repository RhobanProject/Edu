#include <cstdlib>
#include <math.h>
#include "walking.h"
#include "function.h"

#define DEG2RAD(x) (((x)/180.0)*M_PI);
#define RAD2DEG(x) (((x)/M_PI)*180.0);

#define ENABLE_ACC      22

Function rise;
Function step;
Function bar;

double t = 0.0; //, "Temps", 0.0);

double offset = -16; // , "Offset", -16);
double offsetC = -6; // , "OffsetC", -6);

double phase = 0.5; // , "Sinus phases", 0.5);

double timeGain = 1.0; //, "TimeGain", 2.0);

double lR = 0.0; //, "FordwardAmplitude", 0.0);
double lPhi = 0.0; // , "FordwardAmplitude", 0.0);

double rR = 0.0; // , "FordwardAmplitude", 0.0);
double rPhi = 0.0; //, "FordwardAmplitude", 0.0);

double riseGain = 3.0; //, "Rise height", 3.0);
double stepGain = 38; // , "Rise ste", 38.0);
double barGain = 15; // , "Rise ste", 15.0);
double ampGain = 2.0; // , "Amp gain", 0.0);

float barPhase = 0.06; //, "BP", 0.06);

float leftGain = 1.0; //, "Left Gain", 1.0);
float rightGain = 1.0; //, "Right Gain", 1.0);
float barOffset = 0.0; //, "Bar Offset", 0.0);

float legGap = 10;

volatile static double barDelta = 0.05;

void loadConfig(ConfigFile &config)
{
    config.read("walk", "legGap", 5, legGap);
    config.read("walk", "offset", -16, offset);
    config.read("walk", "offsetC", -6, offsetC);

    config.read("walk", "phase", 0.5, phase);

    config.read("walk", "barPhase", 0.06, barPhase);
    config.read("walk", "barGain", 5.0, barGain);
    config.read("walk", "barOffset", 0.0, barOffset);

    config.read("walk", "timeGain", 1.0, timeGain);
    config.read("walk", "riseGain", 3.0, riseGain);
    config.read("walk", "stepGain", 38.0, stepGain);
    config.read("walk", "ampGain", 2.0, ampGain);
}
    
void barSpline(double delta)
{
    bar.clear();
    bar.addPoint(0, 0);
    bar.addPoint(delta, 1);
    bar.addPoint(0.5, 0);
    bar.addPoint(0.5+delta, -1);
    bar.addPoint(1, 0);
}

// In cm
#define L1 8.6
#define L2 13.73
#define L3 3
#define GHEIGHT (L1+L2+L3)

#define R_MIN 10.0
#define R_MAX (L1+L2+L3-0.1)

void computeIK(double X, double Z, double phi, double *t1, double *t2, double *t3)
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

void computeIKpolar(double r, double phi, double *t1, double *t2, double *t3)
{
    double phiRad = DEG2RAD(phi);

    if (r > R_MAX) {
        r = R_MAX;
    }
    if (r < R_MIN) {
        r = R_MIN;
    }

    computeIK(r*sin(phiRad), r*cos(phiRad)+L1+L2+L3-GHEIGHT, phi, t1, t2, t3);
}

/**
 * Vous pouvez écrire du code qui sera exécuté à 
 * l'initialisation ici
 */
void setup()
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

/**
 * Foncton appellée à 50hz, c'est ici que vous pouvez mettre
 * à jour les angles moteurs etc.
 */
void tick(Robot &robot)
{
    t += 0.01*timeGain; // 20ms

    while (t > 1) t -= 1;
    while (t < 0) t += 1;

    /*
       double err = normAccX;
       balanceAcc += err;
       if (balanceAcc < -10) balanceAcc = -10;
       if (balanceAcc > 10) balanceAcc = 10;
       */

    Motors *motors = robot.getMotors();

    // Sinus
    double lRise = L1+L2+L3-rise.getMod(t)*riseGain*abs(ampGain);
    double lStep = step.getMod(t)*stepGain*ampGain*leftGain;
    double l1, l2, l3;
    computeIKpolar(lR+lRise, offset+lPhi+lStep, &l1, &l2, &l3);

    motors->get("Cuisse G")->setAngle(l1);
    motors->get("Epaule G Long")->setAngle(l1*0.3);
    motors->get("Genou G")->setAngle(l2);
    motors->get("Cheville G")->setAngle(-l3-offsetC);

    double rRise = L1+L2+L3-rise.getMod(t+phase)*riseGain*abs(ampGain);
    double rStep = step.getMod(t+phase)*stepGain*ampGain*rightGain;
    double r1, r2, r3;
    computeIKpolar(rR+rRise, offset+rPhi+rStep, &r1, &r2, &r3);
    
    motors->get("Cuisse D")->setAngle(-r1);
    motors->get("Epaule D Long")->setAngle(-r1*0.3);
    motors->get("Genou D")->setAngle(-r2);
    motors->get("Cheville D")->setAngle(r3+offsetC);

    motors->get("Hanche G Lat")->setAngle(-legGap);
    motors->get("Hanche D Lat")->setAngle(legGap);

    double b = bar.getMod(t+barPhase)*barGain*ampGain;
    motors->get("Bassin Lat")->setAngle(b+barOffset);
}

void zero(Robot &robot)
{
    Motors *motors = robot.getMotors();
    motors->get("Cuisse G")->setAngle(0);
    motors->get("Epaule G Long")->setAngle(0);
    motors->get("Genou G")->setAngle(0);
    motors->get("Cheville G")->setAngle(0);
    motors->get("Cuisse D")->setAngle(0);
    motors->get("Epaule D Long")->setAngle(0);
    motors->get("Genou D")->setAngle(0);
    motors->get("Cheville D")->setAngle(0);
    motors->get("Hanche G Lat")->setAngle(0);
    motors->get("Hanche D Lat")->setAngle(0);
    motors->get("Bassin Lat")->setAngle(0);
}
