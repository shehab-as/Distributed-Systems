//
// Created by shehab on 11/18/16.
//

#ifndef DISTRIBUTED_SYSTEMS_PEER_H
#define DISTRIBUTED_SYSTEMS_PEER_H


#include "../common/CM/CM.h"
#include "../common/registry_rpc.h"
#include "../common/Message/Message.h"
#include "peer_svc.cpp"
#include <string>
#include <thread>
#include <vector>

class Peer {

private:
    CM CM_Client;
    CM CM_Server;
    long int token;
    #include "../common/peer_rpc.h"

public:
    Peer(char *_listen_hostname, uint16_t _listen_port);
    void runServer();
    void handleRequest(Message request, sockaddr_in);

    ~Peer();
};

#endif //DISTRIBUTED_SYSTEMS_PEER_H
