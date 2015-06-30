
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
    /* Create a window */
    cv::namedWindow("Example2", CV_WINDOW_AUTOSIZE);
    /* capture frame from video file */
    cv::VideoCapture capture(argv[1]);
    cout << argv[1] << endl;
    if (!capture.isOpened())
		cout << "cannot open camera";
	
	else
	{
		cout << "camera stream opened"; 
		cv::Mat frame;

		while(1) 
		{
		    capture >> frame;
		    cv::imshow("Example2", frame);
		    char c = cv::waitKey(33);
		    if(c==27) break;
		}

		capture.release();
    }
    
    cvDestroyWindow("Example2");
}
