#ifndef DISTRIBUTED_SYSTEMS_REGISTRY_RPC_H
#define DISTRIBUTED_SYSTEMS_REGISTRY_RPC_H

#include <vector>
#include <string>
#include <netinet/in.h>

extern int view_imagelist(std::vector<std::string> &image_container, int token);
extern int view_imagelist_svc(std::vector<std::string> &image_container, int token);

extern int add_entry(std::string image_name, int token);
extern int add_entry_svc(std::string image_name, int token);

extern int remove_entry(std::string image_name, int token);
extern int remove_entry_svc(std::string image_name, int token);

extern int get_client_addr(std::string image_name, std::string &owner_addr, int &owner_port, int token);
extern int get_client_addr_svc(std::string image_name, std::string &owner_addr, int &owner_port, int token);

extern int retrieve_token(std::string username, std::string password, int &token);
extern int retrieve_token_svc(std::string username, std::string password, int &token);

extern int check_viewImage(std::string image_id, bool &can_view, int token);
extern int check_viewImage_svc(std::string image_id, bool &can_view, int token);



#endif //DISTRIBUTED_SYSTEMS_REGISTRY_RPC_H
