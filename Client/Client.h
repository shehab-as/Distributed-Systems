#ifndef CLIENT_H
#define CLIENT_H

#include "Message.h"
#include "../UDP/UDPClientSocket.h"
#include "../Message/Message.h"

class Client {
private:
    UDPClientSocket *udpSocket;
public:
    Client(char *_hostname, int _port);

    Message *execute(Message *_message);

    ~Client();
};

#endif // CLIENT_H
