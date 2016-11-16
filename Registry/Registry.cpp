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
    char receiving_buffer[BUFFER_SIZE];

    while (true) {
        ssize_t bytes_read = serverConnector.recv_with_block(receiving_buffer, BUFFER_SIZE, sender_addr);
        handleRequest(receiving_buffer, sender_addr);
    }
}

#pragma clang diagnostic pop

void Registry::handleRequest(char *request_buffer, sockaddr_in sender_addr) {
    // Reconstruct Message from the marshalled request_buffer
    Message request(request_buffer);

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
            auto reply_marshaled = reply.marshal();
            serverConnector.send_no_ack((char *) reply_marshaled.c_str(), sender_addr);
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

