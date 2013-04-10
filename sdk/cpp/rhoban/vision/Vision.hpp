#ifndef VISION_HPP_
#define VISION_HPP_

#include <map>
#include <string>
#include "communication/Connection.h"

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
             * Grab and display one camera frame
             */
            void runOne(unsigned int width, unsigned int height);

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
    };
}

#endif

