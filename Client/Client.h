#ifndef CLIENT_H
#define CLIENT_H

#include "../common/Message/Message.h"
#include "../common/Message/Message.h"
#include "../common/UDP/UDPSocket.h"

class Client {
private:
    UDPSocket udpSocket;
    static const size_t BUFFER_SIZE = 50;
    char buffer[BUFFER_SIZE];
    sockaddr_in peerAddr;

public:
    Client(char *_myAddr, uint16_t _myPort, char *_peerAddr, uint16_t _peerPort);

    Message execute(Message *_message);

    ~Client();
};

#endif // CLIENT_H
