//
// Created by shehab on 11/18/16.
//

#ifndef DISTRIBUTED_SYSTEMS_PEER_H
#define DISTRIBUTED_SYSTEMS_PEER_H


#include "../common/CM/CM.h"
#include "../common/Message/Message.h"
#include <string>
#include <thread>
#include <vector>

class Peer {

private:
    CM CM_Client;
    CM CM_Server;
    long int token;

public:
    Peer(char *_listen_hostname, uint16_t _listen_port);
    void runServer();
    void handleRequest(Message request, sockaddr_in);

    ~Peer();

    // Peer RPC stubs
    int download_image(std::string image_name, long int token, std::vector<std::string> &reply_params);

    // Peer RPC implementations
    int download_image_svc(std::string image_name, long int token, std::vector<std::string>& reply_params);

    // Registry RPC Stubs
    int view_imagelist(std::vector<std::string> &image_container, long int token);
    int add_entry(std::string image_name, long int token);
    int remove_entry(std::string image_name, long int token);
    int get_client_addr(std::string image_name, std::string &owner_addr, uint16_t &owner_port, long int token);
    int retrieve_token(std::string username, std::string password, long int &token);
    int check_viewImage(std::string image_id, bool &can_view, long int token);

};

#endif //DISTRIBUTED_SYSTEMS_PEER_H
