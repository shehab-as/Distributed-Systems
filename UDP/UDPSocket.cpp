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

ssize_t UDPSocket::writeToSocket(char *message, int maxBytes, sockaddr_in peerAddr) {

    // n will contain number of bytes sent if successful
    // n == -1 on failure
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

uint16_t UDPSocket::getMyPort() {
    return myPort;
}

int UDPSocket::getSocketHandler() {
    return sock;
}



