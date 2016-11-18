#include <vector>
#include <string>
#include <netinet/in.h>

int view_imagelist_svc(std::vector<std::string> &image_container, int token) {
    return 0;
}

int add_entry_svc(std::string image_name, int token){
    return 0;
}

int remove_entry_svc(std::string image_name, int token) {
    return 0;
}

int get_client_addr_svc(std::string image_name, std::string &owner_addr, int &owner_port, int token) {
    return 0;
}

int retrieve_token_svc(std::string username, std::string password, int &token){
    return 0;
}

int check_viewImage_svc(std::string image_id, bool &can_view, int token);