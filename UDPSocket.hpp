#ifndef UDPSOCKET_H 
#define UDPSOCKET_H

class UDPSocket {
protected:
int sock;
sockaddr_in myAddr; sockaddr_in peerAddr; char * myAddress; char * peerAddress; int myPort;
int peerPort;
bool enabled; pthread_mutex_t mutex;
public:
UDPSocket ();
void setFilterAddress (char * _filterAddress);
char * getFilterAddress ();
bool initializeServer (char * _myAddr, int _myPort);
bool initializeClient (char * _peerAddr, int _peerPort);
int writeToSocket (char * buffer, int maxBytes );
int writeToSocketAndWait (char * buffer, int maxBytes,int microSec );
int readFromSocketWithNoBlock (char * buffer, int maxBytes );
int readFromSocketWithTimeout (char * buffer, int maxBytes, int timeoutSec,
int timeoutMilli);
int readFromSocketWithBlock (char * buffer, int maxBytes );
int readSocketWithNoBlock (char * buffer, int maxBytes );
int readSocketWithTimeout (char * buffer, int maxBytes, int timeoutSec, int
timeoutMilli);
int readSocketWithBlock (char * buffer, int maxBytes ); int getMyPort ();
int getPeerPort ();
void enable();
void disable();
bool isEnabled();
void lock();
void unlock();
int getSocketHandler();
~UDPSocket ( );
};

#endif // UDPSOCKET_H