#ifndef DISTRIBUTED_SYSTEMS_CM_H
#define DISTRIBUTED_SYSTEMS_CM_H

#include "../UDP/UDPSocket.h"
#include "../Message/Message.h"

// Communication Manager
// TODO: Packet reordering

class CM {
private:
    UDPSocket udpSocket;
    const size_t RECV_BUFFER_SIZE = 7500;

    sockaddr_in create_sockaddr(char *addr, uint16_t port);

    int rebuild_request(char *recv_buffer, std::string &rebuilt_request, sockaddr_in &sender_addr);

    ssize_t send_fragments(Message message_to_fragment, sockaddr_in sender_addr);

public:
    CM(char *_myAddr, uint16_t _myPort);

    int send_with_ack(Message message_to_send, Message &received_message, int timeout_in_ms, int max_retries,
                      char *receiver_addr, uint16_t receiver_port);

    int send_no_ack(Message message_to_send, char *receiver_addr, uint16_t receiver_port);

    int send_no_ack(Message message_to_send, sockaddr_in receiver_sock_addr);

    int recv_with_block(Message &received_message, sockaddr_in &sender_addr);

    ~CM();
};

#endif //DISTRIBUTED_SYSTEMS_CM_H
