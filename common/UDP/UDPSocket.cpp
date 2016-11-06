#include <netdb.h>
#include "UDPSocket.h"
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <errno.h>

UDPSocket::UDPSocket() {

}

UDPSocket::~UDPSocket() {
    close(sock);
}

bool UDPSocket::initializeSocket(char *_myAddr, uint16_t _myPort = 0) {
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

ssize_t UDPSocket::writeToSocket(char *message, int maxBytes, sockaddr_in peerAddr) {

    // n will contain number of bytes sent if successful
    // n == -1 on failure

    // strlen(message) + 1 to take into account the NULL character at the end
    // will send/receive garbage otherwise

    ssize_t n = sendto(sock, message, strlen(message) + 1, 0, (const sockaddr *) &peerAddr, sizeof(peerAddr));

    if (n < 0) {
        std::cout << strerror(errno) << std::endl;
        std::cout << "Error occured while sending\n";
    }
    return n;
}

ssize_t UDPSocket::readFromSocketWithBlock(char *message, size_t message_size, int maxBytes, sockaddr_in &peerAddr) {
    // Very Important
    peerAddr.sin_family = AF_INET;

    socklen_t slen = sizeof(struct sockaddr_in);
    ssize_t n = recvfrom(sock, message, message_size, 0, (sockaddr *) &peerAddr, &slen);

    if (n < 0) {
        std::cout << strerror(errno) << std::endl;
        std::cout << "Error occured when receiving\n";
    }

    return n;
}


ssize_t UDPSocket::readFromSocketWithTimeout(char *message, size_t message_size, int maxBytes, sockaddr_in &peerAddr,
                                             int timeoutInMS) {
    // Very Important
    peerAddr.sin_family = AF_INET;

    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = timeoutInMS * 100;     // measured in micro second

    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        perror("Error when setting receive timeout.");
    }

    socklen_t slen = sizeof(struct sockaddr_in);
    ssize_t n = recvfrom(sock, message, message_size, 0, (sockaddr *) &peerAddr, &slen);

    if (n < 0) {
        std::cout << "Error occured when receiving\n";
        std::cout << strerror(errno) << std::endl;
    }

    return n;
}

/*
bool UDPSocket::initializeClient() {
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
*/

uint16_t UDPSocket::getMyPort() {
    return myPort;
}

int UDPSocket::getSocketHandler() {
    return sock;
}



