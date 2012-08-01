/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
#include <iostream>
#include <cstdio>

#include <main/Command.h>

#include <threading/Thread.h>
#include <threading/Mutex.h>
#include <threading/Condition.h>

class Waiter : public Thread
{
    void execute()
    {
        Mutex mutex;
        Condition *condition = (Condition*)_Arg;

        cout << "Waiter: Entering condition" << endl;
        while (true)  {
            if (condition->wait(&mutex, 2000)) {
                cout << "Waiter: Condition" << endl;
            } else {
                cout << "Waiter: Time out" << endl;
            }
        }
    }
};

COMMAND_DEFINE(threading, "Tests the threading")
{
    Waiter waiter;
    Condition condition;

    cout << "Main: Running waiter" << endl;

    waiter.start(&condition);

    cout << "Main: Waiter runned" << endl;

    sleep(5);
    cout << "Main: Triggering condition" << endl;
    condition.broadcast();
    sleep(5);

    cout << "Main: Killing waiter" << endl;
    waiter.kill();

    cout << "Main: Exiting" << endl;
}
