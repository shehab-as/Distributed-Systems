//
// Created by shadyf on 11/16/16.
//

#ifndef DISTRIBUTED_SYSTEMS_PEER_RPC_H
#define DISTRIBUTED_SYSTEMS_PEER_RPC_H

#include <string>

extern int download_image(std::string image_name);
extern int download_image_svc(std::string image_name);

extern long int retrieve_token(std::string username, std::string password);
extern long int retrieve_token_svc(std::string username, std::string password);

#endif //DISTRIBUTED_SYSTEMS_PEER_RPC_H
