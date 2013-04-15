#include <iostream>
#include <vector>
#include <string>
#include <opencv.hpp>

#include "Vision.hpp"

using namespace std;

namespace Rhoban
{
    Vision::Vision(Connection* connection) : 
        connection(connection),
        imgNames(),
        isInitialized(false)
    {
        //Initialize image names
        imgNames.push_back("src");
        imgNames.push_back("bgr");
        imgNames.push_back("ball");
        imgNames.push_back("goal");
    }

    void Vision::grabBallInfo(bool clipping)
    {
        Message* response = connection
            ->VisionGetBallInfo_response((ui8)clipping, 10000);
        isBallDetected = response->read_bool();
        ballRelPosX = response->read_float();
        ballRelPosY = response->read_float();
        ballRelRadius = response->read_float();

        delete response;
    }
    
    void Vision::grabGoalInfo(string color)
    {
        Message* response = connection
            ->VisionGetGoalInfo_response(color, 10000);
        isGoalDetected = response->read_bool();
        goalRelPosX = response->read_float();
        goalRelPosY = response->read_float();
        goalRelWidth = response->read_float();
        goalRelHeight = response->read_float();

        delete response;
    }
            
    bool Vision::getIsBallDetected() const
    {
        return isBallDetected;
    }
    float Vision::getBallRelPosX() const
    {
        return ballRelPosX;
    }
    float Vision::getBallRelPosY() const
    {
        return ballRelPosY;
    }
    float Vision::getBallRelRadius() const
    {
        return ballRelRadius;
    }
    bool Vision::getIsGoalDetected() const
    {
        return isGoalDetected;
    }
    float Vision::getGoalRelPosX() const
    {
        return goalRelPosX;
    }
    float Vision::getGoalRelPosY() const
    {
        return goalRelPosY;
    }
    float Vision::getGoalRelWidth() const
    {
        return goalRelWidth;
    }
    float Vision::getGoalRelHeight() const
    {
        return goalRelHeight;
    }
            
    #ifdef WITH_OPENCV
        void Vision::grabAndDisplayFrames
            (unsigned int width, unsigned int height)
        {
            vector<ui32> widths;
            vector<ui32> heights;
            vector<ui8> colors;
            for (size_t i=0;i<imgNames.size();i++) {
                widths.push_back(width);
                heights.push_back(height);
                colors.push_back(0);
            }

            Message* response = connection
                ->VisionGetJpegFrames_response(
                imgNames, widths, heights, colors, 10000);
            display(response);
            
            delete response;
        }
                
        void Vision::ballCalibration()
        {
            vector<string> names;
            vector<ui32> widths;
            vector<ui32> heights;
            vector<ui8> colors;
            names.push_back("bgr");
            widths.push_back(300);
            heights.push_back(200);
            colors.push_back(0);
            
            cv::namedWindow("calibration");

            cout << "Move the ball in the of the screen, ";
            cout << "press space key " << endl;
            cout << "and stay still !" << endl;
            while (1) {
                Message* response = connection
                    ->VisionGetJpegFrames_response(
                    names, widths, heights, colors, 10000);
                names = response->read_string_array();
                vector< vector <byte> > data = response
                    ->read_array_array();
                cv::Mat tmpMat = cv::imdecode(cv::Mat(data[0]), 1);
                cv::circle(tmpMat, 
                    cv::Point(tmpMat.cols/2, tmpMat.rows/2), 
                    50, cv::Scalar(255, 255, 255), 4);
                cv::imshow("calibration", tmpMat);
                delete response;
                char input = cv::waitKey(1);
                if (input == ' ') {
                    break;
                }
            }

            cout << "Starting calibration..." << endl;
            vector<float> dots;
            dots.push_back(160);
            dots.push_back(120);
            Message* response = connection
                ->LocalisationPatternCalibrate_response(
                "ball", dots, 60000);
            cout << response->read_string() << endl;
            delete response;
            
            cv::destroyWindow("calibration");
        }

        void Vision::initWindows()
        {
            for (size_t i=0;i<imgNames.size();i++) {
                cout << "Reading image " << imgNames[i] << endl;
                cv::namedWindow("win_"+imgNames[i]);
            }
            isInitialized = true;
        }

        void Vision::display(Message* response)
        {
            imgNames = response->read_string_array();
            if (!isInitialized) {
                initWindows();
            }
            vector< vector <byte> > data = response->read_array_array();
            for (size_t i=0;i<imgNames.size();i++) {
                cv::Mat tmpMat = cv::imdecode(cv::Mat(data[i]), 1);
                cv::imshow("win_"+imgNames[i], tmpMat);
            }
            cv::waitKey(1);
        }

        void Vision::endWindows()
        {
            for (size_t i=0;i<imgNames.size();i++) {
                cv::destroyWindow("win_"+imgNames[i]);
            }
            isInitialized = false;
        }
    #endif
}

