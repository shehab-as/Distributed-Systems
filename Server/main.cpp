#include <iostream>
#include <vector>
#include "Server.h"

//Testing
int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Arguments: SERVER_PORT NUM_OF_WORKERS";
        return -1;
    }
    long port = strtol(argv[1], NULL, 10);
    long num_of_workers = strtol(argv[2], NULL, 10);
    
    auto server = Server((char *) "localhost", (uint16_t) port, (int) num_of_workers);
}
