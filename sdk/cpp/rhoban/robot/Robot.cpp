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
#include <iostream>
#include <string>
#include <vector>
#include <communication/Connection.h>
#include <communication/CommandsStore.h>
#include <communication/Message.h>
#include <types.h>
#include <motors/Motors.h>
#include <config/Configurations.h>
#include "Robot.h"
#include "Moves.h"
#include "Sensors.h"
#include "file_mngt.h"

using namespace std;

namespace Rhoban
{
Robot::Robot(CommandsStore *commandsStore, string name)
{
	connection = new Connection(commandsStore);
	motors = new Motors(connection);
	configs = new Configurations(connection);
	moves = new Moves(connection);
	sensors = new Sensors(connection);
   vision = new Vision(connection);
	this->name = name;
}

Robot::~Robot()
{
	delete configs;
	delete connection;
	delete motors;
	delete moves;
	delete sensors;
}

void Robot::loadEnvironment(string environment)
{
	try
	{
		this->setEnvironment(environment);
		checkFixEnvironmentPath();

                string env1, env2;

		env1 = this->environment + "LowLevelConfig.xml";

                if (file_exists(env1)) {
                    this->loadLowLevelConfig(env1);
                } else {
		    env1 = this->environment + "ConfigFiles/LowLevelConfig.xml";
        	    this->loadLowLevelConfig(env1);
                }
		
                env2 = this->environment + "MoveSchedulerConfig.xml";
                if (file_exists(env2)) {
    		    this->loadMoveSchedulerConfig(env2);
                } else {
                    env2 = this->environment + "ConfigFiles/MoveSchedulerConfig.xml";
		    this->loadMoveSchedulerConfig(env2);
                }
	}
	catch(string exc)
	{
		throw string("Failed to load environment ") + environment + " for robot " + name + " :\n\t" + exc;
	}
}

void Robot::checkFixEnvironmentPath()
{
	string::iterator it;
	it = environment.end();
	it--;
	if(*it != '/')
		environment.push_back('/');
}

void Robot::connect(const char *adress, int port)
{
	this->hostname = adress;
	this->port = port;
	connection->connectTo(adress, port);
}

int Robot::isConnected()
{
	return connection->isConnected();
}

ui32 Robot::serverVersion()
{
	try
	{
		Message response = connection->ServerGetVersion_response(10000);
		uint answer = response.read_uint();
		return answer;
	}
	catch(string exc)
	{
		throw string("Failed to get server version:\n\t") = exc;
	}
}

int Robot::testConnection()
{
	cout << "Testing server version..." << endl;

	cout << "Version : " << serverVersion() << endl;

	cout << "Testing echo\"Hello world\"..." << endl;

	Message response = connection->ServerEcho_response("Hello ","world", 10000);
	string s1 = response.read_string();
	string s2 = response.read_string();

	cout << " Echo : " << s1 << s2 << endl;

	if((s1 == "Hello ") && (s2 == "world"))
		cout << "Connection test successful." << endl;
	else
		cout << "Connection test failed." << endl;

}

void Robot::loadLowLevelConfig(string filename, bool force)
{
	configs->loadLowLevelConfig(filename, force);
}

void Robot::loadMoveSchedulerConfig(string filename, bool force)
{
	configs->loadMoveSchedulerConfig(filename, force);
	motors->setConfig(configs->getMoveSchedulerConfig());
}

void Robot::allCompliant()
{
	motors->allCompliant();
}

string Robot::moveFileName(string name)
{
	string retval = getEnvironment();
	retval.append("Moves/");
	retval.append(name);
	retval.append(".xml");
	return retval;
}

void Robot::loadMove(string name, bool rawPath)
{
	string filename = name;
        if (!rawPath) {
	    filename.append(".graphics");
	    filename = moveFileName(filename);
        }

	ifstream ifile(filename.c_str());
	bool fileExists = (bool)ifile;
	ifile.close();

	if(fileExists)
		moves->loadMove(filename);
	else
	{
		string filename2 = moveFileName(name);

		ifstream ifile2(filename2.c_str());
		bool fileExists2 = (bool)ifile2;
		ifile2.close();

		if(fileExists2)
			moves->loadMove(filename2);
		else
			cout << "Move file for " << name << " not found." << endl;
	}
}

void Robot::startMove(string name, ui32 duration, ui32 smooth)
{
	moves->startMove(name, duration, smooth);
}

void Robot::pauseMove(string name)
{
	moves->pauseMove(name);
}

void Robot::stopMove(string name, ui32 smooth)
{
	moves->stopMove(name, smooth);
}

void Robot::killMove(string name)
{
	moves->killMove(name);
}

vector<string> Robot::getLoadedMoves()
{
	return moves->getLoadedMoves();
}

void Robot::updateConstant(string moveName, string constantName, float value)
{
	moves->updateConstant(moveName, constantName, value);
}
    
void Robot::updateBlockXML(string moveName, string xml)
{
    moves->updateBlockXML(moveName, xml);
}

void Robot::emergency()
{
	connection->SchedulerEmergencyStop();
}

void Robot::stop()
{
	connection->stop();
	motors->stop();
}

void Robot::setMotors(Motors *motors)
{
	this->motors = motors;
}

Motors* Robot::getMotors()
{
	return motors;
}

void Robot::setConfigs(Configurations *configs)
{
	this->configs = configs;
}

Configurations *Robot::getConfigs()
{
	return configs;
}

void Robot::setConnection(Connection *connection)
{
	this->connection = connection;
}

Connection *Robot::getConnection()
{
	return connection;
}

void Robot::setEnvironment(string path)
{
	environment = path;
	checkFixEnvironmentPath();
}

string Robot::getEnvironment()
{
	checkFixEnvironmentPath();
	return environment;
}
void Robot::setMoves(Moves *moves)
{
	this->moves = moves;
}

Moves *Robot::getMoves()
{
	return moves;
}

void Robot::setSensors(Sensors *sensors)
{
	this->sensors = sensors;
}

Sensors *Robot::getSensors()
{
	return sensors;
}

void Robot::setVision(Vision *vision)
{
	this->vision = vision;
}

Vision *Robot::getVision()
{
	return vision;
}

void Robot::setName(string name)
{
	this->name = name;
}

string Robot::getName()
{
	return name;
}

void Robot::setHostname(string hostname)
{
	this->hostname = hostname;
}

string Robot::getHostname()
{
	return hostname;
}

void Robot::setPort(int port)
{
	this->port = port;
}

int Robot::getPort()
{
	return port;
}
}
