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
    switch (request.getOperation()) {
        case 0: {
            std::vector<std::string> v;
            auto n = view_imagelist_svc(v);
            Message reply(MessageType::Reply, 0, request.getRPCId(), v.size(), v);
            serverConnector.send_no_ack(reply, sender_addr);
            break;
        }
        case 1: {
            // kamel yad :)
            break;
        }
        case 2: {
            break;
        }
        case 3: {
            break;
        }
        default: {
            break;
        }
    }

}

