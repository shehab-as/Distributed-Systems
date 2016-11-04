#include <iostream>
#include <netdb.h>
#include <cstring>
#include "UDPServerSocket.h"

UDPServerSocket::UDPServerSocket() {

}

UDPServerSocket::~UDPServerSocket() {

}

bool UDPServerSocket::initializeServer(char *_myAddr, uint16_t _myPort) {
    sock = socket(AF_INET, SOCK_DGRAM, 0);

    if (sock < 0) {
        std::cout << "Error occured when creating socket\n";
        return false;
    }

    myPort = _myPort;
    myAddr.sin_family = AF_INET;
    myAddr.sin_port = htons((uint16_t) myPort);
    myAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock, (const sockaddr *) &myAddr, sizeof(struct sockaddr_in)) != 0) {
        std::cout << "Binding error\n";
        return false;
    }

    return true;
}