#include <netdb.h>
#include <iostream>
#include <cstring>
#include "Client.h"
#include "../Message/Message.h"

//Constructor
Client::Client(char *_peerAddr, uint16_t _peerPort) {
    UdpClientSocket.initializeSocket(_peerAddr,_peerPort);

    struct hostent *host;
    peerAddr.sin_family = AF_INET;

    if ((host = gethostbyname(_peerAddr)) == NULL) {
        std::cout << "Error occured when fetching hostname\n";
    }

    peerAddr.sin_addr = *(struct in_addr *) (host->h_addr);

    // Sometimes a segmentation fault might occur here when multiple threads
    // call htons(_peerPort) at the same time
    // This is why we initialize peerAddr only once at the client's initialization
    // and not on every message we send
    peerAddr.sin_port = htons(_peerPort);
}

//Execute a msg in client
Message Client::execute(Message *_message) {
    char *message = (char *) _message->getMessage();

    ssize_t bytes_sent = UdpClientSocket.writeToSocket(message, 8, peerAddr);

    // Wait for reply
    UdpClientSocket.readFromSocketWithBlock(buffer, BUFFER_SIZE, 8, peerAddr);

    Message reply(MessageType::Reply, buffer, strlen(buffer), 0);

    return reply;
}


//Destructor
Client::~Client() {

}






