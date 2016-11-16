//
// Created by farida on 16/11/16.
//

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

void Registry::runRegistry() {

    sockaddr_in sender_addr;
    char buffer[BUFFER_SIZE];

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    while (true) {
        ssize_t bytes_read = serverConnector.recv_with_block(buffer, BUFFER_SIZE, sender_addr);
        handleRequest(buffer, sender_addr);
//        Message *replyFromServer;   // msg from server


    }
#pragma clang diagnostic pop
}
// THE GREAT SWITCH
//0: view_imagelist_svc(std::vector<std::string> &image_container);
//1: add_entry_svc(std::string image_name);
//2: remove_entry_svc(std::string image_name);
//3: get_client_addr_svc(std::string image_name, sockaddr_in &owner_addr);

void Registry::handleRequest(char *request_buffer, sockaddr_in sender_addr) {

    Message request(request_buffer);

    switch (request.getOperation()) {
        case 0:
            std::vector<std::string> v;
            auto n = view_imagelist_svc(v);
            Message reply(MessageType::Reply, 0, request.getRPCId(), v.size(), v);
            auto reply_marshaled = reply.marshal();
            serverConnector.send_no_ack((char *) reply_marshaled.c_str(), sender_addr);
        case 1:
            // kamel yad :)


    }


}

