#ifndef DISTRIBUTED_SYSTEMS_REGISTRY_H
#define DISTRIBUTED_SYSTEMS_REGISTRY_H

#include <map>
#include <sqlite3.h>
#include "../common/CM/CM.h"

class Registry {
private:
    CM serverConnector;
    const size_t BUFFER_SIZE = 7500;
    //file (Database)

public:
    Registry (char *_listen_hostname, uint16_t _listen_port, int num_of_workers);
    void runRegistry();
    void handleRequest(char * request_buffer, sockaddr_in);
    ~Registry();
};


#endif //DISTRIBUTED_SYSTEMS_REGISTRY_H
