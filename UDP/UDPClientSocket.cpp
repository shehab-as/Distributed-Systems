#include <iostream>
#include <netdb.h>
#include <unistd.h>
#include "UDPClientSocket.h"

bool UDPClientSocket::initializeClient(char *_peerAddr, int _peerPort) {
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

UDPClientSocket::UDPClientSocket() {

}

UDPClientSocket::~UDPClientSocket() {

}
