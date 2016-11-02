#ifndef SERVER_H
#define SERVER_H

#include <thread>
#include <vector>

#include "../UDP/UDPServerSocket.h"
#include "../Message/Message.h"

class Server {
private:
    const size_t BUFFER_SIZE = 50;
    UDPServerSocket udpServerSocket;

    Message *getRequest();

    Message *doOperation();

    void sendReply(Message *_message, char *_peerAddr, int _peerPort);

public:
    Server(char *_listen_hostname, uint16_t _listen_port, int num_of_workers);

    void serveRequest();

    ~Server();
};

#endif // SERVER_H