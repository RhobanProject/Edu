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
#include <main/Command.h>

#include <ticks.h>

#include <rhoban/robot/Robots.h>

using namespace std;
using namespace Rhoban;

int main(int argc, char **argv)
{
	try
	{
	Robots robots;

	cout << "Starting " << endl;

	// Charge la configuration et connecte les robots
	robots.loadYaml("config.yml");

	Robot * robot = robots["bras"];
	Motors * motors = robot->getMotors();
	Moves * moves = robot->getMoves();

	cout << "Starting motors " << endl;
	motors->start(30);
	syst_wait_ms(1000);

	cout << "Putting all motors compliant " << endl;
	robot->allCompliant();
	syst_wait_ms(1000);

	cout << "Recording for ten seconds..." << endl;
	moves->startRecordingSpline();

	syst_wait_ms(1000);
	cout << "... done." << endl;
	moves->stopRecordingSpline();

	cout << "Retrieving recorded spline" << endl;
	LinearSpline spline = moves->getRecordedSpline();

	cout << "Sending back slower spline" << endl;
	spline.speed_factor *= 0.5;
	moves->setSpline(spline);

	cout << "Taking initial position..." << endl;
	motors->goToInit();
	cout << "...done." << endl;
	syst_wait_ms(1000);

	cout << "Playing slower spline..." << endl;
	moves->playSpline();

	syst_wait_ms(2000);
	cout << "...done." << endl;
	moves->stopSpline();

	cout << "Putting all motors compliant " << endl;
	robot->allCompliant();
	syst_wait_ms(1000);

	cout << "Bye bye" << endl;
	robots.stop();
	}
	catch(string exc)
	{
		cout << "Received exception " << exc << endl;
		exit(0);
	}

}
