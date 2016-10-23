#include <iostream>
#include <netdb.h>
#include <thread>
#include "UDP/UDPSocket.h"

using namespace std;

char message_received[2];
auto server = UDPSocket();
auto client = UDPSocket();
bool flag;

void server_test() {
    server.readFromSocketWithBlock(message_received, 8);
}

void client_test() {
    client.writeToSocket((char *) "hi", 8);
}

int main() {
    flag = server.initializeServer((char *) "localhost", 1234);
    flag = client.initializeClient((char *) "localhost", 1234);

    std::thread t1(server_test);
    std::thread t2(client_test);
    t2.join();
    t1.join();
    cout << message_received << endl;
}