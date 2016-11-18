//
// Created by shehab on 11/18/16.
//

#include "Peer.h"

// Peer Constructor initializes the CM-CLient and sets its port to default 0 while the Servers sets
// its port to defined listen port.
Peer::Peer(char *_listen_hostname, uint16_t _listen_port):
        CM_Client(_listen_hostname, 0),
        CM_Server(_listen_hostname,  _listen_port)
{
    //Each object should have two threads.
    // Server thread that keeps listening infinitely.
    // Client thread which provides an interface in Qt.
    std::thread Peer_Server, Peer_Client;


}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
void Peer::runServer()
{
    sockaddr_in sender_addr;
    while(true)
    {
        Message recv_message = Message();
        ssize_t bytes_read = CM_Server.recv_with_block(recv_message, sender_addr);
        handleRequest(recv_message, sender_addr);
    }

}
#pragma clang diagnostic pop

void Peer::handleRequest(Message request, sockaddr_in sender_addr) {
    // Only One case added for now.
    switch (request.getOperation()) {
        //Operation 0: Download Image
        case 0: {
            std::vector<std::string> params;  // params[0] = image_name, params[size-1] = token
            params = request.getParams();
            std::string image_name = params[0];
            long int token = stoi(params[params.size() - 1]);
            std::vector<std::string> reply_params;
            Message image_msg;
            auto n = download_image_svc(image_name, token, reply_params);
            image_msg = Message(MessageType::Reply, 0, request.getRPCId(), std::to_string(n), reply_params.size(),
                                reply_params);
            CM_Server.send_no_ack(image_msg, sender_addr);
            break;
        }

        default:
            break;
    }
}

Peer::~Peer() {

}