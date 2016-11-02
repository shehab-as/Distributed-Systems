#ifndef UDPCLIENTSOCKET_H
#define UDPCLIENTSOCKET_H

#include "UDPSocket.h"

class UDPClientSocket : public UDPSocket {
public:
    UDPClientSocket();

    bool initializeClient();

    ~UDPClientSocket();
};

#endif // UDPCLIENTSOCKET_H