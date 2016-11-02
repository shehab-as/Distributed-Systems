#include "Server.h"

Server::Server(char *_listen_hostname, uint16_t _listen_port) {
    udpServerSocket.initializeServer(_listen_hostname, _listen_port);
}

Server::~server() {

}

Message *Server::getRequest() {
    //udpServerSocket.readFromSocketWithBlock();
}

Server::Server(char *_listen_hostname, uint16_t _listen_port, int num_of_workers) {
    udpServerSocket.initializeServer(_listen_hostname, _listen_port);

    buffers.resize((unsigned long) num_of_workers);   //buffers of threads
    for (int i = 0; i < num_of_workers; i++)
        workers.push_back(std::thread(serveRequest(i)));
}

void Server::serveRequest(int index) {
    while (true) {
        udpServerSocket.readFromSocketWithBlock(buffers[index], BUFFER_SIZE, 8);
//        doOperation();
        Message* replyFromServer;   // msg from server
        udpServerSocket.writeToSocket((char *)replyFromServer, replyFromServer->getMessageSize()
                ,udpServerSocket.getPeerAddress(),udpServerSocket.getPeerPort());

    }
}


