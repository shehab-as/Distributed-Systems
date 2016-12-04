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
}

Peer::~Peer() {}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

void Peer::runServer() {
    sockaddr_in sender_addr;

    while (true) {
        Message recv_message = Message();
        ssize_t bytes_read = CM_Server.recv_with_block(recv_message, MessageType::Request, sender_addr);
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
    std::string owner_addr;
    uint16_t owner_port;

//    int n = get_client_addr(image_name, owner_addr, owner_port, token);

    // Needed conversion
    owner_addr = "10.40.35.51";
    owner_port = 1234;
//    owner_addr = std::to_string(htonl((uint32_t) std::stol(owner_addr)));
//    owner_port = htons(owner_port);
    int n = SUCCESS;
    if (n == SUCCESS) {
        std::vector<std::string> v{image_name, std::to_string(token)};
        Message request(MessageType::Request, 0, RPC_Count++, "NULL", v.size(), v);
        Message reply;

        n = CM_Client.send_with_ack(request, reply, 1000, 10, (char *) owner_addr.c_str(), owner_port);

        if (n == -1)
            return CONNECTION_ERROR;

        std::vector<std::string> REPLY = reply.getParams();

        for (int i = 0; i < reply.getParamsSize(); i++) {
            reply_params.push_back(REPLY[i]);
        }

        int reply_return_val = stoi(reply.getReturnVal());

        return reply_return_val;
    }

    return n;
}

//////////////////////////////////////////////////
//             Peer RPC Implementation          //
/////////////////////////////////////////////////

// TODO: Finish this
int Peer::download_image_svc(std::string image_name, long int token, std::vector<std::string> &reply_params) {
    bool can_view;

    // Check if requesting user can view the image
//    int n = check_viewImage(image_name, can_view, token);
    int n = SUCCESS;
    if (n == SUCCESS) {
        try {
            //Hardcoded Value 10
            int views = 1;
            std::string views_text = image_name.substr(0, image_name.find('.')) + "_Views.txt";
            std::string dummy_image = "dummy.jpg";
            std::ofstream File;
            File.open(views_text);
            File << views;
            File.close();
            std::string CMD;
            //Encoding.
            CMD = "steghide embed -cf " + image_name + " -ef " + views_text + " -p '' ";
            system(CMD.c_str());
            CMD = "steghide embed -cf " + dummy_image + " -ef " + image_name + " -p '' ";
            system(CMD.c_str());

            // TODO: Encode the original image into the dummy one here
            std::ifstream t(dummy_image, std::ios::binary);
            std::string image_data;

            t.seekg(0, std::ios::end);
            image_data.reserve(t.tellg());
            t.seekg(0, std::ios::beg);

            image_data.assign((std::istreambuf_iterator<char>(t)),
                              std::istreambuf_iterator<char>());

            reply_params.push_back(image_data);
            return 0;
        }
        catch (const std::exception &e) {
            std::cout << e.what() << std::endl;
        }
    }
    return n;
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

    if (reply_return_val == -1)
        return UNAUTHORIZED_ACCESS;

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

int Peer::add_entry(std::string image_name, long int token) {
    std::vector<std::string> V{image_name, std::to_string(token)};
    Message request(MessageType::Request, 1, RPC_Count++, "NULL", V.size(), V);
    Message reply;
    int n = CM_Server.send_with_ack(request, reply, 500, 5, (char *) registry_addr.c_str(), registry_port);

    if (n == -1)
        return CONNECTION_ERROR;

    int reply_return_val = std::stoi(reply.getReturnVal());

    if (reply_return_val == -1)
        return GENERAL_ERROR;

    return SUCCESS;
}

int Peer::remove_entry(std::string image_name, long int token) {
    std::vector<std::string> V{image_name, std::to_string(token)};
    Message request(MessageType::Request, 2, RPC_Count++, "NULL", V.size(), V);
    Message reply;
    int n = CM_Server.send_with_ack(request, reply, 500, 5, (char *) registry_addr.c_str(), registry_port);

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

int Peer::set_image_viewable_by(std::string image_id, long int user_token, std::string allowed_user) {
    std::vector<std::string> v{image_id, std::to_string(user_token), allowed_user};
    Message request(MessageType::Request, 7, RPC_Count++, "NULL", v.size(), v);
    Message reply;
    int n = CM_Client.send_with_ack(request, reply, 500, 5, (char *) registry_addr.c_str(), registry_port);

    if (n == -1)
        return CONNECTION_ERROR;

    int request_reply_val = stoi(reply.getReturnVal());

    if (request_reply_val == -1)
        return GENERAL_ERROR;

    return SUCCESS;
}
