#include "SigmabanWalk.h"

SigmabanWalk::SigmabanWalk(double L1, double L2, double L3)
    : ClockWalk(L1, L2, L3), robot(NULL)
{
}

void SigmabanWalk::setRobot(Robot *robot_)
{
    robot = robot_;
}

void SigmabanWalk::tick(double elapsed)
{
    static double pitchSmooth = 0;

    if (robot != NULL) {
        Sensors *sensors = robot->getSensors();

        double pitch = sensors->get("Pitch")->getValue();
        pitchSmooth = pitch*0.01+pitchSmooth*0.99;
        pitchSmooth *= 0.7;

        ClockWalk::tick(elapsed);

        Motors *motors = robot->getMotors();

        motors->get("Cuisse G")->setAngle(a_l1);
        motors->get("Epaule G Long")->setAngle(a_larm);
        motors->get("Genou G")->setAngle(a_l2);
        motors->get("Cheville G")->setAngle(a_l3);
        motors->get("Cuisse D")->setAngle(a_r1);
        motors->get("Epaule D Long")->setAngle(a_rarm);
        motors->get("Genou D")->setAngle(a_r2);
        motors->get("Cheville D")->setAngle(a_r3);
        motors->get("Hanche G Lat")->setAngle(a_lhip);
        motors->get("Hanche D Lat")->setAngle(a_rhip);
        motors->get("Hanche G Rot")->setAngle(a_lhip_rot);
        motors->get("Hanche D Rot")->setAngle(a_rhip_rot);
        motors->get("Bassin Lat")->setAngle(a_bar);
        motors->get("Pied D")->setAngle(a_r3lat);
        motors->get("Pied G")->setAngle(a_l3lat);
        motors->get("Bassin Long")->setAngle(a_long + pitchSmooth);
    }
}

void SigmabanWalk::zero()
{
    if (robot != NULL) {
        Motors *motors = robot->getMotors();

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
        motors->get("Hanche G Rot")->setAngle(0);
        motors->get("Hanche D Rot")->setAngle(0);
        motors->get("Pied D")->setAngle(0);
        motors->get("Pied G")->setAngle(0);
        motors->get("Bassin Long")->setAngle(0);
    }
}
