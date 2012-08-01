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
  Message *message;
    
  client.connectTo("localhost", 12345);
 
  client.sendMessage(commandsstore.getBuilder()->ServerGetVersion());
  message = client.getMessage();
  cout << "Value = " << message->read_uint() << endl;
  
  delete(message);

  client.sendMessage(commandsstore.getBuilder()->ServerEcho("Hello"));
  message = client.getMessage();
  cout << "Value = " << message->read_string() << endl;
     
  delete(message);

  client.stop();
}
