#ifndef DISTRIBUTED_SYSTEMS_REGISTRY_RPC_H
#define DISTRIBUTED_SYSTEMS_REGISTRY_RPC_H

#include <vector>
#include <string>
#include <netinet/in.h>

extern int view_imagelist(std::vector<std::string> &image_container);
extern int view_imagelist_svc(std::vector<std::string> &image_container);

extern int add_entry(std::string image_name);
extern int add_entry_svc(std::string image_name);

extern int remove_entry(std::string image_name);
extern int remove_entry_svc(std::string image_name);

extern int get_client_addr(std::string image_name, sockaddr_in &owner_addr);
extern int get_client_addr_svc(std::string image_name, sockaddr_in &owner_addr);


#endif //DISTRIBUTED_SYSTEMS_REGISTRY_RPC_H
