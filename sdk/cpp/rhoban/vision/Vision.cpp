#ifdef WITH_OPENCV

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
        imgNames.push_back("ball");
        imgNames.push_back("goal");
    }

    void Vision::runOne(unsigned int width, unsigned int height)
    {
        vector<ui32> widths;
        vector<ui32> heights;
        vector<ui32> colors;
        for (size_t i=0;i<imgNames.size();i++) {
            widths.push_back(width);
            heights.push_back(height);
            colors.push_back(true);
        }

        Message* response = connection
            ->VisionGetJpegFrames_response(
            imgNames, widths, heights, colors, 1000);
        display(response);
        
        delete response;
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
}

#endif

