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
    bool enabled;
    pthread_mutex_t mutex;
public:
    UDPSocket();

    void setFilterAddress(char *_filterAddress);

    char *getFilterAddress();

    bool initializeServer(char *_myAddr, uint16_t _myPort);

    bool initializeClient(char *_peerAddr, int _peerPort);

    ssize_t writeToSocket(char *message, int maxBytes);

    int writeToSocketAndWait(char *buffer, int maxBytes, int microSec);

    int readFromSocketWithNoBlock(char *buffer, int maxBytes);

    int readFromSocketWithTimeout(char *buffer, int maxBytes, int timeoutSec,
                                  int timeoutMilli);

    ssize_t readFromSocketWithBlock(char *message, int maxBytes);

    int getMyPort();

    int getPeerPort();

    void enable();

    void disable();

    bool isEnabled();

    void lock();

    void unlock();

    int getSocketHandler();

    ~UDPSocket();
};

#endif // UDPSOCKET_H