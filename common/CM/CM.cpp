#include <iostream>
#include <netdb.h>
#include <sstream>
#include "CM.h"

// TODO: Communication Manager should handle marshalling/unmarshalling of Message

CM::CM(char *_myAddr, uint16_t _myPort) {
    udpSocket.initializeSocket(_myAddr, _myPort);
}

CM::~CM() {

}

int CM::send_with_ack(Message message_to_send, Message &received_message, int timeout_in_ms, int max_retries,
                      char *receiver_addr, uint16_t receiver_port) {
    // Send and wait for an acknowledgment.
    // A reply to the sent request is implicitly considered an acknowledgment
    ssize_t bytes_read = -1;
    char reply_buffer[RECV_BUFFER_SIZE];

    // Marshall the message to be sent
    std::string marshalled_msg = message_to_send.marshal();

    // Create a sockaddr from the receiver's addr and port
    sockaddr_in receiver_sock_addr = create_sockaddr(receiver_addr, receiver_port);
    sockaddr_in reply_addr;

    // Send message and wait for a reply
    // If no reply is sent within timeout_in_ms, bytes_read will be -1
    // If a reply is received, bytes_read will contain a value > 0 indicating the number of bytes read
    // We repeat this send/wait for reply sequence until a reply is received or max_retries reaches 0
    while (max_retries-- && bytes_read == -1) {
        udpSocket.writeToSocket((char *) marshalled_msg.c_str(), receiver_sock_addr);
        bytes_read = udpSocket.readFromSocketWithTimeout(reply_buffer, RECV_BUFFER_SIZE, reply_addr, timeout_in_ms);
    }

    if (bytes_read >= 0)
        received_message = Message(reply_buffer);

    return (int) bytes_read;
}

int CM::send_no_ack(Message message_to_send, char *receiver_addr, uint16_t receiver_port) {
    // Send and don't wait for a reply

    // Create receiver_sock_addr from receiver's addr and port
    sockaddr_in receiver_sock_addr = create_sockaddr(receiver_addr, receiver_port);

    std::string marshalled_msg = message_to_send.marshal();

    ssize_t bytes_sent = udpSocket.writeToSocket((char *) marshalled_msg.c_str(), receiver_sock_addr);
    return (int) bytes_sent;
}

int CM::send_no_ack(Message message_to_send, sockaddr_in receiver_sock_addr) {
    std::string marshalled_msg = message_to_send.marshal();
    ssize_t bytes_sent = udpSocket.writeToSocket((char *) marshalled_msg.c_str(), receiver_sock_addr);
    return (int) bytes_sent;
}

int CM::recv_with_block(Message &received_message, sockaddr_in &sender_addr) {
    // A blocking receive that fills received_message with the received contents
    bool fragmented = true;
    char recv_buffer[RECV_BUFFER_SIZE];
    std::string recv_request;
    ssize_t bytes_read = -1;

    while(fragmented) {
        fragmented = false;
        bytes_read = udpSocket.readFromSocketWithBlock(recv_buffer, RECV_BUFFER_SIZE, sender_addr);
        int frag_check = check_if_fragmented(recv_buffer);

        if(frag_check == 1)
            bytes_read = rebuild_request(recv_buffer, recv_request, sender_addr);
        else
            recv_request = recv_buffer;
    }

    if (bytes_read >= 0)
        received_message = Message((char *) recv_request.c_str());

    return (int) bytes_read;
}

int CM::rebuild_request(char *initial_fragment, std::string &rebuilt_request, sockaddr_in &sender_addr)
{
    rebuilt_request = initial_fragment;
    bool still_fragmented = true;
    ssize_t bytes_read = -1;
    char recv_buffer[RECV_BUFFER_SIZE];

    while(still_fragmented) {
        udpSocket.writeToSocket(NULL, sender_addr);
        bytes_read = udpSocket.readFromSocketWithBlock(recv_buffer, RECV_BUFFER_SIZE, sender_addr);

        int frag = check_if_fragmented(recv_buffer);

        // An error occured, we should have received -1 not 0
        if(frag == 0)
            return -1;

        // Last packet in the fragmented packets
        if(frag == -1)
            still_fragmented = false;
        rebuilt_request += recv_buffer;
    }

    return 0;
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

int CM::check_if_fragmented(char *recv_buffer) {
    std::stringstream tokenizer(recv_buffer);
    std::string token;

    tokenizer >> token;
    auto message_type = std::stoi(token);

    tokenizer >> token;
    auto op = std::stoull(token);

    tokenizer >> token;
    auto rpc_id = std::stoull(token);

    tokenizer >> token;
    auto seq_id = std::stoull(token);

    tokenizer >> token;
    auto fragmented = std::stoi(token);

    return fragmented;

}
