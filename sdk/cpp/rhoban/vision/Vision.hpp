#ifndef VISION_HPP_
#define VISION_HPP_

#include <map>
#include <string>
#include "communication/Connection.h"

#ifdef WITH_OPENCV
#include <opencv2/opencv.hpp>
#endif

namespace Rhoban
{
    class Vision
    {
        public:

            /**
             * Initialize Vision client
             */
            Vision(Connection* connection);

            /**
             * Ask to RhobanServer ball information and store them internaly
             */
            void grabBallInfo(bool clipping = false);

            /**
             * Ask to RhobanServer goal information and store them internaly
             */
            void grabGoalInfo(std::string color = "yellow");

            /**
             * Ball and Goal getters
             */
            bool getIsBallDetected() const;
            float getBallRelPosX() const;
            float getBallRelPosY() const;
            float getBallRelRadius() const;
            bool getIsGoalDetected() const;
            float getGoalRelPosX() const;
            float getGoalRelPosY() const;
            float getGoalRelWidth() const;
            float getGoalRelHeight() const;

            /**
             * Logging options
             */
            void startLog() const;
            void startLog(uint timeMS) const;
            void endLog() const;
            
            #ifdef WITH_OPENCV
                /**
                 * Grab and display one camera frame
                 */
                void grabAndDisplayFrames(vector<std::string>& names, 
                    unsigned int width, unsigned int height);

                cv::Mat grabJpegFrame(std::string name,
                                    unsigned int width, unsigned int height);

                cv::Mat grabPngFrame(std::string name,
                    unsigned int width, unsigned int height);

                map<std::string, cv::Mat> grabPngFrames(vector<std::string> names,
                    unsigned int width, unsigned int height);

                map<std::string, cv::Mat> grabJpegFrames(vector<std::string> names,
                    unsigned int width, unsigned int height);

                /**
                 * Do the ball calibration
                 * The ball have to be in the middle of the screen
                 */
                void ballCalibration();
            #endif

        protected:

            /**
             * Connection to RhobanServer
             */
            Connection* connection;

            /**
             * Images names to grab
             */
            std::vector<std::string> imgNames;

            /**
             * Is windows initialized
             */
            bool isInitialized;

            #ifdef WITH_OPENCV
                /**
                 * Initialize image windows
                 */
                void initWindows();

                /**
                 * Display on windows the given message
                 */
                void display(Message* response);

                /**
                 * Destroy images windows
                 */
                void endWindows();
            #endif

            /**
             * Ball information
             */
            bool isBallDetected;
            float ballRelPosX;
            float ballRelPosY;
            float ballRelRadius;

            /**
             * Goal information
             */
            bool isGoalDetected;
            float goalRelPosX;
            float goalRelPosY;
            float goalRelWidth;
            float goalRelHeight;
    };
}

#endif

