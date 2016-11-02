#include <iostream>
#include <netdb.h>
#include <unistd.h>
#include "UDPClientSocket.h"

UDPClientSocket::UDPClientSocket() {

}

UDPClientSocket::~UDPClientSocket() {

}

bool UDPClientSocket::initializeClient() {
    // initialize socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
        return false;

    // Make local socket addr
    myAddr.sin_family = AF_INET;
    myAddr.sin_port = htons(0);
    myAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    // bind socket
    if (bind(sock, (const sockaddr *) &myAddr, sizeof(struct sockaddr_in)) != 0) {
        std::cout << "Binding failed\n";
        close(sock);
        return false;
    }
    return true;
}
