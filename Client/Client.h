#ifndef CLIENT_H
#define CLIENT_H

#include "../Message/Message.h"
#include "../UDP/UDPClientSocket.h"
#include "../Message/Message.h"

class Client {
private:
    UDPClientSocket udpSocket;
    static const size_t BUFFER_SIZE = 50;
    char buffer[BUFFER_SIZE];
    sockaddr_in peerAddr;

public:
    Client(char * _peerAddr, uint16_t _peerPort);

    Message execute(Message *_message);

    ~Client();
};

#endif // CLIENT_H
