#ifndef SERVER_H
#define SERVER_H

#include "../UDP/UDPServerSocket.h"
#include "../Message/Message.h"

class Server {
private:
    UDPServerSocket *udpServerSocket;

    Message *getRequest();

    Message *doOperation();

    void sendReply(Message *_message);

public:
    Server(char *_listen_hostname, int _listen_port);

    void serveRequest();

    ~server();
};

#endif // SERVER_H