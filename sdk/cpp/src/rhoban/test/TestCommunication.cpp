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
  cout << "Begin Test Communication" << endl;
  
  CommandsStore commandsstore;
  Connection client;
  Message message;
  
  client.connectTo("localhost", 12345);
 
  client.sendMessage(commandsstore.builder->ServerGetVersion());
  cout << "Value = " << client.getMessage(&message)->read_uint() << endl;

  client.sendMessage(commandsstore.builder->ServerEcho("Hello"));
  cout << "Value = " << client.getMessage(&message)->read_string() << endl;
     
  client.stop();
}
