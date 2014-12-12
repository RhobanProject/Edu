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
#include <map>
#include <string>
#include <threading/Thread.h>
#include <config/MoveSchedulerConfig.h>
#include <communication/Connection.h>
#include <robot/Robot.h>
#include <types.h>
#include "Motor.h"
#include "Motors.h"
#include "ticks.h"

using namespace std;

namespace Rhoban
{
    void motorsValues(Message *values, void *data)
    {
        Motors *self = (Motors *)data;
        self->processValues(values);
    }

    Motors::Motors(Connection *connection) : Thread()
    {
        this->connection = connection;
        this->configuration = NULL;
        running = 0;
    }

    Motors::~Motors()
    {
    }

    void Motors::setConfig(MoveSchedulerConfig *config)
    {
        this->configuration = config;

        map<string, ServoConfig>::iterator it;
        for(it = configuration->servos_config.servos.begin();
                it != configuration->servos_config.servos.end(); ++it)
        {
            ServoConfig &config = (*it).second;
            string name = config.Name;
            motors[name] = new Motor(config.Id, config.Name, config.IniAngle, config.ZeroAngle);
            idMotors[config.Id] = motors[name];
        }
    }

    size_t Motors::len()
    {
        return motors.size();
    }

    Motor *Motors::operator[](string name)
    {
        return get(name);
    }

    Motor *Motors::get(string name)
    {
        if (motors.find(name) != motors.end()) {
            return motors[name];
        } else {
            ostringstream oss;
            oss << "Unable to find motor with name " << name;

            throw oss.str();
        }
    }

    void Motors::start(int frequency)
    {
        this->frequency = frequency;

        if(configuration == NULL)
            cout << "No suitable motors configuration" << endl;
        else
        {
            running = 1;
            Thread::start(NULL);
        }
    }

    void Motors::stop()
    {
        running = 0;
    }

    void Motors::compliant(string name)
    {
        motors[name]->setLoad(0);
    }

    void Motors::allCompliant()
    {
        connection->ServosAllCompliant();

        map<string, Motor *>::iterator it;
        for(it = motors.begin(); it != motors.end(); ++it)
            it->second->setLoad(0);
    }

    void Motors::hard(string name)
    {
        motors[name]->setLoad(1);
    }

    void Motors::allHard()
    {
        map<string, Motor *>::iterator it;
        for(it = motors.begin(); it != motors.end(); ++it)
            it->second->setLoad(1);
    }

    void Motors::pullValues()
    {
        try
        {
            Message values = connection->ServosGetValues_response(1,10000);
            processValues(&values);
        }
        catch(string exc)
        {
            throw string("Failed to pull values:\n\t") + exc;
        }
    }

    void Motors::pushValues()
    {
        map<string, Motor *>::iterator it;

        vector<float> angles;
        vector<string> names;

        for(it = motors.begin(); it != motors.end(); ++it) {
            if(it->second->isDirty()
                    && it->second->getGoalAngleInit()
                    && it->second->getCurrentAngleInit()
                    && it->second->getCurrentSpeedInit()) {
                names.push_back(it->first);
                angles.push_back((int)  (it->second->getGoalAngle()) );
            }
        }

        if(names.size()) {
            connection->SetRelativeServosAngles(names, angles);
        }
    }

    void Motors::processValues(Message *values)
    {
        vector<byte> ids = values->read_array();
        vector<float> angles= values->read_float_array();
        vector<float> speeds= values->read_float_array();
        vector<float> loads= values->read_float_array();
        vector<float> goals= values->read_float_array();

        for(int i=0; i < ids.size(); ++i)
        {
            if(idMotors.count(ids[i]))
            {
                idMotors[ids[i]]->setLastUpdate(time(NULL));

                idMotors[ids[i]]->setCurrentAngle((double)angles[i]);
                if(idMotors[ids[i]]->getGoalAngleInit() == 0)
                    idMotors[ids[i]]->setGoalAngle(idMotors[ids[i]]->getAngle());

                idMotors[ids[i]]->setCurrentGoal((double)goals[i]);

                idMotors[ids[i]]->setCurrentSpeed((double)speeds[i]/1023);
                if(idMotors[ids[i]]->getGoalSpeedInit() == 0)
                {
                    if(idMotors[ids[i]]->getSpeed() > 0)
                        idMotors[ids[i]]->setGoalSpeed(idMotors[ids[i]]->getSpeed());
                    else
                        idMotors[ids[i]]->setGoalSpeed(0);
                }

                idMotors[ids[i]]->setCurrentLoad((double)loads[i]/1023);
                if(idMotors[ids[i]]->getGoalLoadInit() == 0)
                {
                    if(idMotors[ids[i]]->getLoad() > 0)
                        idMotors[ids[i]]->setGoalLoad(idMotors[ids[i]]->getLoad());
                    else
                        idMotors[ids[i]]->setGoalLoad(0);
                }
            }
        }
    }

    void Motors::goToZero(int duration, bool verbose)
    {
        connection->SchedulerTakeRefPosition("Zero", 1000);
    }

    void Motors::goToInit(int duration, bool verbose)
    {
        try
        {
            connection->SchedulerTakeRefPosition("Ini", 1000);
        }
        catch(string exc)
        {
            throw string("Failed to go to initial posiiton:\n\t") + exc;
        }
    }

    void Motors::raiseLoad(int duration, bool verbose)
    {
        map<string, Motor *>::iterator it;
        int cs = duration * 100;

        for(int i=0; i < cs; i++)
        {
            if(verbose)
                printf("\rLoad: %3d%%", ((i+1)*100)/cs);

            for(it = motors.begin(); it != motors.end(); ++it)
                it->second->setLoad(((double)i+1)/(double)cs);

            syst_wait_ms(10);
            pushValues();
        }
        if(verbose)
            cout << "" << endl;
    }

    void Motors::execute()
    {
        while(running)
        {
            connection->ServosGetValues_callback(1, motorsValues, this);
            pushValues();
            syst_wait_ms(1000/frequency);
        }
    }

    void Motors::scan()
    {
        map<string, Motor *>::iterator it;
        for(it = motors.begin(); it != motors.end(); ++it)
            it->second->setLastUpdate(0);
        connection->ServosScan(250, "Normal");
    }

    void Motors::setConnection(Connection *connection)
    {
        this->connection = connection;
    }

    Connection * Motors::getConnection()
    {
        return connection;
    }

    void Motors::setConfiguration(MoveSchedulerConfig *config)
    {
        this->configuration = config;
    }

    MoveSchedulerConfig *Motors::getConfiguration()
    {
        return configuration;
    }

    void Motors::setMotors(map<string, Motor *> motors)
    {
        this->motors = motors;
    }

    map<string, Motor *> Motors::getMotors()
    {
        return motors;
    }

    void Motors::setIdMotors(map<int, Motor *> idMotors)
    {
        this->idMotors = idMotors;
    }

    map<int, Motor *> Motors::getIdMotors()
    {
        return idMotors;
    }

    void Motors::setRunning(bool val)
    {
        this->running = val;
    }

    bool Motors::getRunning()
    {
        return running;
    }

    void Motors::setFrequency(int frequency)
    {
        this->frequency = frequency;
    }

    int Motors::getFrequency()
    {
        return frequency;
    }
}
