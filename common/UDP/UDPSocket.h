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
    ssize_t readFromSocketWithBlock(char *message, size_t message_size, int maxBytes, sockaddr_in &peerAddr);
    ssize_t writeToSocket(char *message, int maxBytes, sockaddr_in peerAddr);
    bool initializeSocket(char *_myAddr, uint16_t _myPort);
    uint16_t getMyPort();
    int getSocketHandler();
    virtual ~UDPSocket();



/*
    int writeToSocketAndWait(char *buffer, int maxBytes, int microSec);

    int readFromSocketWithNoBlock(char *buffer, int maxBytes);

    int readFromSocketWithTimeout(char *buffer, int maxBytes, int timeoutSec,

                                  int timeoutMilli);
       void setFilterAddress(char *_filterAddress);

    char *getFilterAddress();
*/

};

#endif // UDPSOCKET_H