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
#include <string>
#include <file_mngt.h>
#include <communication/Connection.h>
#include <config/LowLevelConfig.h>
#include <config/MoveSchedulerConfig.h>
#include "Configurations.h"

using namespace std;

namespace Rhoban
{

Configurations::Configurations(Connection *connection)
{
	this->connection = connection;
	lowLevelConfig = NULL;
	moveSchedulerConfig = NULL;
}

Configurations::~Configurations()
{
	delete lowLevelConfig;
	delete moveSchedulerConfig;
}

bool Configurations::isMoveSchedulerConfigLoaded()
{
	try
	{
		Message response = connection->SchedulerConfigIsLoaded_response(10000);
		bool answer = response.read_uint();
		return answer;
	}
	catch(string exc)
	{
		throw string("Failed to check whether MoveSchedulerConfigISLoaded:\n\t"+ exc);
	}
}

void Configurations::loadMoveSchedulerConfig(string config, bool force)
{
	this->moveSchedulerConfig = new MoveSchedulerConfig(config);

	if(force || isMoveSchedulerConfigLoaded() == 0)
	{
		string configContents = file_to_string(config);
		connection->SchedulerLoadConfig(configContents);

		connection->ServosScan(250, "Normal");
	}
}

bool Configurations::isLowLevelConfigLoaded()
{
	try
	{
		int max_tries = 3;
		while(max_tries -- >= 0)
		{
			try
			{
				Message response = connection->LowLevelConfigIsLoaded_response(1000);
				bool answer = response.read_uint();
				return answer;
			}catch(string exc)
			{
				if(max_tries <= 0)
					throw string("Failed three times to ask whether low level config is loaded:\n\t")+ exc;
			}
		}
	}
	catch(string exc)
	{
		throw string("Failed to check whether LowLevelConfigisLoaded:\n\t" ) + exc;
	}

}

void Configurations::loadLowLevelConfig(string config, bool force)
{
	try
	{
		lowLevelConfig = new LowLevelConfig(config);

		if(force || isLowLevelConfigLoaded() == 0)
		{
			string configContents = file_to_string(config);
			connection->LowLevelLoadConfig(configContents);
		}
	}
	catch(string exc)
	{
		throw string("Failed to load lowlevelconfig:\n\t") + exc;

	}
}

Connection *Configurations::getConnection()
{
	return connection;
}

void Configurations::setConnection(Connection *connection)
{
	this->connection = connection;
}

LowLevelConfig *Configurations::getLowLevelConfig()
{
	return lowLevelConfig;
}

void Configurations::setLowLevelConfig(LowLevelConfig *lowLevelConfig)
{
	this->lowLevelConfig = lowLevelConfig;
}

MoveSchedulerConfig *Configurations::getMoveSchedulerConfig()
{
	return moveSchedulerConfig;
}

void Configurations::setMoveSchedulerConfig(MoveSchedulerConfig *moveSchedulerConfig)
{
	this->moveSchedulerConfig = moveSchedulerConfig;
}
}
