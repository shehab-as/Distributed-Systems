#include <netdb.h>
#include "UDPSocket.h"
#include <iostream>
#include <unistd.h>
#include <cstring>

UDPSocket::UDPSocket() {

}

UDPSocket::~UDPSocket() {
    close(sock);
}

bool UDPSocket::initializeClient(char *_peerAddr, int _peerPort) {
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
    struct hostent *host;
    peerAddr.sin_family = AF_INET;
    if ((host = gethostbyname(_peerAddr)) == NULL) {
        std::cout << "Error occured when fetching hostname\n";
        return false;
    }
    peerAddr.sin_addr = *(struct in_addr *) (host->h_addr);
    peerAddr.sin_port = htons((uint16_t) _peerPort);
    return true;
}

ssize_t UDPSocket::writeToSocket(char *message, int maxBytes) {
    // n will contain number of bytes sent if successful
    // n == -1 on failure
    ssize_t n = sendto(sock, message, strlen(message), 0, (const sockaddr *) &peerAddr, sizeof(struct sockaddr_in));
    if (n < 0)
        std::cout << "Error occured while sending\n";
    return n;
}

void UDPSocket::disable() {
    enabled = false;
}

// Server
bool UDPSocket::initializeServer(char *_myAddr, uint16_t _myPort) {
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        std::cout << "Error occured when creating socket\n";
        return false;
    }
    myAddr.sin_family = AF_INET;
    myAddr.sin_port = htons((uint16_t) myPort);
    myAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock, (const sockaddr *) &myAddr, sizeof(struct sockaddr_in)) != 0) {
        std::cout << "Binding error\n";
        return false;
    }

    peerAddr.sin_family = AF_INET;
    return true;
}

ssize_t UDPSocket::readFromSocketWithBlock(char *message, int maxBytes) {
    size_t message_size = sizeof(message);
    ssize_t n = recvfrom(sock, message, message_size, 0, (sockaddr *) &peerAddr, (socklen_t *) sizeof(peerAddr));
    if (n < 0)
        std::cout << "Error occured when receiving\n";

    std::cout << "Stopped receiving\n";
    return n;
}



