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
#include <communication/Connection.h>
#include <file_mngt.h>
#include "Moves.h"

using namespace std;

namespace Rhoban
{
Moves::Moves(Connection *connection)
{
	this->connection = connection;
}

void Moves::loadMove(string path)
{
	connection->SchedulerLoadXMLMove_response(file_to_string(path));
}

void Moves::startMove(string name, ui32 duration, ui32 smooth)
{
	connection->SchedulerStartMove(name, duration, smooth);
}

void Moves::pauseMove(string name)
{
	connection->SchedulerPauseMove(name);
}

void Moves::stopMove(string name, ui32 smooth)
{
	connection->SchedulerStopMove(name, smooth);
}

void Moves::killMove(string name)
{
	connection->SchedulerKillMove(name);
}

vector<string> Moves::getLoadedMoves()
{
	Message response = connection->SchedulerGetLoadedMoves_response();
	vector<string> answer = response.read_string_array();
	return answer;
}

void Moves::updateConstant(string moveName, string constantName, float value)
{
	vector<float> values;
	values.push_back(value);
	connection->SchedulerUpdateConstant(moveName, constantName, values);
}

LinearSpline Moves::getSpline(string movename, string splineName)
{
	LinearSpline spline;
	try
	{
		Message answer = connection->SchedulerGetCompressedRecordedSpline_response(movename, splineName,10000);

		string movename = answer.read_string();
		string splinename = answer.read_string();
		string stream = answer.read_string();

		//we serialize and deserialize to check everything is fine
		spline.Serializable::from_xml(stream);

		vector < vector < float > > values = answer.read_float_array_array();

		spline.importRawData(values);

		return spline;
	}
	catch(string & exc)
	{
		throw string("Failed to get spline:\n\t" + exc);
	}
}


void Moves::setSpline(const LinearSpline & spline, string movename)
{
	try
	{
		vector < vector < float > > values;
		spline.exportToRawData(values);
		connection->SchedulerSetCompressedSpline(movename, spline.name, values);
	}
	catch(string & exc)
	{
		throw string("Failed to set spline:\n\t" + exc);
	}

}

void Moves::startRecordingSpline(string movename, string splinename)
{
	connection->SchedulerStartMove(movename, 0, 0);
	connection->SchedulerRecordSpline(movename, splinename);
}
void Moves::stopRecordingSpline(string movename, string splinename)
{
	connection->SchedulerStopSpline(movename, splinename);
}

void Moves::playSpline(string movename, string splinename)
{
	connection->SchedulerPlaySpline(movename, splinename);
}
void Moves::stopSpline(string movename, string splinename)
{
	connection->SchedulerStopSpline(movename, splinename);
}

void Moves::pauseResumeSpline(string movename, string splinename)
{
	connection->SchedulerPauseResumeSpline(movename, splinename);
}

void Moves::testSplinePosition(float x, string movename, string splinename)
{
	connection->SchedulerTestSpline(movename, splinename,x, 0);
}

void Moves::setConnection(Connection *connection)
{
	this->connection = connection;
}

Connection *Moves::getConnection()
{
	return connection;
}
}
