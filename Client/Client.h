#ifndef CLIENT_H
#define CLIENT_H

#include "../Message/Message.h"
#include "../UDP/UDPClientSocket.h"
#include "../Message/Message.h"

class Client {
private:
    UDPClientSocket udpSocket;
public:
    Client();

    Message execute(Message *_message, char *_peerAddr, uint16_t _peerPort);

    sockaddr_in get_sockaddr(char *address, uint16_t port);

    ~Client();
};

#endif // CLIENT_H
