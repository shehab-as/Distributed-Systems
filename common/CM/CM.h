#ifndef DISTRIBUTED_SYSTEMS_CM_H
#define DISTRIBUTED_SYSTEMS_CM_H

#include "../UDP/UDPSocket.h"

// Communication Manager
// TODO: Packet reordering

class CM {
private:
    UDPSocket udpSocket;

    sockaddr_in create_sockaddr(char *addr, uint16_t port);

public:
    CM(char *_myAddr, uint16_t _myPort);

    int send_with_ack(char *message, char *reply_buffer, size_t reply_buffer_size, int timeout_in_ms, int max_retries,
                      char *receiver_addr, uint16_t receiver_port);

    int send_no_ack(char *message, char *receiver_addr, uint16_t receiver_port);

    int send_no_ack(char *message, sockaddr_in receiver_sock_addr);

    int recv_with_block(char *message, size_t message_size, sockaddr_in &receiver_addr);

    ~CM();
};

#endif //DISTRIBUTED_SYSTEMS_CM_H
