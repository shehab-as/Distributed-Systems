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

public:
    Client();

    Message execute(Message *_message, char *_peerAddr, uint16_t _peerPort);

    sockaddr_in get_sockaddr(char *address, uint16_t port);

    ~Client();
};

#endif // CLIENT_H
