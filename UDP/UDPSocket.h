#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include <netinet/in.h>

class UDPSocket {
protected:
    int sock;
    sockaddr_in myAddr;
    char *myAddress;
    uint16_t myPort;
public:
    UDPSocket();

    void setFilterAddress(char *_filterAddress);

    char *getFilterAddress();


    ssize_t writeToSocket(char *message, int maxBytes, char *_peerAddr, uint16_t _peerPort);

    int writeToSocketAndWait(char *buffer, int maxBytes, int microSec);

    int readFromSocketWithNoBlock(char *buffer, int maxBytes);

    int readFromSocketWithTimeout(char *buffer, int maxBytes, int timeoutSec,
                                  int timeoutMilli);

    ssize_t readFromSocketWithBlock(char *message, size_t message_size, int maxBytes, sockaddr_in *peerAddr);

    uint16_t getMyPort();


    int getSocketHandler();

    virtual ~UDPSocket();
};

#endif // UDPSOCKET_H