#include <cstdlib>
#include <iostream>
#include <main/Command.h>
#include <rhoban/types.h>
#include <rhoban/communication/CommandsStore.h>
#include <rhoban/communication/Connection.h>
#include <rhoban/communication/Message.h>

using namespace std;
using namespace Rhoban;

void test_sendMessage(Connection *client, CommandsStore *commandsstore)
{
  cout << "\n Testing sendMessage()" << endl;

  cout << "sendMessage(ServerGetVersion)" << endl;
  client->sendMessage(commandsstore->getBuilder()->ServerGetVersion()); // 1
  
  cout << "sendMessage(ServerEcho(Hello))" << endl;
  client->sendMessage(commandsstore->getBuilder()->ServerEcho("Hello")); // Hello 
}

void test_sendMessageRecieve(Connection *client, CommandsStore *commandsstore)
{
  cout << "\n Testing sendMessageRecieve()" << endl;

  Message *response;

  cout << "sendMessageRecieve(ServerGetVersion,1)" << endl;
  response = client->sendMessageRecieve(commandsstore->getBuilder()->ServerGetVersion(),1);
  if(response != NULL)
    cout << "Value = " << response->read_uint() << endl;
  else
    cout << "Value = NULL" << endl;

  delete(response);
    
  cout << "sendMessageRecieve(ServerGetVersion,2000)" << endl;
  response = client->sendMessageRecieve(commandsstore->getBuilder()->ServerGetVersion(),2000);
  if(response != NULL)
    cout << "Value = " << response->read_uint() << endl;
  else
    cout << "Value = NULL" << endl;
  
  delete(response);

  cout << "sendMessageRecieve(ServerGetVersion)" << endl;
  response = client->sendMessageRecieve(commandsstore->getBuilder()->ServerGetVersion());
  if(response != NULL)
    cout << "Value = " << response->read_uint() << endl;
  else
    cout << "Value = NULL" << endl;

  delete(response);
  
  cout << "sendMessageRecieve(ServerEcho(Hello),1)" << endl;
  response = client->sendMessageRecieve(commandsstore->getBuilder()->ServerEcho("Hello"),1);
  if(response != NULL)
    cout << "Value = " << response->read_string() << endl;
  else
    cout << "Value = NULL" << endl;

  delete(response);

  cout << "sendMessageRecieve(ServerEcho(Hello),2000)" << endl;
  response = client->sendMessageRecieve(commandsstore->getBuilder()->ServerEcho("Hello"),2000);
  if(response != NULL)
    cout << "Value = " << response->read_string() << endl;
  else
    cout << "Value = NULL" << endl;

  delete(response);

  cout << "sendMessageRecieve(ServerEcho(Hello))" << endl;
  response = client->sendMessageRecieve(commandsstore->getBuilder()->ServerEcho("Hello"));
  if(response != NULL)
    cout << "Value = " << response->read_string() << endl;
  else
    cout << "Value = NULL" << endl;

  delete(response);
}

void callbackResponseUint(Message *msg)
{
  cout << "Value = " << msg->read_uint() << endl;
}
 
void callbackResponseString(Message *msg)
{
  cout << "Value = " << msg->read_string() << endl;
}

void printCallingBack(Message *msg)
{
  cout << "Value = Calling back after receiving message from server" << endl;
}



void test_sendMessageCallback(Connection *client, CommandsStore *commandsstore)
{
  cout << "\n Testing sendMessageCallback()" << endl;
  
  cout << "sendMessageCallback(ServerGetVersion, callbackResponse)" << endl;
  client->sendMessageCallback(commandsstore->getBuilder()->ServerGetVersion(), callbackResponseUint);
  sleep(1);
  
  cout << "sendMessageCallback(ServerGetVersion, print(callingback))" << endl;
  client->sendMessageCallback(commandsstore->getBuilder()->ServerGetVersion(), printCallingBack);
  sleep(1);

  cout << "" << endl;
  
  cout << "sendMessageCallback(ServerEcho(Hello), callbackReponse)" << endl;
  client->sendMessageCallback(commandsstore->getBuilder()->ServerEcho("Hello"), callbackResponseString);
  sleep(1);

  cout << "sendMessageCallback(ServerEcho(Hello), print(callingback))" << endl;
  client->sendMessageCallback(commandsstore->getBuilder()->ServerEcho("Hello"), printCallingBack);
  sleep(1);
}

COMMAND_DEFINE(test_mailbox, "Test Mailbox")
{
  cout << "\n Begin Test Mailbox" << endl;
  
  CommandsStore commandsstore;
  Connection client;

  client.connectTo("localhost", 12345);
  cout << "\n Connected" << endl;  
  
  client.startMailbox();
  cout << "\n Running Mailbox" << endl;

  test_sendMessage(&client, &commandsstore);

  test_sendMessageCallback(&client, &commandsstore);

  test_sendMessageRecieve(&client, &commandsstore);
  
  client.stop();
}
