#include <vector>
#include <string>
#include <netinet/in.h>
#include "Registry.h"

int Registry::view_imagelist_svc(std::vector<std::string> &image_container, long int token){
    return 0;
}

int Registry::add_entry_svc(std::string image_name, long int token)
{
    return 0;
}

int Registry::remove_entry_svc(std::string image_name, long int token)
{
    return 0;
}

int Registry::get_client_addr_svc(std::string image_name, std::string &owner_addr, uint16_t &owner_port, long int token)
{
    return 0;
}

int Registry::retrieve_token_svc(std::string username, std::string password, long int &token)
{
    return 0;
}

int Registry::check_viewImage_svc(std::string image_id, bool &can_view, long int token)
{
    return 0;
}