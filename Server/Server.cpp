#include <iostream>
#include "Server.h"

Server::Server(char *_listen_hostname, uint16_t _listen_port, int num_of_workers) {
    udpServerSocket.initializeServer(_listen_hostname, _listen_port);
    std::vector<std::thread> workers;

    for (int i = 0; i < num_of_workers; i++)
        workers.push_back(std::thread(&Server::serveRequest, this));

    for (int i = 0; i < num_of_workers; i++)
        workers[i].join();
}

Message *Server::getRequest() {
    //udpServerSocket.readFromSocketWithBlock();
}

void Server::serveRequest() {
    sockaddr_in peerAddr;
    char buffer[BUFFER_SIZE];

    while (true) {
        ssize_t bytes_read = udpServerSocket.readFromSocketWithBlock(buffer, BUFFER_SIZE, 8, &peerAddr);
        std::cout << buffer << std::endl;
        Message *replyFromServer;   // msg from server
        udpServerSocket.writeToSocket(buffer + 'zizobizo', 8, peerAddr);
    }
}

Server::~Server() {

}

