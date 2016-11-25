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

//////////////////////////////////////////////////
//             Send/Receive methods             //
/////////////////////////////////////////////////
int CM::send_with_ack(Message message_to_send, Message &received_message, int timeout_in_ms, int max_retries,
                      char *receiver_addr, uint16_t receiver_port) {
    // Send and wait for an acknowledgment.
    // A reply to the sent request is implicitly considered an acknowledgment
    ssize_t bytes_read = -1;

    // Create a sockaddr from the receiver's addr and port
    sockaddr_in receiver_sock_addr = create_sockaddr(receiver_addr, receiver_port);
    sockaddr_in sender_addr;

    // Send message and wait for a reply
    // If no reply is sent within timeout_in_ms, bytes_read will be -1
    // If a reply is received, bytes_read will contain a value > 0 indicating the number of bytes read
    // We repeat this send/wait for reply sequence until a reply is received or max_retries reaches 0
    while (max_retries-- && bytes_read == -1) {
        send_no_ack(message_to_send, receiver_sock_addr);
        bytes_read = recv_with_timeout(received_message, sender_addr, timeout_in_ms);
    }

    return (int) bytes_read;
}

int CM::send_no_ack(Message message_to_send, char *receiver_addr, uint16_t receiver_port) {
    // Create receiver_sock_addr from receiver's addr and port
    sockaddr_in receiver_sock_addr = create_sockaddr(receiver_addr, receiver_port);

    return (int) send_message(message_to_send, receiver_sock_addr);
}

int CM::send_no_ack(Message message_to_send, sockaddr_in receiver_sock_addr) {
    return (int) send_message(message_to_send, receiver_sock_addr);
}

ssize_t CM::recv_with_timeout(Message &received_message, sockaddr_in &sender_addr, int timeout_in_ms) {
    ssize_t bytes_read;
    char recv_buffer[RECV_BUFFER_SIZE];
    std::string recv_request;

    bytes_read = udpSocket.readFromSocketWithTimeout(recv_buffer, RECV_BUFFER_SIZE, sender_addr, timeout_in_ms);

    if (bytes_read == -1)
        return -1;

    // Extract the packet's header
    Header header(recv_buffer);

    // If the received message is an ack or part of a fragmented message, this would indicate that a connection occured
    // in rebuild_request and these are merely the leftovers from the previous unsuccesful request, in which case they
    // should be dropped/not handled
    while (header.message_type == MessageType::Ack || (header.fragmented == 1 && header.sequence_id != 0)) {
        bytes_read = udpSocket.readFromSocketWithTimeout(recv_buffer, RECV_BUFFER_SIZE, sender_addr, timeout_in_ms);

        if (bytes_read == -1)
            return -1;

        header = Header(recv_buffer);
    }

    // If sender did not get the ack for the previous message's last fragment, resend the ack
    // This is done to not let the sender report a connection error due to the last fragment's
    // ack not being received (rebuild_request only send out one ack with no retries for the last fragment)
    if (header.fragmented == -1) {
        Header ack_header = header;
        ack_header.message_type = MessageType::Ack;
        ack_header.fragmented = 0;
        ack_header.sequence_id = 0;
        Payload ack_payload = Payload(0, 1, std::vector<std::string>{"ack"}, 0);
        Message ack(ack_header, ack_payload);
        send_no_ack(ack, sender_addr);
    } else if (header.fragmented == 1)
        // Call rebuild_request if header indicated that the message is fragmented
        bytes_read = rebuild_request(recv_buffer, recv_request, sender_addr);
    else
        recv_request = recv_buffer;

    // If message was successfully received, unmarshall and build received_message
    if (bytes_read >= 0)
        received_message = Message((char *) recv_request.c_str());

    return bytes_read;
}

int CM::recv_with_block(Message &received_message, sockaddr_in &sender_addr) {
    // A blocking receive that fills received_message with the received contents
    char recv_buffer[RECV_BUFFER_SIZE];
    std::string recv_request;
    ssize_t bytes_read;

    // Wait for a packet to be received
    bytes_read = udpSocket.readFromSocketWithBlock(recv_buffer, RECV_BUFFER_SIZE, sender_addr);

    // Extract the packet's header
    Header header(recv_buffer);

    while (header.message_type == MessageType::Ack && bytes_read != -1
           || (header.fragmented == 1 && header.sequence_id != 0)) {
        bytes_read = udpSocket.readFromSocketWithBlock(recv_buffer, RECV_BUFFER_SIZE, sender_addr);
        header = Header(recv_buffer);
    }

    if (header.fragmented == -1) {
        Header ack_header = header;
        ack_header.message_type = MessageType::Ack;
        ack_header.fragmented = 0;
        ack_header.sequence_id = 0;
        Payload ack_payload = Payload(0, 1, std::vector<std::string>{"ack"}, 0);
        Message ack(ack_header, ack_payload);
        send_no_ack(ack, sender_addr);
    }
        // Call rebuild_request if header indicated that the message is fragmented
    else if (header.fragmented == 1)
        bytes_read = rebuild_request(recv_buffer, recv_request, sender_addr);
    else
        recv_request = recv_buffer;

    // If message was successfully received, unmarshall and build received_message
    if (bytes_read >= 0)
        received_message = Message((char *) recv_request.c_str());

    return (int) bytes_read;
}

//////////////////////////////////////////////////
//             Helper methods                   //
/////////////////////////////////////////////////
ssize_t CM::send_message(Message message_to_send, sockaddr_in receiver_sock_addr) {
    std::string marshalled_msg = message_to_send.marshal();
    ssize_t bytes_sent;

    // If message size if bigger than the receiver's buffer size, fragment the message
    if (marshalled_msg.size() > RECV_BUFFER_SIZE - 1)
        bytes_sent = send_fragments(message_to_send, receiver_sock_addr);
    else
        bytes_sent = udpSocket.writeToSocket((char *) marshalled_msg.c_str(), receiver_sock_addr);

    return bytes_sent;
}

ssize_t CM::send_fragments(Message message_to_fragment, sockaddr_in receiver_sock_addr) {
    // Maximum time (in ms) to wait for an ack
    const int ACK_TIMEOUT = 150;

    // Max attempts at sending a fragment
    const int MAX_RETRIES = 5;
    int max_retries;

    ssize_t bytes_read;
    ssize_t bytes_sent = 0;
    ssize_t total_bytes_sent = 0;

    // ACK buffer to hold acks from the receiver
    char ack_buffer[RECV_BUFFER_SIZE];

    // sockaddr_in which will store the address of the receiver/replier given they are the same
    // TODO: Make send_fragments return reply address to its caller
    sockaddr_in reply_addr;

    // Mark the message as being fragmented, needed for receiver to prepare itself to rebuild the request
    // in rebuild_request
    message_to_fragment.setFrag(1);

    Header header = message_to_fragment.getHeader();
    std::string payload_str = message_to_fragment.getPayload().str();

    // Set the maximum payload size
    // An extra 1 is subtracted to accomodate for the additional 1 that writeToSocket adds
    // when calculating the length of the buffer (strlen(message) + 1 in writeToSocket)
    size_t max_payload_size = RECV_BUFFER_SIZE - header.str().size() - 1;

    while (payload_str.size() > 0) {
        max_retries = MAX_RETRIES;
        bytes_read = -1;

        // slice the payload to an acceptable size
        std::string slice = payload_str.substr(0, max_payload_size);

        // Check if this is the last payload
        if (max_payload_size >= payload_str.size()) {
            header.fragmented = -1;     // Mark this packet as the last in the sequence of fragmented packets
            payload_str = "";           // Empty payload_str to be able to exit the while loop
        } else
            // remove the slice from the original payload
            payload_str = payload_str.substr(max_payload_size, std::string::npos);

        // Send the slice over to the receiver and wait for an ack
        while (max_retries-- && bytes_read == -1) {
            udpSocket.writeToSocket((char *) (header.str() + slice).c_str(), receiver_sock_addr);
            bytes_read = udpSocket.readFromSocketWithTimeout(ack_buffer, RECV_BUFFER_SIZE, reply_addr, ACK_TIMEOUT);
        }

        // If no ack was received, a connection error has likely occured hence we return -1
        if (bytes_read == -1)
            return -1;

        // If ack was received, increment the sequence id (to be used for the next fragment to be sent) and recalculate
        // the max payload size again (needed for when for when the number of digits of the sequence id is increases)
        header.sequence_id++;
        max_payload_size = RECV_BUFFER_SIZE - header.str().size() - 1;

        // Update the total amount of bytes sent
        total_bytes_sent += bytes_sent;
    }

    return total_bytes_sent;
}

int CM::rebuild_request(char *initial_fragment, std::string &rebuilt_request, sockaddr_in &sender_addr) {
    const int MAX_RETRIES = 5;
    int max_retries;

    ssize_t bytes_read;
    ssize_t total_bytes_read = 0;

    // Store the sequence id of the last received message because we rebuild the request sequentially
    int last_sequence_id_recv = 0;

    //
    char recv_buffer[RECV_BUFFER_SIZE];

    // Create Header and Payload of ack message
    rebuilt_request = initial_fragment;
    Header ack_header = Header((char *) rebuilt_request.c_str());
    ack_header.message_type = MessageType::Ack;
    auto v = std::vector<std::string> {"ack"};
    Payload ack_payload("0", v.size(), v, false);


    bool still_fragmented = true;

    while (still_fragmented) {
        // Send ack for previous packet
        max_retries = MAX_RETRIES;
        bytes_read = -1;

        // Create an ack message
        Message ack(ack_header, ack_payload);
        std::string ack_str = ack.marshal();

        // Send an ack for the previously received message and wait for a new one
        while (max_retries-- && bytes_read == -1) {
            udpSocket.writeToSocket((char *) ack_str.c_str(), sender_addr);
            bytes_read = udpSocket.readFromSocketWithTimeout(recv_buffer, RECV_BUFFER_SIZE, sender_addr, 150);
        }

        // If we exit the while loop without receiving anything, a connection error might have occured and we return -1
        if (bytes_read == -1)
            return -1;

        // Get the recv_header of the received packet
        Header recv_header = Header(recv_buffer);
//        std::cout << "Bytes read: " << bytes_read << std::endl;

        // An error occured, we should have received -1 to indicate the last fragmented packet and not a 0
        if (recv_header.fragmented == 0 || recv_header.sequence_id > last_sequence_id_recv + 1)
            return -1;

        // Last packet in the fragmented packets
        if (recv_header.fragmented == -1) {
            still_fragmented = false;
            total_bytes_read += bytes_read;

            // Only send ack once
            // If sender does not receive this ack, recv_with_block or recv_with_timeout
            // will handle resending the ack again
            udpSocket.writeToSocket((char *) ack_str.c_str(), sender_addr);
        }

        if (recv_header.sequence_id == last_sequence_id_recv + 1) {
            last_sequence_id_recv++;
            total_bytes_read += bytes_read;

            // Retrieve this message's payload and append it to rebuilt_request
            Payload payload(recv_buffer, true);
            rebuilt_request += payload.str();
//            std::cout << payload.str() << std::endl;

            // Increment sequence id of next ack header
            ack_header.sequence_id++;
        }
    }
//    std::cout << "Rebuilt request: " << rebuilt_request << std::endl;
    return (int) total_bytes_read;
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
