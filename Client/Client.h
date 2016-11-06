#ifndef CLIENT_H
#define CLIENT_H

#include "../Message/Message.h"
#include "../Message/Message.h"
#include "../UDP/UDPSocket.h"

class Client {
private:
    UDPSocket UdpClientSocket;
    static const size_t BUFFER_SIZE = 50;
    char buffer[BUFFER_SIZE];
    sockaddr_in peerAddr;

public:
    Client(char * _peerAddr, uint16_t _peerPort);

    Message execute(Message *_message);

    ~Client();
};

#endif // CLIENT_H
