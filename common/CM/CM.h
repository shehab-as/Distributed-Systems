#ifndef DISTRIBUTED_SYSTEMS_CM_H
#define DISTRIBUTED_SYSTEMS_CM_H


#include "../UDP/UDPSocket.h"

class CM {
private:
    UDPSocket udpSocket;
    sockaddr_in create_sockaddr(char *addr, uint16_t port);

public:
    CM(char *_myAddr, uint16_t _myPort);
    ~CM() {}

    int send_with_ack(char *message,size_t max_message_size, int timeout_in_ms, int max_retries, char *receiver_addr, uint16_t receiver_port);
    int send_no_ack(char *message, char *receiver_addr, uint16_t receiver_port);
    int recv_with_block(char *message, size_t message_size, sockaddr_in &receiver_addr);
    // packet reordering
};


#endif //DISTRIBUTED_SYSTEMS_CM_H
