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

bool UDPSocket::initializeSocket(char *_myAddr=NULL, uint16_t _myPort = 0) {
    sock = socket(AF_INET, SOCK_DGRAM, 0);

    if (sock < 0) {
        std::cerr << "Error occured when creating socket\n";
        return false;
    }

    myPort = _myPort;
    myAddr.sin_family = AF_INET;

    // if myPort == 0, a random empty port will be picked by the OS
    myAddr.sin_port = htons((uint16_t) myPort);
    myAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock, (const sockaddr *) &myAddr, sizeof(struct sockaddr_in)) != 0) {
        std::cerr << "Binding error\n";
        return false;
    }

    return true;
}

ssize_t UDPSocket::writeToSocket(char *message, sockaddr_in peerAddr) {
    // n will contain number of bytes sent if successful
    // n == -1 on failure

    // strlen(parameters) + 1 to take into account the NULL character at the end
    // will send/receive garbage otherwise

    ssize_t bytes_sent = sendto(sock, message, strlen(message) + 1, 0, (const sockaddr *) &peerAddr, sizeof(peerAddr));

    if (bytes_sent < 0) {
        std::cerr << "Error occured while sending\n";
        std::cerr << strerror(errno) << std::endl;
    }
    return bytes_sent;
}

ssize_t UDPSocket::readFromSocketWithBlock(char *message, size_t message_size, sockaddr_in &peerAddr) {
    // Very Important
    peerAddr.sin_family = AF_INET;

    socklen_t slen = sizeof(struct sockaddr_in);
    ssize_t bytes_read = recvfrom(sock, message, message_size, 0, (sockaddr *) &peerAddr, &slen);

    if (bytes_read < 0) {
        std::cerr << "Error occured when receiving\n";
        std::cerr << strerror(errno) << std::endl;
    }

    return bytes_read;
}


ssize_t UDPSocket::readFromSocketWithTimeout(char *message, size_t message_size,sockaddr_in &peerAddr,
                                             int timeoutInMS) {
    // Very Important
    peerAddr.sin_family = AF_INET;

    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = timeoutInMS * 100;     // measured in micro second

    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        std::cerr << "Error when setting receive timeout.\n";
    }

    socklen_t slen = sizeof(struct sockaddr_in);
    ssize_t bytes_read = recvfrom(sock, message, message_size, 0, (sockaddr *) &peerAddr, &slen);

    // Setting socket back to blocking/no timeout
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        std::cerr << "Error when setting socket back to blocking.\n";
    }

    return bytes_read;
}

uint16_t UDPSocket::getMyPort() {
    return myPort;
}

int UDPSocket::getSocketHandler() {
    return sock;
}



