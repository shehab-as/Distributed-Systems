#include <netdb.h>
#include <iostream>
#include <cstring>
#include "Client.h"
#include "../Message/Message.h"

//Constructor
Client::Client() {
    udpSocket.initializeClient();
}


//Execute a msg in client
Message Client::execute(Message *_message, char *_peerAddr, uint16_t _peerPort) {
    char *message = (char *) _message->getMessage();
    sockaddr_in peerAddr = get_sockaddr(_peerAddr, _peerPort);

    ssize_t bytes_sent = udpSocket.writeToSocket(message, 8, peerAddr);

    // Wait for reply
    udpSocket.readFromSocketWithBlock(buffer, BUFFER_SIZE, 8, peerAddr);

    Message reply(MessageType::Reply, buffer, strlen(buffer), 0);

    return reply;
}


//Destructor
Client::~Client() {

}

sockaddr_in Client::get_sockaddr(char *address, uint16_t port) {
    sockaddr_in peerAddr;
    struct hostent *host;
    peerAddr.sin_family = AF_INET;

    if ((host = gethostbyname(address)) == NULL) {
        std::cout << "Error occured when fetching hostname\n";
        return sockaddr_in();
    }

    peerAddr.sin_addr = *(struct in_addr *) (host->h_addr);
    peerAddr.sin_port = htons(port);
    return peerAddr;
}








