/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <yaml-cpp/yaml.h>
#include <main/Command.h>
#include <util.h>
#include "Robots.h"
#include "Robot.h"

using namespace std;

namespace Rhoban
{
    Robots::Robots(){}

    size_t Robots::len()
    {
        return robots.size();
    }

    Robot *Robots::operator[](string name)
    {
        return get(name);
    }

    Robot *Robots::get(string name)
    {
        if (robots.find(name) != robots.end()) {
            return robots[name];
        } else {
            ostringstream oss;
            oss << "Unable to find robot with name " << name;

            throw oss.str();
        }
    }

    void Robots::loadYaml(string filename)
    {
        cout << endl << "Loading yaml configuration " << filename << endl;

        try
        {
            ifstream cfgfile(filename.c_str());
            if(!cfgfile)
                throw string("Failed to open file " + filename);

            YAML::Parser parser(cfgfile);
            YAML::Node doc;

            bool result = parser.GetNextDocument(doc);
            if(!result)
                throw string("Parser failed to load document");

            if(!doc.FindValue("robots"))
            {
                throw string("Config error : no robots entry in yaml doc");
            }
            else
            {
                YAML::Iterator it;
                int i;

                string name, host, environment, move;
                int port;
                vector<string> loadedMoves;

                for(it=doc["robots"].begin();it!=doc["robots"].end();++it)
                {
                    loadedMoves.clear();

                    // Name
                    it.first() >> name;
                    robots[name] = new Robot(new CommandsStore, name);

                    // Host & port
                    if(it.second().FindValue("host"))
                    {
                        if(it.second().FindValue("host"))
                            it.second()["host"] >> host;
                        else
                            host = "localhost";

                        if(it.second().FindValue("port")) {
                            it.second()["port"] >> port;
                        } else {
                            port = 7777;
                        }

                        robots[name]->connect(host.c_str(), port);
                    }

                    // Environment
                    if(it.second().FindValue("environment"))
                    {
                        it.second()["environment"] >> environment;
                        robots[name]->loadEnvironment(environment);
                    }

                    // LoadMoves
                    if(it.second().FindValue("loadMoves"))
                    {
                        loadedMoves = robots[name]->getLoadedMoves();

                        for(i=0; i<it.second()["loadMoves"].size(); ++i)
                        {
                            it.second()["loadMoves"][i] >> move;

                            if(!is_in_vector_string(loadedMoves, move))
                            {
                                loadedMoves.push_back(move);
                                robots[name]->loadMove(move);
                            }
                        }
                    }
                }
                cout << endl;
            }
        }
        catch(const std::exception& ex)
        {
            throw string("Failed to load yaml configuration:\n\t")+ string(ex.what());
        }
    }






    void Robots::stop()
    {
        map<string, Robot *>::iterator it;
        for(it = robots.begin(); it != robots.end(); ++it)
            it->second->stop();
    }

    void Robots::setRobots(map<string, Robot *> robots)
    {
        this->robots = robots;
    }

    map<string, Robot *> Robots::getRobots()
    {
        return robots;
    }
}
