#include "Client.h"
#include "../Message/Message.h"

//Constructor
Client::Client(char *_hostname, int _port)
{
    udpSocket->initializeClient(_hostname,_port);
}


//Execute a msg in client
Message* Client::execute(Message *_message)
{

    udpSocket->writeToSocket((char *) _message->getMessage(), _message->getMessageSize());
}


//Destructor
Client::~Client()
{
    udpSocket->disable();
}








