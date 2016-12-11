// Created by shehab on 11/18/16.
//

#ifndef DISTRIBUTED_SYSTEMS_PEER_H
#define DISTRIBUTED_SYSTEMS_PEER_H


#include "../common/CM/CM.h"
#include "../common/Message/Message.h"
#include <string>
#include <thread>
#include <vector>

#define CONNECTION_ERROR -1
#define UNAUTHORIZED_ACCESS -2
#define GENERAL_ERROR -3
#define SUCCESS 0

class Peer {
private:
    CM CM_Client;
    CM CM_Server;
    unsigned long long int RPC_Count = 0;

public:
    std::string registry_addr;
    uint16_t registry_port;

    Peer(char *_listen_hostname, uint16_t _listen_port);
    void runServer();
    void handleRequest(Message request, sockaddr_in);

    virtual ~Peer();

    // Peer RPC stubs
    int download_image(std::string image_name, long int token, std::vector<std::string> &reply_params);

    // RPC implementations
    int download_image_svc(std::string image_name, long int token, std::vector<std::string>& reply_params);

    // Registry RPC Stubs
    int view_imagelist(std::vector<std::string> &image_container, long int token);
    int add_entry(std::string image_name, long int token);
    int remove_entry(std::string image_name, long int token);
    int get_client_addr(std::string image_name, std::string &owner_addr, uint16_t &owner_port, long int token);
    int retrieve_token(std::string username, std::string password, long int &token);
    int check_viewImage(std::string image_name, bool &can_view, long int token);
    int set_image_viewable_by(std::string image_id, long int user_token, std::string allowed_user, int views);
    //New Functions
    int revoke_access(std::string image_id, long int user_token, std::string user_to_revoke);
    int update_User_views(std::string image_name, long int user_token, std::string allowed_user, int views);
    int retrieve_updated_views(std::string image_name, long int user_token, int& views);
    int decrement_views_internal(std::string image_name, long int user_token, int views);
};


#endif //DISTRIBUTED_SYSTEMS_PEER_H
