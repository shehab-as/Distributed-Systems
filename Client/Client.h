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

    ~Client();
};

#endif // CLIENT_H
