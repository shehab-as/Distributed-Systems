#ifndef DISTRIBUTED_SYSTEMS_REGISTRY_H
#define DISTRIBUTED_SYSTEMS_REGISTRY_H

#include <map>
#include <sqlite3.h>
#include "../common/CM/CM.h"

class Registry {
private:
    CM serverConnector;
    //file (Database)

public:
    Registry (char *_listen_hostname, uint16_t _listen_port, int num_of_workers);
    void runRegistry();
    void handleRequest(Message request, sockaddr_in);
    ~Registry();
};


#endif //DISTRIBUTED_SYSTEMS_REGISTRY_H
