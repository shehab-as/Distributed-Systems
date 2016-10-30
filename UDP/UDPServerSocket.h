#ifndef UDPSERVERSOCKET_H
#define UDPSERVERSOCKET_H

#include "UDPSocket.h"

class UDPServerSocket : public UDPSocket {
public:
    UDPServerSocket();

    bool initializeServer(char *_myAddr, uint16_t _myPort);

    ~UDPServerSocket();

    ssize_t writeToSocket(char *message, int maxBytes, char *_peerAddr, int _peerPort);
};

#endif // UDPSERVERSOCKET_H