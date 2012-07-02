#include <iostream>
#include <stdio.h>
#include <pthread.h>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <motion/motion.h>

#include <sockets/TCPClient.h>
#include <main/Command.h>

using namespace std;
using namespace cv;
using namespace Rhoban;

// Enable GUI display (window)
#define GUI

// Enable Debug
#define DEBUG

/**
 * Account for goal smoothing
 */
#define ACCOUNT 0.8

/**
 * Goal
 */
double goalX = 0.0;
double goalY = 0.0;

TCPClient *client;

static int cameraIndex = 1;

string window_name = "Capture - Motion detection";

/**
 * Detects and display if necessary
 */
static void detectAndDisplay(Mat frame);
static void *motionThread(void *data);

COMMAND_DEFINE(motion, "Motion tracking")
{
    try {
        motionThread(NULL);
    } catch (string exception) {
        cout << "Error: " << exception << endl;
    }
}

void *motionThread(void *data)
{
    char buffer[128];
    CvCapture* capture;
    Mat frame;

    client = new TCPClient;
    client->connectTo("192.168.16.102", 7777);

#ifdef GUI
    namedWindow(window_name, CV_WINDOW_NORMAL);
#endif

    printf("[Motion] Thread started\n");

    capture = cvCaptureFromCAM(cameraIndex);
    if( capture )
    {
        while( true )
        {
            frame = cvQueryFrame(capture);

            if( !frame.empty() ) { 
                detectAndDisplay(frame);

                sprintf(buffer, "%g, %g\n", goalX, goalY);
                client->transmit(buffer, strlen(buffer));
            }
       
            waitKey(10);
        }
    } else {
        printf("[Motion] No suitable camera\n");
    }

    return NULL;
}

/**
 * Runs the motion thread
 */
void startMotionTracking(int deviceIndex)
{
    pthread_t motion;

    cameraIndex = deviceIndex;

    printf("[Motion] Running motion thread\n");
    pthread_create(&motion, NULL, motionThread, NULL);
}

int has_last = 0;
Mat last_frame;

double bX = 0.0;
double bY = 0.0;

/** @function detectAndDisplay */
static void detectAndDisplay( Mat frame )
{
    double X = 0.0;
    double Y = 0.0;
    int N = 0;

    int x, y;
    std::vector<Rect> faces;
    Mat frame_gray;
    Mat frame_diff;

    cvtColor(frame, frame_gray, CV_BGR2GRAY );
    equalizeHist(frame_gray, frame_gray);
#ifdef GUI
    frame_diff = frame_gray.clone();
#endif

    if (has_last) {
        for (y=0; y<frame_gray.rows; y++)
        {
            for (x=0; x<frame_gray.cols; x++)
            {
                int diff = abs(frame_gray.ptr<uchar>(y)[x]-last_frame.ptr<uchar>(y)[x]);
#ifdef GUI
                frame_diff.ptr<uchar>(y)[x] = diff;
#endif

                if (diff > 50) {
                    N++;
                    X += x;
                    Y += y;
                }
            }
        }

        if (N > 100) {
            X /= N;
            Y /= N;

            bX = ACCOUNT*bX + (1-ACCOUNT)*X;
            bY = ACCOUNT*bY + (1-ACCOUNT)*Y;
        }

#ifdef GUI
        rectangle(frame_diff, Point(bX-3, bY-3), Point(bX+3, bY+3), Scalar(255));
#endif

        goalX = (2*bX/(double)frame_gray.cols) -1;
        goalY = -((2*bY/(double)frame_gray.rows) -1);

#ifdef DEBUG
        printf("[Motion] Debug: goalX=%g, goalY=%g, N=%d\n", goalX, goalY, N);
#endif

#ifdef GUI
        imshow(window_name, frame_diff);
#endif
    }

    if (has_last) {
        last_frame.release();
    }

    has_last = 1;
    last_frame = frame_gray.clone();
}

