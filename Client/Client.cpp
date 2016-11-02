#include "Client.h"
#include "../Message/Message.h"

//Constructor
Client::Client()
{
    udpSocket->initializeClient();
}


//Execute a msg in client
Message* Client::execute(Message *_message)
{

}


//Destructor
Client::~Client()
{

}








