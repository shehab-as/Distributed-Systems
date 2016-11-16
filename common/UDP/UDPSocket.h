#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include <netinet/in.h>

class UDPSocket {
protected:
    int sock;
    sockaddr_in myAddr;
    uint16_t myPort;

public:
    UDPSocket();

    bool initializeSocket(char *_myAddr, uint16_t _myPort);

    ssize_t readFromSocketWithBlock(char *message, size_t message_size, sockaddr_in &peerAddr);

    ssize_t readFromSocketWithTimeout(char *message, size_t message_size, sockaddr_in &peerAddr, int timeoutInMS);

    ssize_t writeToSocket(char *message, sockaddr_in peerAddr);

    uint16_t getMyPort();

    int getSocketHandler();

    virtual ~UDPSocket();

//    void setFilterAddress(char *_filterAddress);
//
//    char *getFilterAddress();
};

#endif // UDPSOCKET_H