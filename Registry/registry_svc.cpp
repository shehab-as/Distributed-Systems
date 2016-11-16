#include <vector>
#include <string>
#include <netinet/in.h>

int view_imagelist_svc(std::vector<std::string> &image_container) {
    return 0;
}

int add_entry_svc(std::string image_name){
    return 0;
}

int remove_entry_svc(std::string image_name) {
    return 0;
}

int get_client_addr_svc(std::string image_name, sockaddr_in &owner_addr) {
    return 0;
}