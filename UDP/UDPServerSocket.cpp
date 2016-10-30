#include <iostream>
#include <netdb.h>
#include <cstring>
#include "UDPServerSocket.h"

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

    // Very important
    peerAddr.sin_family = AF_INET;

    return true;
}

ssize_t UDPServerSocket::writeToSocket(char *message, int maxBytes, char *_peerAddr, int _peerPort) {
    struct hostent *host;
    peerAddr.sin_family = AF_INET;
    if ((host = gethostbyname(_peerAddr)) == NULL) {
        std::cout << "Error occured when fetching hostname\n";
        return false;
    }
    peerAddr.sin_addr = *(struct in_addr *) (host->h_addr);
    peerAddr.sin_port = htons((uint16_t) _peerPort);
    ssize_t n = sendto(sock, message, strlen(message), 0, (const sockaddr *) &peerAddr, sizeof(struct sockaddr_in));
    if (n < 0)
        std::cout << "Error occured while sending\n";
    return n;
}

UDPServerSocket::UDPServerSocket() {

}

UDPServerSocket::~UDPServerSocket() {

}
