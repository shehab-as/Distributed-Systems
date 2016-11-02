#ifndef CLIENT_H
#define CLIENT_H

#include "../Message/Message.h"
#include "../UDP/UDPClientSocket.h"
#include "../Message/Message.h"

class Client {
private:
    UDPClientSocket *udpSocket;
public:
    Client();

    Message *execute(Message *_message);

    ~Client();
};

#endif // CLIENT_H
