#ifndef DISTRIBUTED_SYSTEMS_CM_H
#define DISTRIBUTED_SYSTEMS_CM_H

#include "../UDP/UDPSocket.h"
#include "../Message/Message.h"

// Communication Manager
// TODO: Packet reordering

class CM {
private:
    UDPSocket udpSocket;

    std::vector<std::string> replies;
    const int REPLIES_SIZE = 5000;
    const size_t RECV_BUFFER_SIZE = 8000;

    sockaddr_in create_sockaddr(char *addr, uint16_t port);

    int rebuild_request(char *recv_buffer, std::string &rebuilt_request, sockaddr_in &sender_addr);

    ssize_t send_fragments(Message message_to_fragment, sockaddr_in sender_addr);

    ssize_t send_message(Message message_to_send, sockaddr_in receiver_sock_addr);

    void create_and_send_ack(Header header, sockaddr_in sender_addr);

public:
    CM(char *_myAddr, uint16_t _myPort);

    ~CM();

    int send_with_ack(Message message_to_send, Message &received_message, int timeout_in_ms, int max_retries,
                      char *receiver_addr, uint16_t receiver_port);

    int send_no_ack(Message message_to_send, char *receiver_addr, uint16_t receiver_port);

    int send_no_ack(Message message_to_send, sockaddr_in receiver_sock_addr);

    int recv_with_block(Message &received_message, MessageType message_filter, sockaddr_in &sender_addr);

    ssize_t recv_with_timeout(Message &received_message, MessageType message_filter, sockaddr_in &sender_addr, int timeout_in_ms);
};

#endif //DISTRIBUTED_SYSTEMS_CM_H
