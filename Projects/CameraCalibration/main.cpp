/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <util.h>
#include <main/Command.h>

#include <ticks.h>

#include <timing/sleep.h>
#include <rhoban/robot/Robots.h>

using namespace std;
using namespace Rhoban;

bool getAverageXY(Vision *vision, double &X, double &Y, int iterations = 50)
{
    vector<double> Xs;
    vector<double> Ys;
    int i;
    vision->grabBallInfo(false);

    for (i=0; i<iterations; i++) {
        vision->grabBallInfo(false);
        if (vision->getIsBallDetected()) {
            Xs.push_back(vision->getBallRelPosX());
            Ys.push_back(vision->getBallRelPosY());
        }
        sleep_ms(50);
    }

    if (Xs.size() < iterations*7/8) {
        return false;
    }

    sort(Xs.begin(), Xs.end());
    sort(Ys.begin(), Ys.end());

    X = Xs[iterations/2];
    Y = Ys[iterations/2];

    return true;
}

int main(int argc, char **argv)
{
    try
    {
        vector<string> motors;
        vector<float> angles;
        double X, Y;
        double pan, tilt;
        Robots robots;
        pan = tilt = 0;

        cout << "Starting " << endl;

        // Charge la configuration et connecte les robots
        robots.loadYaml("config.yml");

        Robot *robot = robots["robot"];
        Vision *vision = robot->getVision();

        // Going to zero position
        // robot->getConnection()->SchedulerTakeRefPosition("Zero", 1000);

        // Initializing motor vectors
        motors.push_back("TeteLat");
        motors.push_back("TeteAlt");
        angles.push_back(pan);
        angles.push_back(tilt);
        Connection *connection = robot->getConnection();
        connection->SetRelativeServosAngles(motors, angles);

        cout << "* Step 1: centering the ball to 0,0" << endl;
        double distance = 1.0;

        do {
            if (getAverageXY(vision, X, Y, 10)) {
                pan -= X*10;
                tilt += Y*10;
                pan = my_min(30, my_max(-30, pan));
                tilt = my_min(30, my_max(-30, tilt));
                angles[0] = pan;
                angles[1] = tilt;
                connection->SetRelativeServosAngles(motors, angles);
                distance = sqrt(X*X+Y*Y);
                cout << "D=" << distance << endl;;
            } else {
                cout << "* No ball *" << endl;
            }
        } while (distance > 0.01);

        cout << "*Step 2: moving along X" << endl;
        double x;

        for (x=-30; x<=30; x+=1) {
            double alpha = pan+x;
            angles[0] = alpha;
            connection->SetRelativeServosAngles(motors, angles);
            getAverageXY(vision, X, Y, 30);
            cout << x << ";" << X << endl;
        }

    } catch(string exc) {
        cout << "Received exception " << exc << endl;
        exit(0);
    }

}
