
/*************************************************
 * Publicly released by Rhoban System, May 2013
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
#include <rhoban/vision/Vision.hpp>

#include <opencv2/opencv.hpp>

using namespace std;
using namespace Rhoban;

int main(int argc, char **argv)
{
	try
	{
		Connection connection;
		connection.connectTo("192.168.16.120", 7777);

		Rhoban::Vision vision(&connection);

		while(true)
		{
			cv::Mat png_frame = vision.grabPngFrame(argv[1], 320, 240);
			cv::imshow("png", png_frame);
			cv::waitKey(33);
		}
	}
	catch(string exc)
	{
		cout << "Received exception " << exc << endl;
		exit(0);
	}

}
