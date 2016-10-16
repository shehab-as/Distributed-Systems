#ifndef UDPSERVERSOCKET_H
#define UDPSERVERSOCKET_H

class UDPServerSocket : public UDPSocket {
public:
    UDPServerSocket();

    bool initializeServer(char *_myAddr, int _myPort);

    ~UDPServerSocket();
};

#endif // UDPSERVERSOCKET_H
