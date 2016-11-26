#include <SQLiteCpp/Database.h>
#include <iostream>
#include <fstream>
#include "Registry.h"

int main() {
//    SQLite::Database    db("/home/farida/Dist-DB.db");
    using namespace std;
    CM server(NULL, 1234);
    Message request;
    sockaddr_in sender_addr;
    ofstream outfile ("WHAT.png", ios::binary);
    while(true) {
        auto n = server.recv_with_block(request, MessageType::Request, sender_addr);
        if (n == -1) {
            cout << "n is -1" <<endl;
            continue;
        }
        std::cout << "Message request size: " << request.marshal().size() << std::endl;
        outfile << request.getParams()[0];
        request.setMessageType(MessageType::Reply);
        server.send_no_ack(request, sender_addr);
    }
}