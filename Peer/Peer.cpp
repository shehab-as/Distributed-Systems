//
// Created by shehab on 11/18/16.
//

#include <ios>
#include <fstream>
#include <iostream>
#include <SQLiteCpp/Database.h>
#include "Peer.h"
//#include "QVector"

// Peer Constructor initializes the CM-CLient and sets its port to default 0 while the Servers sets
// its port to defined listen port.
Peer::Peer(char *_listen_hostname, uint16_t _listen_port) :
        CM_Client(_listen_hostname, 0),
        CM_Server(_listen_hostname, _listen_port) {
    //Each object should have two threads.
    // Server thread that keeps listening infinitely.
    // Client thread which provides an interface in Qt.
    std::thread Peer_Server, Peer_Client;

}

Peer::~Peer() {

    CM_Client.~CM();
    CM_Server.~CM();
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

void Peer::runServer() {
    sockaddr_in sender_addr;
    while (true) {
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

///////////////////////////////////////////////////
//                Peer RPC Stubs                //
//////////////////////////////////////////////////

// TODO: Finish this
int Peer::download_image(std::string image_name, long int token, std::vector<std::string> &reply_params) {
    std::vector<std::string> V{image_name, std::to_string(token)};
    Message request(MessageType::Request, 0, RPC_Count++, "NULL", V.size(), V);
    Message reply;
    int n = CM_Client.send_with_ack(request, reply, 500, 5, (char *) registry_addr.c_str(), registry_port);

    if (n == -1)
        return CONNECTION_ERROR;

    std::vector<std::string> REPLY = reply.getParams();
    for (int i = 0; i < reply.getParamsSize(); i++) {
        reply_params.push_back(REPLY[i]);
    }

}

//////////////////////////////////////////////////
//             Peer RPC Implementation          //
/////////////////////////////////////////////////

// TODO: Finish this
int Peer::download_image_svc(std::string image_name, long int token, std::vector<std::string> &reply_params) {
//    int n = check_token(token);
    //if n = 0, success.
    //Else return -1
    int n = 0;
    if (n == 0) {
        try {
            //Opening image file to stream then storing it into a string.
            std::ifstream fin(image_name, std::ios::binary);
            std::string image_data;
            image_data.reserve(1000000);
//        Not working
//        std::copy(std::istreambuf_iterator<char>(fin),
//                  std::istreambuf_iterator<char>(),
//                  std::back_insert_iterator(image_data));

            reply_params.push_back(image_data);
        }
        catch (const std::exception &e) {
            std::cout << e.what() << std::endl;
        }
    }

    return 0;
}

//////////////////////////////////////////////////
//             Registry RPC Stubs               //
/////////////////////////////////////////////////
int Peer::retrieve_token(std::string username, std::string password, long int &token) {
    std::vector<std::string> V{username, password};
    Message request(MessageType::Request, 4, RPC_Count++, "NULL", V.size(), V);
    Message reply;
    int n = CM_Client.send_with_ack(request, reply, 500, 5, (char *) registry_addr.c_str(), registry_port);
    if (n < 0)
        return CONNECTION_ERROR;

    int reply_return_val = std::stoi(reply.getReturnVal());
    if (reply_return_val != 0)
        return GENERAL_ERROR;

    token = std::stol(reply.getParams()[0]);
    return SUCCESS;
}

int Peer::view_imagelist(std::vector<std::string> &image_container, long int token) {
    std::vector<std::string> V{std::to_string(token)};
    Message request(MessageType::Request, 0, RPC_Count++, "NULL", V.size(), V);
    Message reply;
    int n = CM_Client.send_with_ack(request, reply, 500, 5, (char *) registry_addr.c_str(), registry_port);
    if (n < 0)
        return CONNECTION_ERROR;

    int reply_return_val = std::stoi(reply.getReturnVal());
    if (reply_return_val == -1)
        return GENERAL_ERROR;

    size_t size_vec = reply.getParamsSize();
    std::vector<std::string> reply_vector = reply.getParams();

    for (int i = 0; i < size_vec; i++) {
        image_container.push_back(reply_vector[i]);
    }

    return SUCCESS;
}

// TODO: Finish this, it's NOT DONE!
int Peer::add_entry(std::string image_name, long int token) {
    std::vector<std::string> V{image_name, std::to_string(token)};
    Message request(MessageType::Request, 1, RPC_Count++, "NULL", V.size(), V);
    Message reply;
    int n = CM_Client.send_with_ack(request, reply, 500, 5, (char *) registry_addr.c_str(), registry_port);


}

//NOT DONE!
int Peer::remove_entry(std::string image_name, long int token) {
    std::vector<std::string> V{image_name, std::to_string(token)};
    Message request(MessageType::Request, 2, RPC_Count++, "NULL", V.size(), V);
    Message reply;
    int n = CM_Client.send_with_ack(request, reply, 500, 5, (char *) registry_addr.c_str(), registry_port);

    if (n == -1)
        return CONNECTION_ERROR;

    int reply_return_val = stoi(reply.getReturnVal());
    if (reply_return_val == -1)
        return GENERAL_ERROR;

    return SUCCESS;
}

int Peer::get_client_addr(std::string image_name, std::string &owner_addr, uint16_t &owner_port, long int token) {
    std::vector<std::string> V{image_name, std::to_string(token)};
    Message request(MessageType::Request, 3, RPC_Count++, "NULL", V.size(), V);
    Message reply;
    int n = CM_Client.send_with_ack(request, reply, 500, 5, (char *) registry_addr.c_str(), registry_port);

    if (n == -1)
        return CONNECTION_ERROR;

    int reply_return_val = stoi(reply.getReturnVal());
    if (reply_return_val == -1)
        return GENERAL_ERROR;

    owner_addr = reply.getParams()[0];
    owner_port = (uint16_t) stoi(reply.getParams()[1]);

    return SUCCESS;
}

int Peer::check_viewImage(std::string image_name, bool &can_view, long int token) {
    std::vector<std::string> V{image_name, std::to_string(token)};
    Message request(MessageType::Request, 5, RPC_Count++, "NULL", V.size(), V);
    Message reply;
    int n = CM_Client.send_with_ack(request, reply, 500, 5, (char *) registry_addr.c_str(), registry_port);

    if (n == -1)
        return CONNECTION_ERROR;

    int request_reply_val = stoi(reply.getReturnVal());

    if (request_reply_val == -1)
        return GENERAL_ERROR;

    can_view = true;
    return SUCCESS;
}


int main() {
//    SQLite::Database    db("/home/farida/Dist-DB.db");
    using namespace std;
    CM server(NULL, 1234);
    Message request;
    sockaddr_in sender_addr;
    ofstream outfile("surprise.png", ios::binary);
    while (true) {
        auto n = server.recv_with_block(request, sender_addr);
        if (n == -1) {
            cout << "n is -1" << endl;
            continue;
        }
        std::cout << "Message request size: " << request.marshal().size() << std::endl;
        outfile << request.getParams()[0];
        server.send_no_ack(request, sender_addr);
    }
}
