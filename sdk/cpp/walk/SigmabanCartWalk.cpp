#include "SigmabanCartWalk.h"

SigmabanCartWalk::SigmabanCartWalk(double L1, double L2)
    : CartWalk(L1, L2), robot(NULL)
{
}

void SigmabanCartWalk::setRobot(Robot *robot_)
{
    robot = robot_;
}

void SigmabanCartWalk::tick(double elapsed)
{
    static double pitchSmooth = 0;

    if (robot != NULL) {
        Sensors *sensors = robot->getSensors();

        double pitch = sensors->get("Pitch")->getValue();
        pitchSmooth = pitch*0.01+pitchSmooth*0.99;
        pitchSmooth *= 0.7;

        CartWalk::tick(elapsed);

        Motors *motors = robot->getMotors();

        motors->get("Cuisse G")->setAngle(a_l_hip_pitch);
        motors->get("Epaule G Long")->setAngle(a_l_arm);
        motors->get("Genou G")->setAngle(a_l_knee);
        motors->get("Cheville G")->setAngle(a_l_foot_pitch);
        motors->get("Cuisse D")->setAngle(a_r_hip_pitch);
        motors->get("Epaule D Long")->setAngle(a_r_arm);
        motors->get("Genou D")->setAngle(a_r_knee);
        motors->get("Cheville D")->setAngle(a_r_foot_pitch);
        motors->get("Hanche G Lat")->setAngle(a_l_hip_roll);
        motors->get("Hanche D Lat")->setAngle(a_r_hip_roll);

        motors->get("Hanche G Rot")->setAngle(a_l_hip_yaw);
        motors->get("Hanche D Rot")->setAngle(a_r_hip_yaw);

        motors->get("Pied G")->setAngle(a_l_foot_roll);
        motors->get("Pied D")->setAngle(a_r_foot_roll);
    }
}

void SigmabanCartWalk::zero()
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
