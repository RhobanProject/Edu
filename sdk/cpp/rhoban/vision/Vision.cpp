#include <iostream>
#include <vector>
#include <string>
    
#ifdef WITH_OPENCV
    #include <opencv2/opencv.hpp>
#endif

#include "Vision.hpp"

using namespace std;

namespace Rhoban
{
    Vision::Vision(Connection* connection) : 
        connection(connection),
        imgNames(),
        isInitialized(false)
    {
    }

    void Vision::grabBallInfo(bool clipping)
    {
        Message response = connection
            ->VisionGetBallInfo_response((ui8)clipping, 10000);
        isBallDetected = response.read_bool();
        ballRelPosX = response.read_float();
        ballRelPosY = response.read_float();
        ballRelRadius = response.read_float();
    }
    
    void Vision::grabGoalInfo(string color)
    {
        Message response = connection
            ->VisionGetGoalInfo_response(10000);
        isGoalDetected = response.read_bool();
        goalRelPosX = response.read_float();
        goalRelPosY = response.read_float();
        goalRelWidth = response.read_float();
        goalRelHeight = response.read_float();

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
            (vector<string>& names, unsigned int width, unsigned int height)
        {
            imgNames = names;
            vector<ui32> widths;
            vector<ui32> heights;
            vector<bool> colors;
            for (size_t i=0;i<imgNames.size();i++) {
                widths.push_back(width);
                heights.push_back(height);
                colors.push_back(0);
            }

            Message response = connection
                ->VisionGetJpegFrames_response(
                imgNames, widths, heights, colors, 10000);
            display(&response);
            
        }
        map<std::string, cv::Mat> grabFrames(vector<std::string> names,
        		unsigned int width, unsigned int height, Connection* connection, std::string format)
        {
        	if(format != "jpg" && format !="png")
        		throw string("Only jpg and png extension supported");

        	vector<ui32> widths;
        	vector<ui32> heights;
        	vector<bool> colors;
        	for(int i = 0; i< names.size(); i++)
        	{
        		widths.push_back(width);
        		heights.push_back(height);
        		colors.push_back(false);
        	}

        	Message response = 0;
        	if(format == "jpg")
        		response =	connection
        			->VisionGetJpegFrames_response(
        					names, widths, heights, colors, 2000);

        	if(format == "png")
        		response =	connection
        			->VisionGetPngFrames_response(
        					names, widths, heights, colors, 2000);

        	vector< std::string > names_back = response.read_string_array();
        	vector< vector <byte> > data = response.read_array_array();
        	if(names_back.size() != data.size())
        		throw string("Wrong data length in ReadImages answer");

        	map<std::string, cv::Mat> answer;
        	for(int i = 0 ; i < names_back.size(); i++)
        		answer[names_back[i]] = cv::imdecode(cv::Mat(data[i]), 1);

        	return answer;
        }

        map<std::string, cv::Mat> Vision::grabJpegFrames(vector<std::string> names,
        		unsigned int width, unsigned int height)
        {
        	return grabFrames(names, width, height, connection, "jpg");
        }

        map<std::string, cv::Mat> Vision::grabPngFrames(vector<std::string> names,
        		unsigned int width, unsigned int height)
        {
        	return grabFrames(names, width, height, connection, "png");
        }

        cv::Mat Vision::grabPngFrame(std::string name,
        		unsigned int width, unsigned int height)
        {
        	vector<std::string> names;
        	names.push_back(name);
        	map<string, cv::Mat> result = grabPngFrames(names,width,height);
        	if(result.find(name) == result.end())
        		throw string("Failed to grab png frame '" + name + "'");
        	return result[name];
        }

        cv::Mat Vision::grabJpegFrame(std::string name,
        		unsigned int width, unsigned int height)
        {
        	vector<std::string> names;
        	names.push_back(name);
        	map<string, cv::Mat> result = grabJpegFrames(names,width,height);
        	if(result.find(name) == result.end())
        		throw string("Failed to grab jpeg frame '" + name + "'");
        	return result[name];
        }

        void Vision::ballCalibration()
        {
            /*
            vector<string> names;
            vector<ui32> widths;
            vector<ui32> heights;
            vector<bool> colors;
            names.push_back("ballcalibration");
            widths.push_back(300);
            heights.push_back(200);
            colors.push_back(0);
            
            cv::namedWindow("calibration");

            cout << "Move the ball in the of the screen, ";
            cout << "press space key " << endl;
            cout << "and stay still !" << endl;
            while (1) {
                Message response = connection
                    ->VisionGetJpegFrames_response(
                    names, widths, heights, colors, 10000);
                names = response.read_string_array();
                vector< vector <byte> > data = response
                    .read_array_array();
                cv::Mat tmpMat = cv::imdecode(cv::Mat(data[0]), 1);
                cv::imshow("calibration", tmpMat);
                char input = cv::waitKey(1);
                if (input == ' ') {
                    break;
                }
            }

            cout << "Starting calibration..." << endl;
            vector<float> dots;
            dots.push_back(0.0);
            dots.push_back(0.0);
            Message response = connection
                ->LocalisationPatternCalibrate_response(
                "ball", dots, 60000);
            cout << response.read_string() << endl;
            
            cv::destroyWindow("calibration");
            */
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

