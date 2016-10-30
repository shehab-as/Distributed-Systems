#ifndef SERVER_H
#define SERVER_H

#include "../UDP/UDPServerSocket.h"
#include "../Message/Message.h"
#include <thread>
#include <vector>

class Server {
private:
    const int BUFFER_SIZE = 50;
    std::vector<std::thread> workers;
    std::vector<char[BUFFER_SIZE]> buffers;
    UDPServerSocket udpServerSocket;

    Message *getRequest();

    Message *doOperation();

    void sendReply(Message *_message);

public:
    Server(char *_listen_hostname, uint16_t _listen_port, int num_of_workers);

    void serveRequest(int index);

    ~server();
};

#endif // SERVER_H