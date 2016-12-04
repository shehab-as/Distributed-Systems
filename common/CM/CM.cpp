#include <iostream>
#include <netdb.h>
#include <sstream>
#include "CM.h"

// TODO: Communication Manager should handle marshalling/unmarshalling of Message

CM::CM(char *_myAddr, uint16_t _myPort) {
    udpSocket.initializeSocket(_myAddr, _myPort);
    replies.resize(REPLIES_SIZE);
    fill(replies.begin(), replies.end(), "");
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
    ssize_t bytes_sent;

    // Create a sockaddr from the receiver's addr and port
    sockaddr_in receiver_sock_addr = create_sockaddr(receiver_addr, receiver_port);
    sockaddr_in sender_addr;

    int max_send_retries;
    int max_recv_retries;
    received_message.setRPCId(message_to_send.getRPCId()+1);

    // Send message and wait for a reply
    // If no reply is sent within timeout_in_ms, bytes_read will be -1
    // If a reply is received, bytes_read will contain a value > 0 indicating the number of bytes read
    // We repeat this send/wait for reply sequence until a reply is received or max_retries reaches 0
    while (max_retries-- && bytes_read == -1) {
        max_send_retries = max_recv_retries = max_retries;
        bytes_sent = -1;

        if(replies[message_to_send.getRPCId() % REPLIES_SIZE] != "") {
            received_message = Message((char *) replies[message_to_send.getRPCId() % REPLIES_SIZE].c_str());
            bytes_read = replies[message_to_send.getRPCId() % REPLIES_SIZE].size();
            break;
        }

        while (max_send_retries-- && bytes_sent == -1)
            bytes_sent = send_no_ack(message_to_send, receiver_sock_addr);

        if (bytes_sent != -1)
            while(max_recv_retries-- && message_to_send.getRPCId() != received_message.getRPCId())
                bytes_read = recv_with_timeout(received_message, MessageType::Reply, sender_addr, timeout_in_ms);
    }

    return (int) (message_to_send.getRPCId() == received_message.getRPCId() ? bytes_read : -1);
}

int CM::send_no_ack(Message message_to_send, char *receiver_addr, uint16_t receiver_port) {
    // Create receiver_sock_addr from receiver's addr and port
    sockaddr_in receiver_sock_addr = create_sockaddr(receiver_addr, receiver_port);

    return (int) send_message(message_to_send, receiver_sock_addr);
}

int CM::send_no_ack(Message message_to_send, sockaddr_in receiver_sock_addr) {
    return (int) send_message(message_to_send, receiver_sock_addr);
}

ssize_t CM::recv_with_timeout(Message &received_message, MessageType message_filter, sockaddr_in &sender_addr,
                              int timeout_in_ms) {
    ssize_t bytes_read;
    char recv_buffer[RECV_BUFFER_SIZE];
    std::string recv_request;
    bool rebuilt = false;
    // Extract the packet's recvd_header
    Header recvd_header;

    // If message filter is Frag, check for LastFrag aswell
    MessageType last_frag_filter = message_filter;
    if (message_filter == MessageType::Frag)
        last_frag_filter = MessageType::LastFrag;

    // If the received message is an ack or part of a fragmented message, this would indicate that a connection error occured
    // in rebuild_request and these are merely the leftovers from the previous unsuccesful request, in which case they
    // should be dropped/not handled
    do {
        bytes_read = udpSocket.readFromSocketWithTimeout(recv_buffer, RECV_BUFFER_SIZE, sender_addr, timeout_in_ms);

        if (bytes_read == -1)
            return -1;

        recvd_header = Header(recv_buffer);

        if (recvd_header.message_type == MessageType::Reply && recvd_header.fragmented == 0) {
            std::string recvd_str = recv_buffer;
            replies[recvd_header.rpc_id % REPLIES_SIZE] = recvd_str;
            break;
        }

        // If sender did not get the ack for the previous message's last fragment, resend the ack
        // This is done to not let the sender report a connection error due to the last fragment's
        // ack not being received (rebuild_request only send out one ack with no retries for the last fragment)
        if (recvd_header.message_type == MessageType::LastFrag && message_filter != MessageType::LastFrag)
            create_and_send_ack(recvd_header, sender_addr);

    } while (recvd_header.message_type != message_filter && recvd_header.message_type != last_frag_filter);

    // Call rebuild_request if recvd_header indicated that the message is fragmented
    if ((recvd_header.message_type == MessageType::Request || recvd_header.message_type == MessageType::Reply)
        && recvd_header.fragmented == 1) {
        bytes_read = rebuild_request(recv_buffer, recv_request, sender_addr);
        rebuilt = true;
    } else
        recv_request = recv_buffer;

    // If message was successfully received, unmarshall and build received_message
    if (bytes_read >= 0) {
        Payload recvd_payload;
        if (rebuilt)
            recvd_payload = Payload((char *) recv_request.c_str(), false);
        else
            recvd_payload = Payload((char *) recv_request.c_str(), recvd_header.message_type == MessageType::Frag ||
                                                                   recvd_header.message_type == MessageType::LastFrag);
        received_message = Message(recvd_header, recvd_payload);
    }

    return bytes_read;
}

int CM::recv_with_block(Message &received_message, MessageType message_filter, sockaddr_in &sender_addr) {
    // A blocking receive that fills received_message with the received contents
    char recv_buffer[RECV_BUFFER_SIZE];
    std::string recv_request;
    ssize_t bytes_read;
    bool rebuilt = false;
    // Extract the packet's recvd_header
    Header recvd_header;

    // If message filter is Frag, check for LastFrag aswell
    MessageType last_frag_filter = message_filter;
    if (message_filter == MessageType::Frag)
        last_frag_filter = MessageType::LastFrag;
    // If the received message is an ack or part of a fragmented message, this would indicate that a connection error occured
    // in rebuild_request and these are merely the leftovers from the previous unsuccesful request, in which case they
    // should be dropped/not handled
    do {
        bytes_read = udpSocket.readFromSocketWithBlock(recv_buffer, RECV_BUFFER_SIZE, sender_addr);

        recvd_header = Header(recv_buffer);

        if (recvd_header.message_type == MessageType::Reply && recvd_header.fragmented == 0) {
            std::string recvd_str = recv_buffer;
            replies[recvd_header.rpc_id % REPLIES_SIZE] = recvd_str;
            break;
        }

        // If sender did not get the ack for the previous message's last fragment, resend the ack
        // This is done to not let the sender report a connection error due to the last fragment's
        // ack not being received (rebuild_request only send out one ack with no retries for the last fragment)
        if (recvd_header.message_type == MessageType::LastFrag && message_filter != MessageType::LastFrag)
            create_and_send_ack(recvd_header, sender_addr);

    } while (recvd_header.message_type != message_filter && recvd_header.message_type != last_frag_filter);

    // Call rebuild_request if recvd_header indicated that the message is fragmented
    if ((recvd_header.message_type == MessageType::Request || recvd_header.message_type == MessageType::Reply)
        && recvd_header.fragmented == 1) {
        bytes_read = rebuild_request(recv_buffer, recv_request, sender_addr);
        rebuilt = true;
    } else
        recv_request = recv_buffer;

    // If message was successfully received, unmarshall and build received_message
    if (bytes_read >= 0) {
        Payload recvd_payload;

        if (rebuilt)
            recvd_payload = Payload((char *) recv_request.c_str(), false);
        else
            recvd_payload = Payload((char *) recv_request.c_str(), recvd_header.message_type == MessageType::Frag ||
                                                                   recvd_header.message_type == MessageType::LastFrag);
        received_message = Message(recvd_header, recvd_payload);
    }

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
    const int ACK_TIMEOUT = 300;

    // Max attempts at sending a fragment
    const int MAX_RETRIES = 5;
    int max_retries;
    int max_send_retries;

    ssize_t bytes_read;
    ssize_t bytes_sent = 0;
    ssize_t total_bytes_sent = 0;

    // ACK buffer to hold acks from the receiver
    Message recvd_ack;

    // sockaddr_in which will store the address of the receiver/replier given they are the same
    // TODO: Make send_fragments return reply address to its caller
    sockaddr_in reply_addr;

    // Mark the message as being fragmented, needed for receiver to prepare itself to rebuild the request
    // in rebuild_request
    message_to_fragment.setFrag(1);
    message_to_fragment.setSeqId(0);

    Header header = message_to_fragment.getHeader();
    auto payload_something = message_to_fragment.getPayload();
    std::string payload_str = payload_something.str();

    // Set the maximum payload size
    // An extra 1 is subtracted to accomodate for the additional 1 that writeToSocket adds
    // when calculating the length of the buffer (strlen(message) + 1 in writeToSocket)
    size_t max_payload_size = RECV_BUFFER_SIZE - header.str().size() - 2;

    std::string slice = payload_str.substr(0, max_payload_size);
    payload_str = payload_str.substr(max_payload_size, std::string::npos);

    bool done = false;
    bool first_fragment = true;
    while (!done) {

        if (payload_str == "")
            done = true;

        max_retries = MAX_RETRIES;
        bytes_read = -1;

        while (max_retries-- && bytes_read == -1) {
            max_send_retries = max_retries;
            bytes_sent = -1;

            Message fragment_to_send = Message(header, slice, true, false);
            while (max_send_retries-- && bytes_sent == -1)
                bytes_sent = send_no_ack(fragment_to_send, receiver_sock_addr);

            if (bytes_sent != -1)
                bytes_read = recv_with_timeout(recvd_ack, MessageType::Ack, reply_addr, ACK_TIMEOUT);
        }

        // If no ack was received, a connection error has likely occured hence we return -1
        if (bytes_read == -1)
            return -1;

        // since ack was received, increment the sequence id (to be used for the next fragment to be sent) and recalculate
        // the max payload size again (needed for when for when the number of digits of the sequence id is increases)
        header.sequence_id++;
        header.message_type = MessageType::Frag;
        first_fragment = false;
        // slice the next payload
        slice = payload_str.substr(0, max_payload_size);

        // Check if this is the last payload
        if (max_payload_size >= payload_str.size()) {
            // Mark this packet as the last in the sequence of fragmented packets
            header.message_type = MessageType::LastFrag;
            header.fragmented = -1;

            // Empty payload_str to be able to exit the while loop
            payload_str = "";
        } else
            // remove the slice from the original payload
            payload_str = payload_str.substr(max_payload_size, std::string::npos);

        max_payload_size = RECV_BUFFER_SIZE - header.str().size() - 2;

        total_bytes_sent += bytes_sent;     // Update the total amount of bytes sent
    }

    return total_bytes_sent <= 0 ? -1 : total_bytes_sent;
}

int CM::rebuild_request(char *initial_fragment, std::string &rebuilt_request, sockaddr_in &sender_addr) {
    // Maximum time (in ms) to wait for an ack
    const int ACK_TIMEOUT = 300;

    const int MAX_RETRIES = 5;
    int max_retries;
    int max_send_retries;

    ssize_t bytes_read;
    ssize_t bytes_sent;
    ssize_t total_bytes_read = 0;

    // Store the sequence id of the last received message because we rebuild the request sequentially
    int last_sequence_id_recv = 0;

    // ACK buffer to hold acks from the receiver
    Message recvd_message;

    // Create Header and Payload of ack message
    rebuilt_request = initial_fragment;
    Header ack_header = Header((char *) rebuilt_request.c_str());
    ack_header.message_type = MessageType::Ack;
    ack_header.fragmented = 0;
    auto v = std::vector<std::string> {"ack"};
    Payload ack_payload("0", v.size(), v, false);


    bool still_fragmented = true;

    while (still_fragmented) {
        // Send ack for previous packet
        max_retries = MAX_RETRIES;
        bytes_read = -1;

        // Create an ack message
        Message ack(ack_header, ack_payload);

        while (max_retries-- && bytes_read == -1) {
            max_send_retries = max_retries;
            bytes_sent = -1;

            while (max_send_retries-- && bytes_sent == -1)
                bytes_sent = send_no_ack(ack, sender_addr);

            if (bytes_sent != -1)
                bytes_read = recv_with_timeout(recvd_message, MessageType::Frag, sender_addr, ACK_TIMEOUT);
        }

        // If we exit the while loop without receiving anything, a connection error might have occured and we return -1
        if (bytes_read == -1)
            return -1;

        // Get the recv_header of the received packet
        Header recv_header = recvd_message.getHeader();


        // TODO: Should check if messagetype is a request aswell
        if (recv_header.message_type == MessageType::Reply) {
            rebuilt_request = recvd_message.marshal();
            return (int) recvd_message.marshal().size();
        }

        // Last packet in the fragmented packets
        if (recv_header.fragmented == -1 && recv_header.message_type == MessageType::LastFrag) {
            still_fragmented = false;
            total_bytes_read += bytes_read;
        }

        if (recv_header.sequence_id == last_sequence_id_recv + 1) {
            // Update sequence id of last received fragment and update the sequence id of the next ack
            last_sequence_id_recv++;
            ack_header.sequence_id++;
            rebuilt_request += recvd_message.getPayload().str();

            total_bytes_read += bytes_read;
        }
    }
    std::cout << "Rebuilt request size: " << rebuilt_request.size() << std::endl;
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

void CM::create_and_send_ack(Header header, sockaddr_in sender_addr) {
    Header ack_header = header;

    ack_header.message_type = MessageType::Ack;
    ack_header.fragmented = 0;
    ack_header.sequence_id = 0;

    std::vector<std::string> v{"ack"};
    Payload ack_payload = Payload("0", v.size(), v, 0);

    // Create ack message from header and payload
    Message ack(ack_header, ack_payload);

    // send the ack
    send_no_ack(ack, sender_addr);
}
