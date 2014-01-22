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
    if (robot != NULL) {
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
        motors->get("Bassin Lat")->setAngle(a_bar);
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
    }
}
