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
    ~Registry();

    void runRegistry();
    void handleRequest(Message request, sockaddr_in);

    // Registry RPC implementations
    int view_imagelist_svc(std::vector<std::string> &image_container, long int token);
    int add_entry_svc(std::string image_name, long int token);
    int remove_entry_svc(std::string image_name, long int token);
    int get_client_addr_svc(std::string image_name, std::string &owner_addr, uint16_t &owner_port, long int token);
    int retrieve_token_svc(std::string username, std::string password, long int &token);
    int check_viewImage_svc(std::string image_id, bool &can_view, long int token);

};


#endif //DISTRIBUTED_SYSTEMS_REGISTRY_H
