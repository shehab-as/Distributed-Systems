#include <iostream>
#include <netdb.h>
#include <thread>
#include <cstring>
#include "Client/Client.h"
#include "Server/Server.h"
#include "common/Message/Message.h"

using namespace std;


// For client
void client(int thread_num) {
    auto client = Client((char *) "192.168.1.100", 1234);
    for (int i = 0; i < 900; i++) {
        string message = "thread #" + to_string(thread_num) + " packet # " + to_string(i);
        Message request(MessageType::Request, (char *) message.c_str(), message.size(), 0);
        auto reply = client.execute(&request);
//        cout << (char *) reply.getMessage();
    }

}


//Testing
int main() {
    vector<thread> workers;
    for (int i = 0; i < 80; i++)
        workers.push_back(thread(client, i));

    for (int i = 0; i < 80; i++) {
        cout << "Waiting for thread #" << i << " to join()\n";
        workers[i].join();
    }

    cout << "All threads returned\n";

}


