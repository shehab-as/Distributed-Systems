#include <iostream>
#include <vector>
#include "Server.h"
#include "../common/CM/CM.h"

//Testing
int main(int argc, char *argv[]) {

    CM server((char *) "10.10.1.1", 1234);
    size_t BUFFER_SIZE = 7500;
    char BUFFER[BUFFER_SIZE];
    sockaddr_in sender;

    server.recv_with_block(BUFFER, BUFFER_SIZE, sender);
    server.send_no_ack(BUFFER, sender);



}
