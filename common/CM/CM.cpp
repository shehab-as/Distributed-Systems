#include <iostream>
#include <netdb.h>
#include "CM.h"

CM::CM(char *_myAddr, uint16_t _myPort) {
    udpSocket.initializeSocket(_myAddr, _myPort);
}

// Send and wait for reply
int CM::send_with_ack(char *message, char* reply_buffer, size_t reply_buffer_size, int timeout_in_ms, int max_retries, char *receiver_addr,
                      uint16_t receiver_port) {
    ssize_t n = -1;
    char *local_message = (char *) htonl((uint32_t) message);

    sockaddr_in receiver_sock_addr = create_sockaddr(receiver_addr, receiver_port);
    sockaddr_in reply_addr;

    while (max_retries-- && n != 0) {
        udpSocket.writeToSocket(local_message, 8, receiver_sock_addr);
        n = udpSocket.readFromSocketWithTimeout(reply_buffer, reply_buffer_size, 0, reply_addr, timeout_in_ms);
    }
    if (n == 0)
        reply_buffer = (char *) ntohl((uint32_t) reply_buffer);
    return (int) n;
}

// Send and don't wait for a reply
int CM::send_no_ack(char *message, char *receiver_addr, uint16_t receiver_port) {
    sockaddr_in receiver_sock_addr = create_sockaddr(receiver_addr, receiver_port);

    char *local_message = (char *) htonl((uint32_t) message);
    ssize_t n = udpSocket.writeToSocket(local_message, 8, receiver_sock_addr);
    return (int) n;
}

int CM::send_no_ack(char *message, sockaddr_in receiver_sock_addr) {
    char *local_message = (char *) htonl((uint32_t) message);
    ssize_t n = udpSocket.writeToSocket(local_message, 8, receiver_sock_addr);
    return (int) n;
}

sockaddr_in CM::create_sockaddr(char *addr, uint16_t port) {
    struct hostent *host;
    sockaddr_in sock_addr;
    sock_addr.sin_family = AF_INET;

    if ((host = gethostbyname(addr)) == NULL) {
        std::cerr << "Error occured when fetching hostname\n";
    }

    sock_addr.sin_addr = *(struct in_addr *) (host->h_addr);

    // Sometimes a segmentation fault might occur here when multiple threads
    // call htons(_peerPort) at the same time
    // This is why we initialize peerAddr only once at the client's initialization
    // and not on every parameters we send
    sock_addr.sin_port = htons(port);
    return sock_addr;
}

int CM::recv_with_block(char *message, size_t message_size, sockaddr_in &receiver_addr) {
    ssize_t n = udpSocket.readFromSocketWithBlock(message, message_size, 8, receiver_addr);
    if (n == 0)
        message = (char *) ntohl((uint32_t) message);
    return (int) n;
}
