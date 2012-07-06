#include <cstdlib>
#include <iostream>
#include <main/Command.h>
#include <rhoban/types.h>
#include <rhoban/communication/CommandsStore.h>
#include <rhoban/communication/Connection.h>
#include <rhoban/communication/Message.h>

using namespace std;
using namespace Rhoban;


COMMAND_DEFINE(test_communication, "Test Communication")
{
    Message response;

    cout << "Begin Test Communication" << endl;

    CommandsStore *commandsstore = new CommandsStore;
    Connection *client = new Connection;

    client->connectTo("192.168.16.100", 12345);

    client->sendMessage(commandsstore->builder->ServerGetVersion());
    cout << "Value = " << client->getMessage(&response)->read_uint() << endl;

    client->sendMessage(commandsstore->builder->ServerEcho("Hello"));
    cout << "Value = " << client->getMessage(&response)->read_string() << endl;

    client->stop();
}
