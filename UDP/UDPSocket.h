#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include <netinet/in.h>

class UDPSocket {
protected:
    int sock;
    sockaddr_in myAddr;
    sockaddr_in peerAddr;
    char *myAddress;
    char *peerAddress;
    uint16_t myPort;
    int peerPort;
    //pthread_mutex_t mutex;
public:
    UDPSocket();

    void setFilterAddress(char *_filterAddress);

    char *getFilterAddress();


    ssize_t writeToSocket(char *message, int maxBytes);

    int writeToSocketAndWait(char *buffer, int maxBytes, int microSec);

    int readFromSocketWithNoBlock(char *buffer, int maxBytes);

    int readFromSocketWithTimeout(char *buffer, int maxBytes, int timeoutSec,
                                  int timeoutMilli);

    ssize_t readFromSocketWithBlock(char *message, size_t message_size, int maxBytes);

    uint16_t getMyPort();

    int getPeerPort();

//    void lock();
//
//    void unlock();

    int getSocketHandler();

    char* getPeerAddress();

    virtual ~UDPSocket();
};

#endif // UDPSOCKET_H