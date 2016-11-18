#include <thread>
#include <string>
#include <vector>
#include "Registry.h"
#include "../common/Message/Message.h"
#include "../common/registry_rpc.h"

Registry::Registry(char *_listen_hostname, uint16_t _listen_port, int num_of_workers) : serverConnector(
        _listen_hostname, _listen_port) {

    std::vector<std::thread> workers;

    for (int i = 0; i < num_of_workers; i++)
        workers.push_back(std::thread(&Registry::runRegistry, this));

    for (int i = 0; i < num_of_workers; i++)
        workers[i].join();

}

Registry::~Registry() {

}

// Fancy way to surpress infinite loop warning inside of this method
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

void Registry::runRegistry() {
    // The core of the registry
    // Each worker thread should run this method
    sockaddr_in sender_addr;

    while (true) {
        Message recv_message = Message();
        ssize_t bytes_read = serverConnector.recv_with_block(recv_message, sender_addr);
        handleRequest(recv_message, sender_addr);
    }
}

#pragma clang diagnostic pop

void Registry::handleRequest(Message request, sockaddr_in sender_addr) {
    // THE GREAT SWITCH
    // Check which RPC method/operation was called and send reply accordingly
    // 0: view_imagelist_svc(std::vector<std::string> &image_container);
    // 1: add_entry_svc(std::string image_name);
    // 2: remove_entry_svc(std::string image_name);
    // 3: get_client_addr_svc(std::string image_name, sockaddr_in &owner_addr);
    // 4: retrieve_token_svc(std::string username, std::string password, int &token);
    // 5: check_viewImage_svc(std::string image_id, bool &can_view, int token);
    switch (request.getOperation()) {
        case 0: {
            // 0: view_imagelist_svc(std::vector<std::string> &image_container);
            std::vector<std::string> image_container;
            std::vector<std::string> params;
            params = request.getParams();
            //Note here: taking only token from params vector to call view_imagelist function.
            //~Shehab
            long int token = stoi(params[params.size() - 1]);
            auto n = view_imagelist_svc(image_container, token);
            Message reply(MessageType::Reply, 0, request.getRPCId(), std::to_string(n), image_container.size(),
                          image_container);
            serverConnector.send_no_ack(reply, sender_addr);
            break;
        }
        case 1: {
            // 1: add_entry_svc(std::string image_name, int token);
            std::vector<std::string> params;  // params[0] = image_name, params[size-1] = token
            params = request.getParams();
            std::string image_name = params[0];
            long int token = stoi(params[params.size() - 1]);
            auto n = add_entry_svc(image_name, token);
            Message reply(MessageType::Reply, 1, request.getRPCId(), std::to_string(n), 0, NULL);
            serverConnector.send_no_ack(reply, sender_addr);
            break;
        }
        case 2: {
            // 2: remove_entry_svc(std::string image_name, int token);
            std::vector<std::string> params;
            params = request.getParams();
            std::string image_name = params[0];     // params[0] = image_name, params[size-1] = token
            long int token = stoi(params[params.size() - 1]);
            auto n = remove_entry_svc(image_name, token);
            Message reply(MessageType::Reply, 2, request.getRPCId(), std::to_string(n), 0, NULL);
            serverConnector.send_no_ack(reply, sender_addr);
            break;
        }
        case 3: {
            // 3: int get_client_addr_svc(std::string image_name, std::string &owner_addr, int &owner_port, int token);
            std::vector<std::string> params;
            params = request.getParams();
            std::string image_name = params[0];
            std::string owner_addr;
            uint16_t owner_port;
            long int token = stoi(params[params.size() - 1]);
            auto n = get_client_addr_svc(image_name, owner_addr, owner_port, token);
            std::vector<std::string> reply_params;
            reply_params.push_back(owner_addr);
            reply_params.push_back(std::to_string(owner_port));
            Message reply(MessageType::Reply, 3, request.getRPCId(), std::to_string(n), reply_params.size(),
                          reply_params);
            serverConnector.send_no_ack(reply, sender_addr);
            break;
        }

        case 4: {
            // 4: int retrieve_token_svc(std::string username, std::string password, int &token);
            std::vector<std::string> params;
            params = request.getParams();
            std::string username = params[0];
            std::string password = params[1];
            long int token;
            auto n = retrieve_token_svc(username, password, token);
            std::vector<std::string> reply_params;
            reply_params.push_back(std::to_string(token));
            Message reply(MessageType::Reply, 4, request.getRPCId(), std::to_string(n), reply_params.size(), reply_params);
            serverConnector.send_no_ack(reply, sender_addr);
            break;
        }

        case 5: {
            //int check_viewImage_svc(std::string image_id, bool &can_view, int token);
            std::vector<std::string> params;
            params = request.getParams();
            std::string image_id = params[0];
            long int token = stoi(params[params.size() - 1]);
            bool can_view;
            auto n = check_viewImage_svc(image_id, can_view, token);
            std::vector<std::string> reply_params;
            reply_params.push_back(std::to_string(can_view));
            Message reply(MessageType::Reply, 5, request.getRPCId(), std::to_string(n), reply_params.size(), reply_params);
            serverConnector.send_no_ack(reply,sender_addr);
            break;
        }

        default: {
            break;
        }
    }

}

